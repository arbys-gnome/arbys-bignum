#ifndef BIGNUMBERS_LIBRARY_H
#define BIGNUMBERS_LIBRARY_H

#include <cstdint>
#include <string>
#include <vector>
#include <compare>
#include <expected>

#include "errors.h"

namespace arbys::bignumbers {
    typedef uint8_t limb_t;

    class BigInt;

    class BigInt {
    public:
        BigInt();

        static std::expected<BigInt, ParseError>
        from_string(std::string_view input, std::string_view separator);

        static std::expected<BigInt, ParseError>
        from_string(std::string_view input);

        static std::expected<BigInt, ParseError>
        from_string(std::string_view input, char separator);

        template<std::integral IntType>
        static std::expected<BigInt, ParseError> from_integer(IntType value) {
            bool is_negative = value < 0;
            using UnsignedType = std::make_unsigned_t<IntType>;
            UnsignedType abs_value = is_negative ? -UnsignedType(value) : value;

            std::vector<limb_t> digits;
            if (abs_value == 0) {
                digits.push_back(0);
            } else {
                while (abs_value != 0) {
                    digits.push_back(abs_value % 10);
                    abs_value /= 10;
                }
            }

            return BigInt(is_negative, digits, digits.size());
        }

        [[nodiscard]] limb_t get_digit(size_t index) const;
        [[nodiscard]] size_t get_length() const noexcept { return length_; }
        [[nodiscard]] bool is_negative() const noexcept { return is_negative_; }
        [[nodiscard]] bool is_zero() const noexcept { return length_ == 1 && digits_[0] == 0; }


        [[nodiscard]] std::strong_ordering
        cmp(const BigInt &other) const;

        /**
         * @brief Three-way comparison operator.
         *
         * Performs lexicographical comparison based on mathematical value.
         * Provides strong total ordering for BigInt values.
         *
         * @param other The BigInt to compare against
         * @return std::strong_ordering representing the comparison result
         */
        [[nodiscard]] std::strong_ordering
        operator<=>(const BigInt& other) const { return cmp(other); }

        bool operator==(const BigInt& other) const = default;

        [[nodiscard]] BigInt add(const BigInt &other) const;
        [[nodiscard]] BigInt sub(const BigInt &other) const;
        [[nodiscard]] BigInt mul(const BigInt &other) const;
        [[nodiscard]] BigInt div(long long other) const;

        BigInt operator+(const BigInt &other) const;
        BigInt operator-(const BigInt &other) const;
        BigInt operator*(const BigInt &other) const;
        BigInt operator/(long long other) const;

        BigInt *to_abs();
        [[nodiscard]] std::string to_string() const;

    private:
        std::vector<limb_t> digits_;
        size_t length_ = 0;
        bool is_negative_ = false;

        /**
         * Constructor to create a BigInt out of the vector
         * containing the digits in little-endian order and the
         * length of the number.
         *
         * @param is_negative Boolean to determine whether the number is negative
         * @param digits Vector of digits stored in little-endian order.
         * @param length The length of the number.
         */
        BigInt(bool is_negative, const std::vector<limb_t> &digits, size_t length);

        friend class BigIntImpl;
    };

}

#endif // BIGNUMBERS_LIBRARY_H
