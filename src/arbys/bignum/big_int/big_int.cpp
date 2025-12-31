#include "arbys/bignum/big_int.h"
#include "../detail/big_int_internal.h"
#include "../detail/config.h"
#include "../detail/detail.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <istream>
#include <ranges>
#include <stdexcept>

namespace arbys::bignum {

big_int::big_int() : impl_(std::make_unique<detail::big_int_impl>()) {}

big_int::~big_int() = default;

big_int::big_int(const big_int &other) : impl_(std::make_unique<detail::big_int_impl>(*other.impl_)) {}

big_int::big_int(big_int &&other) noexcept = default;

big_int &big_int::operator=(const big_int &other) {
    if (this != &other) {
        impl_ = std::make_unique<detail::big_int_impl>(*other.impl_);
    }
    return *this;
}

big_int &big_int::operator=(big_int &&other) noexcept = default;

big_int::big_int(std::unique_ptr<detail::big_int_impl> impl) : impl_(std::move(impl)) {}

template <std::integral T> big_int big_int::from_integer(T value) {
    bool is_negative   = value < 0;
    using Unsigned     = std::make_unsigned_t<T>;
    Unsigned abs_value = is_negative ? -static_cast<Unsigned>(value) : static_cast<Unsigned>(value);

    std::vector<detail::limb_t> limbs;

    if (abs_value == 0) {
        limbs.push_back(0);
    } else {
        // Convert to base 2^32 representation
        constexpr detail::dlimb_t LIMB_MASK = (detail::dlimb_t{1} << detail::LIMB_BITS) - 1;

        while (abs_value != 0) {
            limbs.push_back(static_cast<detail::limb_t>(abs_value & LIMB_MASK));
            if constexpr (sizeof(T) > sizeof(detail::limb_t)) {
                abs_value >>= detail::LIMB_BITS;
            } else {
                abs_value = 0;
            }
        }
    }

    return big_int(std::make_unique<detail::big_int_impl>(is_negative, std::move(limbs)));
}

// Explicit instantiations for common integer types
template big_int big_int::from_integer<int>(int);
template big_int big_int::from_integer<long>(long);
template big_int big_int::from_integer<long long>(long long);
template big_int big_int::from_integer<unsigned int>(unsigned int);
template big_int big_int::from_integer<unsigned long>(unsigned long);
template big_int big_int::from_integer<unsigned long long>(unsigned long long);

std::expected<big_int, errors::ParseError> big_int::from_string(std::string_view input) {
    input = detail::trim_view(input);
    if (input.empty()) {
        return std::unexpected(errors::ParseError::EmptyInput);
    }

    bool is_negative = false;
    if (input.front() == '-') {
        is_negative = true;
        input.remove_prefix(1);
    } else if (input.front() == '+') {
        input.remove_prefix(1);
    }

    if (input.empty()) {
        return std::unexpected(errors::ParseError::NoDigits);
    }

    return detail::parse_limbs(input, is_negative);
}

std::expected<big_int, errors::ParseError> big_int::from_string(std::string_view input, const char separator) {
    input = detail::trim_view(input);
    if (input.empty()) {
        return std::unexpected(errors::ParseError::EmptyInput);
    }

    bool is_negative = false;
    if (input.front() == '-') {
        is_negative = true;
        input.remove_prefix(1);
    } else if (input.front() == '+') {
        input.remove_prefix(1);
    }

    std::string compact;
    compact.reserve(input.size());

    for (const char ch : input) {
        if (ch == separator) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return std::unexpected(errors::ParseError::InvalidCharacter);
        }
        compact.push_back(ch);
    }

    if (compact.empty()) {
        return std::unexpected(errors::ParseError::NoDigits);
    }

    return detail::parse_limbs(compact, is_negative);
}

