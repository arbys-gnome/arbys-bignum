#include <gtest/gtest.h>
#include "bignumber.h"

namespace arbys::bignumbers::tests {
    // ================= Helper Functions =================
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
        for (char ch : a)
            if (!std::isdigit(static_cast<unsigned char>(ch)))
                throw std::invalid_argument("Invalid digit in first argument to bigIntegerStringAdd.");

        for (char ch : b)
            if (!std::isdigit(static_cast<unsigned char>(ch)))
                throw std::invalid_argument("Invalid digit in second argument to bigIntegerStringAdd.");

        int i = static_cast<int>(a.size()) - 1;
        int j = static_cast<int>(b.size()) - 1;

        int carry = 0;
        std::string out;
        out.reserve(std::max(a.size(), b.size()) + 1);

        while (i >= 0 || j >= 0 || carry != 0) {
            int da = (i >= 0 ? a[i] - '0' : 0);
            int db = (j >= 0 ? b[j] - '0' : 0);

            int sum = da + db + carry;
            out.push_back(char('0' + (sum % 10)));
            carry = sum / 10;

            i--;
            j--;
        }

        // out is reversed → reverse it back
        std::reverse(out.begin(), out.end());
        return out;
    }

    // ================= No Separator Mode =================

    TEST(BigNumberFromString, NoSeparatorDigitsOnly) {
        const std::string s = makeIncreasing(10);

        // Leading zeros removed
        expectBigNumberEquals(BigNumber::fromString("000" + s, ""), s);

        // Whitespace ignored
        expectBigNumberEquals(BigNumber::fromString("  \t " + s + "  \n ", ""), s);
    }

    TEST(BigNumberFromString, NoSeparatorRejectsNonDigits) {
        EXPECT_THROW(BigNumber::fromString("12a34", ""), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString("12-34", ""), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString("   ", ""), std::invalid_argument);
    }

    // ================= Separator Mode =================

    TEST(BigNumberFromString, RegexSeparatorCommaSpace) {
        const std::string sep = " *, *";

        // Leading zeros in the first tokens are ignored
        const auto bn = BigNumber::fromString("0001,02,003,004", sep);
        expectBigNumberEquals(bn, "102003004");
    }

    TEST(BigNumberFromString, SeparatorPreservesInternalZeros) {
        const std::string sep = "[,; ]+";

        // Internal zeros in tokens preserved, first token leading zeros removed
        const auto bn = BigNumber::fromString("0001,0020,0030", sep);
        expectBigNumberEquals(bn, "100200030");
    }

    TEST(BigNumberFromString, SeparatorRejectsInvalidTokens) {
        const std::string sep = ",";

        EXPECT_THROW(BigNumber::fromString("1,a,2", sep), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString(",,", sep), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString("a,b,c", sep), std::invalid_argument);
    }

    // ================= Complex Cases =================

    TEST(BigNumberFromString, LeadingZerosOnlyTokensIgnored) {
        const std::string sep = ",";

        // Only the first token's leading zeros are removed, completely zero tokens ignored
        const auto bn = BigNumber::fromString("0000,0001,0020,0003", sep);
        expectBigNumberEquals(bn, "100200003");
    }

    TEST(BigNumberFromString, LargeNumberWithSpaces) {
        const std::string sep = " +";
        const std::string s = makeIncreasing(50);

        std::string spaced;
        for (const char c : s) spaced += std::string("  ") + c + " ";

        const auto bn = BigNumber::fromString(spaced, sep);
        expectBigNumberEquals(bn, s);
    }

    TEST(BigNumberFromString, ZeroValue) {
        EXPECT_NO_THROW(BigNumber::fromString("0000", ","));
        EXPECT_NO_THROW(BigNumber::fromString("    0000   ", ","));
    }

    TEST(BigNumberAdd, SimpleNoCarry) {
        const BigNumber a = BigNumber::fromString("123");
        const BigNumber b = BigNumber::fromString("321");
        const BigNumber r = a + b;
        expectBigNumberEquals(r, "444");
    }

    TEST(BigNumberAdd, SimpleCarry) {
        const BigNumber a = BigNumber::fromString("19");
        const BigNumber b = BigNumber::fromString("34");
        const BigNumber r = a + b;
        expectBigNumberEquals(r, "53");
    }

    TEST(BigNumberAdd, CarryAcrossMultipleDigits) {
        const BigNumber a = BigNumber::fromString("999");
        const BigNumber b = BigNumber::fromString("1");
        const BigNumber r = a + b;
        expectBigNumberEquals(r, "1000");
    }

    TEST(BigNumberAdd, DifferentLengthsNoCarry) {
        const BigNumber a = BigNumber::fromString("5000");
        const BigNumber b = BigNumber::fromString("20");
        const BigNumber r = a + b;
        expectBigNumberEquals(r, "5020");
    }

    TEST(BigNumberAdd, DifferentLengthsWithCarry) {
        const BigNumber a = BigNumber::fromString("1090");
        const BigNumber b = BigNumber::fromString("20");
        const BigNumber r = a + b;
        expectBigNumberEquals(r, "1110");
    }

    TEST(BigNumberAdd, AddingZero) {
        const BigNumber a = BigNumber::fromString("123456");
        const BigNumber zero = BigNumber::fromString("0");

        expectBigNumberEquals(a + zero, "123456");
        expectBigNumberEquals(zero + a, "123456");
    }

    TEST(BigNumberAdd, LeadingZerosAreIgnored) {
        const BigNumber a = BigNumber::fromString("000123");
        const BigNumber b = BigNumber::fromString("000007");
        const BigNumber r = a + b;
        expectBigNumberEquals(r, "130");
    }

    TEST(BigNumberAdd, LargeNumbers) {
        const BigNumber a = BigNumber::fromString("123456789012345678901234567890");
        const BigNumber b = BigNumber::fromString("987654321098765432109876543210");
        const BigNumber r = a + b;

        expectBigNumberEquals(r, "1111111110111111111011111111100");
    }

    TEST(BigNumberAdd, Commutativity) {
        const BigNumber a = BigNumber::fromString("918273645");
        const BigNumber b = BigNumber::fromString("192837465");

        const BigNumber r1 = a + b;
        const BigNumber r2 = b + a;

        expectBigNumberEquals(r1, "1111111110");
        expectBigNumberEquals(r2, "1111111110");
    }

    TEST(BigNumberAdd, ManyCarries) {
        const BigNumber a = BigNumber::fromString("55555555555555555555");
        const BigNumber b = BigNumber::fromString("55555555555555555555");
        const BigNumber r = a + b;

        expectBigNumberEquals(r, "111111111111111111110");
    }

    TEST(BigNumberAdd, RandomizedSmall) {
        for (int i = 0; i < 100; i++) {
            const int x = rand() % 10000;
            const int y = rand() % 10000;

            BigNumber a = BigNumber::fromString(std::to_string(x));
            BigNumber b = BigNumber::fromString(std::to_string(y));
            BigNumber r = a + b;

            expectBigNumberEquals(r, std::to_string(x + y));
        }
    }

    TEST(BigNumberAdd, RandomizedLarge) {
        for (int i = 0; i < 20; i++) {
            std::string s1, s2;

            for (int d = 0; d < 100; d++)
                s1 += '0' + (rand() % 10);

            for (int d = 0; d < 100; d++)
                s2 += '0' + (rand() % 10);

            BigNumber a = BigNumber::fromString(s1);
            BigNumber b = BigNumber::fromString(s2);

            // compute expected using builtin big-int via std::string
            std::string expected = bigIntegerStringAdd(s1, s2);

            BigNumber result = a + b;
            expectBigNumberEquals(result, expected);
        }
    }
} // namespace arbys::bignumbers::tests
