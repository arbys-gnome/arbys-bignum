#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/bigint.h"

namespace arbys::bignumbers::tests::helpers {
    std::string big_int_string_div(const std::string &a, const std::string &b) {
        const long long lhs  = std::stoll(a);
        const long long rhs  = std::stoll(b);
        if (rhs == 0)
            throw std::invalid_argument("division by zero");
        const long long res = lhs / rhs;
        return std::to_string(res);
    }

    void expect_compare_eq(const BigInt& a, const BigInt& b, const std::strong_ordering expected) {
        const auto r = a.cmp(b);
        EXPECT_EQ(r, expected);
    }

    std::string make_increasing(const size_t len) {
        std::string s;
        for (size_t i = 1; i <= len; i++)
            s += std::to_string(i % 10);
        return s;
    }

    void expect_eq(const BigInt &bn, const std::string &s) {
        ASSERT_EQ(bn.get_length(), s.size());
        for (size_t i = 0; i < s.size(); i++)
            EXPECT_EQ(bn.get_digit(i), s[i] - '0');
    }

    void expect_eq(const BigInt &bn, const BigInt &other) {
        ASSERT_EQ(bn.get_length(), other.get_length());
        for (size_t i = 0; i < bn.get_length(); i++)
            EXPECT_EQ(bn.get_digit(i), other.get_digit(i));
    }

    std::string big_int_string_add(const std::string& a, const std::string& b) {
        // Ensure both inputs contain only digits
        for (const char ch : a)
            if (!std::isdigit(static_cast<unsigned char>(ch)))
                throw std::invalid_argument("Invalid digit in first argument to bigIntegerStringAdd.");

        for (const char ch : b)
            if (!std::isdigit(static_cast<unsigned char>(ch)))
                throw std::invalid_argument("Invalid digit in second argument to bigIntegerStringAdd.");

        int i = static_cast<int>(a.size()) - 1;
        int j = static_cast<int>(b.size()) - 1;

        int carry = 0;
        std::string out;
        out.reserve(std::max(a.size(), b.size()) + 1);

        while (i >= 0 || j >= 0 || carry != 0) {
            const int da = (i >= 0 ? a[i] - '0' : 0);
            const int db = (j >= 0 ? b[j] - '0' : 0);

            const int sum = da + db + carry;
            out.push_back(static_cast<char>('0' + (sum % 10)));
            carry = sum / 10;

            i--;
            j--;
        }

        // out is reversed → reverse it back
        std::reverse(out.begin(), out.end());
        return out;
    }
}