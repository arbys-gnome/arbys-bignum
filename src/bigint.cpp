#include "bigint.h"

#include <regex>
#include <stdexcept>
#include <ranges>

namespace arbys::bignumbers {
    // TODO: implement negative numbers
    BigInt::BigInt() = default;

    BigInt::BigInt(const std::vector<int> &digits, const size_t length) : m_digits(digits), m_length(length) {
        m_digits.resize(m_length);
    }

    BigInt BigInt::fromString(const std::string& str, const std::string& separator) {
        // Remove all whitespace
        std::string trimmed_str;
        trimmed_str = trim(str);

        if (trimmed_str.empty())
            throw std::invalid_argument("Input contains no digits.");

        std::vector<std::string> tokens;

        if (separator.empty()) {
            // No separator: only digits allowed
            tokens.push_back(trimmed_str);
        } else {
            // Split using regex separator
            std::regex sep(separator);
            std::sregex_token_iterator it(trimmed_str.begin(), trimmed_str.end(), sep, -1);
            std::sregex_token_iterator end;

            for (; it != end; ++it) {
                if (it->str().empty())
                    continue; // skip empty tokens from consecutive separators
                tokens.push_back(it->str());
            }

            if (tokens.empty())
                throw std::invalid_argument("Input contains no valid numeric tokens.");
        }

        // Parse digits from tokens
        std::string digits_str;
        bool leading_zero = true;
        for (const auto& tok : tokens) {
            // Validate token: must contain only digits
            for (char ch : tok) {
                if (!std::isdigit(ch))
                    throw std::invalid_argument("Invalid character in number token.");
            }

            if (leading_zero && tok.starts_with('0')) {
                // Remove leading zeros
                size_t first_non_zero = tok.find_first_not_of('0');
                if (first_non_zero == std::string::npos) {
                    // Token is "0000" → ignored
                    continue;
                }
                digits_str += tok.substr(first_non_zero);
            } else {
                digits_str += tok;
            }
            leading_zero = false;
        }

        if (digits_str.empty())
            digits_str = "0";

        // Convert to BigInt internal format
        const size_t length = digits_str.length();
        std::vector<int> digits(length);

        // BigInt stores least significant digit at index 0 (reverse order)
        for (size_t i = 0; i < length; i++)
            digits[i] = digits_str[length - 1 - i] - '0';

        return { digits, length };
    }

    BigInt BigInt::fromInt(int nr) {
        // TODO: implement
        return {};
    }

    BigInt BigInt::fromLLInt(long long nr) {
        // TODO: implement
        return {};
    }

    std::strong_ordering BigInt::compare(const BigInt &other) const {
        if (this->m_length != other.m_length) {
            return (this->m_length < other.m_length)
                    ? std::strong_ordering::less
                    : std::strong_ordering::greater;
        }

        for (size_t i = 0; i < this->m_length; i++) {
            const size_t j = this->m_length - 1 - i;
            if (this->m_digits[j] != other.m_digits[j]) {
                return (this->m_digits[j] < other.m_digits[j])
                        ? std::strong_ordering::less
                        : std::strong_ordering::greater;
            }
        }

        return std::strong_ordering::equal;
    }

    uint8_t BigInt::getDigit(const size_t index) const {
        return m_digits[m_length - 1 - index];
    }

    BigInt BigInt::add(const BigInt& other) const {
        const BigInt* bigger;
        const BigInt* smaller;

        if (this->m_length < other.m_length) {
            smaller = this;
            bigger  = &other;
        } else {
            smaller = &other;
            bigger  = this;
        }

        std::vector<int> digits;
        digits.resize(bigger->m_length);

        int carry = 0;
        int i = 0;

        // Add common digits
        for (; i < smaller->m_length; ++i) {
            const int sum = smaller->m_digits[i] + bigger->m_digits[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }

        // Add remaining digits from the bigger number
        for (; i < bigger->m_length; ++i) {
            const int sum = bigger->m_digits[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }

        // Extra carry at the highest digit
        if (carry) digits.push_back(carry);

        return { digits, digits.size() };
    }

    BigInt BigInt::operator+(const BigInt &other) const {
        return add(other);
    }

    BigInt BigInt::sub(const BigInt &other) const {
        const BigInt* bigger;
        const BigInt* smaller;

        if (this->m_length < other.m_length) {
            smaller = this;
            bigger  = &other;
        } else {
            smaller = &other;
            bigger  = this;
        }

        std::vector<int> digits;
        digits.resize(bigger->m_length);

        int borrow = 0;
        size_t i = 0;
        for (; i < smaller->m_length; ++i) {
            if (bigger->m_digits[i] - borrow >= smaller->m_digits[i]) {
                digits[i] = bigger->m_digits[i] - smaller->m_digits[i] - borrow;
                borrow = 0;
            } else {
                digits[i] = 10 + bigger->m_digits[i] - smaller->m_digits[i] - borrow;
                borrow = 1;
            }
        }

        for (; i < bigger->m_length; ++i) {
            if (borrow) {
                if (bigger->m_digits[i] == 0) {
                    digits[i] = 9;
                } else {
                    digits[i] = bigger->m_digits[i] - borrow;
                    borrow = 0;
                }
            } else {
                digits[i] = bigger->m_digits[i];
            }
        }

        size_t length = digits.size();
        for (const auto d : digits | std::views::drop(1) | std::views::reverse) {
            if (d == 0) length--;
        }

        return { digits, length };
    }

    BigInt BigInt::operator-(const BigInt &other) const {
        return sub(other);
    }

} // arbys::bignumbers

