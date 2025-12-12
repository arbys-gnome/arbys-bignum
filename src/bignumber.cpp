#include "bignumber.h"

#include <regex>
#include <stdexcept>
#include <ranges>

namespace arbys::bignumbers {
    BigNumber::BigNumber() = default;

    BigNumber BigNumber::fromString(const std::string& str, const std::string& separator) {
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

        // Convert to BigNumber internal format
        const size_t length = digits_str.length();
        std::vector<int> digits(length);

        // BigNumber stores least significant digit at index 0 (reverse order)
        for (size_t i = 0; i < length; i++)
            digits[i] = digits_str[length - 1 - i] - '0';

        return { digits, length };
    }

    BigNumber BigNumber::fromInt(int nr) {
        // TODO: implement
        return {};
    }

    BigNumber BigNumber::fromLLInt(long long nr) {
        // TODO: implement
        return {};
    }

    uint8_t BigNumber::getDigit(const size_t index) const {
        return m_digits[m_length - 1 - index];
    }

    BigNumber BigNumber::add(const BigNumber& other) const {
        const BigNumber* bigger;
        const BigNumber* smaller;

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

    BigNumber BigNumber::operator+(const BigNumber &other) const {
        return add(other);
    }

    BigNumber BigNumber::sub(const BigNumber &other) const {
        return {};
    }

    BigNumber BigNumber::operator-(const BigNumber &other) const {
        return sub(other);
    }

} // arbys::bignumbers

