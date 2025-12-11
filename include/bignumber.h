#ifndef BIGNUMBERS_LIBRARY_H
#define BIGNUMBERS_LIBRARY_H

#include <cstdint>
#include <string>
#include <vector>

namespace arbys::bignumbers {

    class BigNumber {
    private:
        std::vector<int> m_digits;
        size_t m_length = 0;

    public:
        BigNumber();

        /**
         * @brief Constructs a BigNumber from a string representation.
         *
         * This factory function parses a string containing digits and optional separators
         * into a BigNumber object. Leading zeros at the beginning of the number are removed.
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
        static BigNumber fromString(const std::string &str, const std::string &separator = "");

        static BigNumber fromInt(int nr);

        static BigNumber fromLLInt(long long nr);

        [[nodiscard]] uint8_t getDigit(size_t index) const;

        [[nodiscard]] BigNumber add(const BigNumber &other) const;

        [[nodiscard]] size_t getLength() const { return m_length; }

        BigNumber operator+(const BigNumber &other) const;

    private:
        /**
         * Constructor to create a BigNumber out of the vector
         * containing the digits in little-endian order and the
         * length of the number.
         *
         * @param digits Vector of digits stored in little-endian order.
         * @param length The length of the number.
         */
        BigNumber(const std::vector<int> &digits, const size_t length) : m_digits(digits), m_length(length) {}

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