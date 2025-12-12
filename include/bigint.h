#ifndef BIGNUMBERS_LIBRARY_H
#define BIGNUMBERS_LIBRARY_H

#include <cstdint>
#include <string>
#include <vector>
#include <compare>

namespace arbys::bignumbers {

    class BigInt {
    private:
        std::vector<int> m_digits;
        size_t m_length = 0;

    public:
        BigInt();

        /**
         * @brief Constructs a BigNumber from a string representation.
         *
         * This factory function parses a string containing digits and optional separators
         * into a BigInt object. Leading zeros at the beginning of the number are removed.
         * Trailing zeros and zeros inside the number are preserved. Leading and trailing
         * whitespaces are ignored.
         *
         * @param str The input string containing the number.
         * @param separator Optional regular expression string used to split tokens. If empty, the
         *                  string is expected to contain only digits.
         *
         * @return BigNumber The resulting BigNumber object.
         *
         *
         * @throws std::invalid_argument If:
         *   - The input contains no digits after removing leading and trailing whitespace.
         *   - A token contains invalid (non-digit) characters.
         */
        static BigInt fromString(const std::string &str, const std::string &separator = "");

        static BigInt fromInt(int nr);

        static BigInt fromLLInt(long long nr);

        [[nodiscard]] uint8_t getDigit(size_t index) const;

        [[nodiscard]] size_t getLength() const { return m_length; }

        /**
         * @brief Compares this BigInt with another BigInt using total ordering.
         *
         * Performs a lexicographical comparison based on the mathematical value
         * represented by the BigInt. The comparison is consistent with the internal
         * representation, where digits are stored in reverse order
         * (least-significant digit at index 0) and no leading zeros are present.
         *
         * This function provides a strong total ordering, meaning:
         *  - All BigInt values are comparable.
         *  - Equality and ordering behave exactly like built-in integer types.
         *
         * @param other The BigInt to compare against.
         *
         * @return std::strong_ordering
         *         - std::strong_ordering::less    if *this < other
         *         - std::strong_ordering::equal   if *this == other
         *         - std::strong_ordering::greater if *this > other
         *
         * This function is intended to be used by the three-way comparison operator
         * (operator<=>), enabling automatic generation of all comparison operators
         * (<, <=, >, >=, ==, !=) in accordance with C++20 ordering rules.
         */
        [[nodiscard]] std::strong_ordering compare(const BigInt &other) const;

        std::strong_ordering operator<=>(const BigInt& other) const { return compare(other); }
        bool operator==(const BigInt& other) const = default;

        [[nodiscard]] BigInt add(const BigInt &other) const;

        BigInt operator+(const BigInt &other) const;

        [[nodiscard]] BigInt sub(const BigInt &other) const;

        BigInt operator-(const BigInt &other) const;

    private:
        /**
         * Constructor to create a BigInt out of the vector
         * containing the digits in little-endian order and the
         * length of the number.
         *
         * @param digits Vector of digits stored in little-endian order.
         * @param length The length of the number.
         */
        BigInt(const std::vector<int> &digits, size_t length);

        static std::string trim(const std::string& s) {
            if (s.empty())
                return "";

            size_t start = 0;
            while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
                start++;
            }

            if (start == s.size()) {
                // string is all whitespace
                return "";
            }

            size_t end = s.size() - 1;
            while (end > start && std::isspace(static_cast<unsigned char>(s[end]))) {
                end--;
            }

            return s.substr(start, end - start + 1);
        }
    };

}

#endif // BIGNUMBERS_LIBRARY_H