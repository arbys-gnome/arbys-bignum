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

/**
 * @brief Type for representing arbytrary prcision numbers
 */
class big_int {
  public:
    // Constructors and factory methods
    big_int();

    /**
     * @brief Implicit conversion constructor from integral types.
     * Allows: big_int bi = 100000;
     * @tparam T any std::integral type
     * @param value any std::integral
     */
    template <std::integral T> big_int(T value) : big_int(from_integer(value)) {}

    /**
     * @brief Copy constructor
     * @param other the big_int from which to copy the data
     */
    big_int(const big_int &other);

    /**
     * @brief Move constructor
     * @param other the big_int to move
     */
    big_int(big_int &&other) noexcept;

    ~big_int();

    /**
     * @brief Copy assignment operator
     * @param other the big_int from which to copy the data
     * @return big_int which has copied the data from other
     */
    big_int &operator=(const big_int &other);

    /**
     * @brief Move assignment operator
     * @param other the big_int to move
     * @return big_int which has moved the data from other
     */
    big_int &operator=(big_int &&other) noexcept;

    /**
     * @brief Integral assignment operator
     * @tparam T any std::integral type
     * @param value the value to store in big_int
     * @return big_int with the same value as 'value'
     */
    template <std::integral T> big_int &operator=(T value) {
        *this = from_integer(value);
        return *this;
    }

    /**
     * @brief Factory method for constructing a big_int out of a std::string_view
     * @param input
     * @return
     */
    [[nodiscard]] static std::expected<big_int, errors::ParseError> from_string(std::string_view input);
    /**
     * @brief Factory method for constructing a big_int out of a std::string_view
     * @param input
     * @param separator
     * @return
     */
    [[nodiscard]] static std::expected<big_int, errors::ParseError> from_string(std::string_view input, char separator);
    /**
     * @brief Factory method for constructing a big_int out of a std::string_view
     * @param input
     * @param separator
     * @return
     */
    [[nodiscard]] static std::expected<big_int, errors::ParseError> from_string(
      std::string_view input,
      std::string_view separator
    );

    /**
     * @brief Factory method for constructing a big_int out of a std::integral
     * @tparam T
     * @param value
     * @return
     */
    template <std::integral T> [[nodiscard]] static big_int from_integer(T value);

    /**
     * @brief Returns weather or not the numbr is negative
     * @return true if the number is negative, false if it is positive
     */
    [[nodiscard]] bool is_negative() const noexcept;

    /**
     * @brief Returns weather or not the number is zero
     * @return true if the number is zero, false otherwise
     */
    [[nodiscard]] bool is_zero() const noexcept;

    /**
     * @brief Converts the number to a std::string in base 10
     * @return std::string representing the number in base 10
     */
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

    /**
     * @brief Returns the absolute value of the number
     * @return the absolute value of the number
     */
    [[nodiscard]] big_int abs() const;

    /**
     * @brief Changes the sign of the number
     * @return A copy of the number with the sign changed
     */
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
    // private implementation pointer (PImpl)
    std::unique_ptr<detail::big_int_impl> impl_;

    // Private constructor for internal use
    explicit big_int(std::unique_ptr<detail::big_int_impl> impl);

    // access struct: allows implementations to access private fields
    friend struct detail::big_int_access;
};

std::ostream &operator<<(std::ostream &os, const big_int &bi);
std::istream &operator>>(std::istream &is, big_int &bi);

} // namespace arbys::bignum