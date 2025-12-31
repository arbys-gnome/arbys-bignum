#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <memory>
#include <string>
#include <string_view>

#include "errors.h"

namespace arbys::bignum {

namespace detail {
struct big_int_impl;
struct big_int_access;
} // namespace detail

class big_int {
  public:
    // Constructors and factory methods
    big_int();

    // Implicit conversion from integral types
    // This allows: big_int bn = 100000;
    template <std::integral T> big_int(T value) : big_int(from_integer(value)) {}

    // Copy and move constructors
    big_int(const big_int &other);
    big_int(big_int &&other) noexcept;

    ~big_int();

    // Copy and move assignment
    big_int &operator=(const big_int &other);
    big_int &operator=(big_int &&other) noexcept;

    // Assignment from integral types
    template <std::integral T> big_int &operator=(T value) {
        *this = from_integer(value);
        return *this;
    }

    // Factory methods for parsing
    [[nodiscard]] static std::expected<big_int, errors::ParseError> from_string(std::string_view input);
    [[nodiscard]] static std::expected<big_int, errors::ParseError> from_string(std::string_view input, char separator);
    [[nodiscard]] static std::expected<big_int, errors::ParseError> from_string(
      std::string_view input,
      std::string_view separator
    );

    // Factory method for integers
    template <std::integral T> [[nodiscard]] static big_int from_integer(T value);

    // Query methods
    [[nodiscard]] std::size_t length() const noexcept;
    [[nodiscard]] bool        is_negative() const noexcept;
    [[nodiscard]] bool        is_zero() const noexcept;
    [[nodiscard]] std::string to_string() const;

    template <class Out> Out format_to(Out out) const {
        const std::string s = to_string();
        return std::copy(s.begin(), s.end(), out);
    }

    // Arithmetic operations
    [[nodiscard]] big_int                                         add(const big_int &other) const noexcept;
    [[nodiscard]] big_int                                         sub(const big_int &other) const noexcept;
    [[nodiscard]] big_int                                         mul(const big_int &other) const noexcept;
    [[nodiscard]] std::expected<big_int, errors::ArithmeticError> div(const big_int &other) const noexcept;
    [[nodiscard]] std::expected<big_int, errors::ArithmeticError> mod(const big_int &other) const noexcept;
    [[nodiscard]] std::expected<std::pair<big_int, big_int>, errors::ArithmeticError> div_mod(
      const big_int &other
    ) const noexcept;

    [[nodiscard]] big_int abs() const;
    [[nodiscard]] big_int negate() const;

    // Operators
    [[nodiscard]] big_int  operator+(const big_int &other) const noexcept;
    big_int                operator+=(const big_int &other) noexcept;
    [[nodiscard]] big_int  operator-(const big_int &other) const noexcept;
    big_int                operator-=(const big_int &other) noexcept;
    [[nodiscard]] big_int  operator-() const noexcept; // unary minus
    [[nodiscard]] big_int  operator*(const big_int &other) const noexcept;
    [[nodiscard]] big_int  operator/(const big_int &other) const;
    [[nodiscard]] big_int  operator%(const big_int &other) const;
    big_int               &operator/=(const big_int &other);
    big_int               &operator%=(const big_int &other);
    [[nodiscard]] explicit operator bool() const noexcept;
    friend std::ostream   &operator<<(std::ostream &os, const big_int &bi);
    friend std::istream   &operator>>(std::istream &is, big_int &bi);

    // Comparison
    [[nodiscard]] std::strong_ordering operator<=>(const big_int &other) const;
    [[nodiscard]] bool                 operator==(const big_int &other) const;

  private:
    std::unique_ptr<detail::big_int_impl> impl_;

    // Private constructor for internal use
    explicit big_int(std::unique_ptr<detail::big_int_impl> impl);

    friend struct detail::big_int_access;
};

std::ostream &operator<<(std::ostream &os, const big_int &bi);
std::istream &operator>>(std::istream &is, big_int &bi);

} // namespace arbys::bignum