std::expected<big_int, errors::ParseError> big_int::from_string(
  std::string_view       input,
  const std::string_view separator
) {
    if (separator.empty()) {
        return from_string(input);
    }

    input = detail::trim_view(input);
    if (input.empty()) {
        return std::unexpected(errors::ParseError::EmptyInput);
    }

    bool is_negative = false;
    if (input.front() == '-') {
        is_negative = true;
        input.remove_prefix(1);
    } else if (input.front() == '+') {
        input.remove_prefix(1);
    }

    std::string compact;
    compact.reserve(input.size());

    while (!input.empty()) {
        const size_t           pos   = input.find(separator);
        const std::string_view chunk = input.substr(0, pos);

        for (const char c : chunk) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                return std::unexpected(errors::ParseError::InvalidCharacter);
            }
            compact.push_back(c);
        }

        if (pos == std::string_view::npos) {
            break;
        }
        input.remove_prefix(pos + separator.size());
    }

    if (compact.empty()) {
        return std::unexpected(errors::ParseError::NoDigits);
    }

    return detail::parse_limbs(compact, is_negative);
}

size_t big_int::length() const noexcept { return impl_->length(); }

bool big_int::is_negative() const noexcept { return impl_->is_negative_; }

bool big_int::is_zero() const noexcept { return impl_->length_ == 1 && impl_->limbs_[0] == 0; }

std::string big_int::to_string() const {
    if (is_zero()) {
        return "0";
    }

    // Convert from base 2^32 to base 10
    // Use a temporary copy for the conversion
    std::vector<detail::limb_t> temp     = impl_->limbs_;
    size_t                      temp_len = impl_->length_;
    std::string                 result;
    result.reserve(temp_len * 10); // rough estimate: log10(2^32) ≈ 9.63

    // Repeatedly divide by 10 to extract decimal digits
    while (temp_len > 1 || temp[0] > 0) {
        detail::dlimb_t remainder = 0;

        // Divide the entire number by 10 (from most significant to least)
        for (size_t i = temp_len; i-- > 0;) {
            const detail::dlimb_t current = (remainder << detail::LIMB_BITS) | temp[i];
            temp[i]                       = static_cast<detail::limb_t>(current / 10);
            remainder                     = current % 10;
        }

        // Remove leading zero limbs
        while (temp_len > 1 && temp[temp_len - 1] == 0) {
            temp_len--;
        }

        result += static_cast<char>('0' + remainder);
    }

    if (impl_->is_negative_) {
        result += '-';
    }

    std::ranges::reverse(result);
    return result;
}

