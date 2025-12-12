#include <gtest/gtest.h>

#include "bignumber.h"

namespace arbys::bignumbers::tests::helpers {
    std::string makeIncreasing(const size_t len) {
        std::string s;
        for (size_t i = 1; i <= len; i++)
            s += std::to_string(i % 10);
        return s;
    }

    void expectBigNumberEquals(const BigNumber &bn, const std::string &s) {
        ASSERT_EQ(bn.getLength(), s.size());
        for (size_t i = 0; i < s.size(); i++)
            EXPECT_EQ(bn.getDigit(i), s[i] - '0');
    }

    std::string bigIntegerStringAdd(const std::string& a, const std::string& b) {
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