std::strong_ordering big_int::operator<=>(const big_int &other) const {
    // Different signs
    if (impl_->is_negative_ != other.impl_->is_negative_) {
        return impl_->is_negative_ ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    // Same sign
    const std::strong_ordering abs_cmp = detail::cmp_abs(*this, other);

    // Both negative
    if (impl_->is_negative_) {
        if (abs_cmp == std::strong_ordering::less) {
            return std::strong_ordering::greater;
        }
        if (abs_cmp == std::strong_ordering::greater) {
            return std::strong_ordering::less;
        }
    }

    return abs_cmp;
}

bool big_int::operator==(const big_int &other) const {
    return impl_->is_negative_ == other.impl_->is_negative_ && impl_->limbs_ == other.impl_->limbs_;
}

big_int big_int::abs() const {
    auto result          = std::make_unique<detail::big_int_impl>(*impl_);
    result->is_negative_ = false;
    return big_int(std::move(result));
}

big_int big_int::negate() const {
    auto result = std::make_unique<detail::big_int_impl>(*impl_);
    if (!is_zero()) {
        result->is_negative_ = !result->is_negative_;
    }
    return big_int(std::move(result));
}

big_int big_int::add(const big_int &other) const noexcept {
    if (impl_->is_negative_ == other.impl_->is_negative_) {
        big_int result             = detail::add_abs(*this, other);
        result.impl_->is_negative_ = impl_->is_negative_;
        result.impl_->normalize();
        return result;
    }

    // Different signs
    const std::strong_ordering cmp = detail::cmp_abs(*this, other);

    if (cmp == std::strong_ordering::equal) {
        return big_int{};
    }

    if (cmp == std::strong_ordering::greater) {
        big_int result             = detail::sub_abs(*this, other);
        result.impl_->is_negative_ = impl_->is_negative_;
        return result;
    }

    big_int result             = detail::sub_abs(other, *this);
    result.impl_->is_negative_ = other.impl_->is_negative_;
    return result;
}

big_int big_int::sub(const big_int &other) const noexcept { return add(other.negate()); }

big_int big_int::mul(const big_int &other) const noexcept {
    if (is_zero() || other.is_zero()) {
        return big_int();
    }

    big_int result             = detail::mul_abs(*this, other);
    result.impl_->is_negative_ = impl_->is_negative_ != other.impl_->is_negative_;
    result.impl_->normalize();
    return result;
}

std::expected<big_int, errors::ArithmeticError> big_int::div(const big_int &other) const noexcept {
    if (is_zero()) {
        return big_int(); // 0 / anything = 0 (except 0)
    }

    auto result = detail::div_abs(*this, other);
    if (!result) {
        return std::unexpected(result.error());
    }

    // Apply sign rules
    const bool result_negative = impl_->is_negative_ != other.impl_->is_negative_;
    if (result_negative && !result->is_zero()) {
        result->impl_->is_negative_ = true;
    }

    return *result;
}

std::expected<big_int, errors::ArithmeticError> big_int::mod(const big_int &other) const noexcept {
    if (is_zero()) {
        return big_int(); // 0 % anything = 0 (except 0)
    }

    auto result = detail::mod_abs(*this, other);
    if (!result) {
        return std::unexpected(result.error());
    }

    // Remainder has same sign as dividend
    if (impl_->is_negative_ && !result->is_zero()) {
        result->impl_->is_negative_ = true;
    }

    return *result;
}

std::expected<std::pair<big_int, big_int>, errors::ArithmeticError> big_int::div_mod(
  const big_int &other
) const noexcept {
    if (is_zero()) {
        return std::pair{big_int(), big_int()};
    }

    auto result = detail::div_mod_abs(*this, other);
    if (!result) {
        return std::unexpected(result.error());
    }

    // Apply sign rules
    const bool quotient_negative = impl_->is_negative_ != other.impl_->is_negative_;
    if (quotient_negative && !result->quotient.is_zero()) {
        result->quotient.impl_->is_negative_ = true;
    }

    // Remainder has same sign as dividend
    if (impl_->is_negative_ && !result->remainder.is_zero()) {
        result->remainder.impl_->is_negative_ = true;
    }

    return std::pair{std::move(result->quotient), std::move(result->remainder)};
}

big_int big_int::operator/(const big_int &other) const {
    auto result = div(other);
    if (!result) {
        throw std::domain_error(std::string(errors::to_string(result.error())));
    }
    return *result;
}

big_int big_int::operator%(const big_int &other) const {
    auto result = mod(other);
    if (!result) {
        throw std::domain_error(std::string(errors::to_string(result.error())));
    }
    return *result;
}

big_int &big_int::operator/=(const big_int &other) {
    *this = *this / other; // Uses throwing operator
    return *this;
}

big_int &big_int::operator%=(const big_int &other) {
    *this = *this % other; // Uses throwing operator
    return *this;
}

big_int::operator bool() const noexcept { return !is_zero(); }

bool big_int::operator!() const noexcept { return !static_cast<bool>(*this); }

std::ostream &operator<<(std::ostream &os, const big_int &bi) {
    // TODO: try to avoid temporary string (consider bi.write_to(os) and write bytes directly)
    os << bi.to_string();
    return os;
}

std::istream& operator>>(std::istream& is, big_int& bi) {
    std::string token;
    if (!(is >> token))
        return is;

    auto parsed = big_int::from_string(token);
    if (!parsed) {
        is.setstate(std::ios::failbit);
        return is;
    }

    bi = std::move(*parsed);
    return is;
}

big_int big_int::operator+(const big_int &other) const noexcept { return add(other); }

big_int big_int::operator+=(const big_int &other) noexcept {
    *this = *this + other;
    return *this;
}

big_int big_int::operator-(const big_int &other) const noexcept { return sub(other); }

big_int big_int::operator-=(const big_int &other) noexcept {
    *this = *this - other;
    return *this;
}

big_int big_int::operator*(const big_int &other) const noexcept { return mul(other); }

big_int big_int::operator-() const noexcept { return negate(); }

} // namespace arbys::bignum
