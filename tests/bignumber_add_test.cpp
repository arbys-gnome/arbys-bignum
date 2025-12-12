#include <gtest/gtest.h>

#include "bignumber.h"
#include "helpers/helpers.h"

namespace arbys::bignumbers::tests::add {
    TEST(BigNumberAdd, SimpleNoCarry) {
        const BigNumber a = BigNumber::fromString("123");
        const BigNumber b = BigNumber::fromString("321");
        const BigNumber r = a + b;
        helpers::expectBigNumberEquals(r, "444");
    }

    TEST(BigNumberAdd, SimpleCarry) {
        const BigNumber a = BigNumber::fromString("19");
        const BigNumber b = BigNumber::fromString("34");
        const BigNumber r = a + b;
        helpers::expectBigNumberEquals(r, "53");
    }

    TEST(BigNumberAdd, CarryAcrossMultipleDigits) {
        const BigNumber a = BigNumber::fromString("999");
        const BigNumber b = BigNumber::fromString("1");
        const BigNumber r = a + b;
        helpers::expectBigNumberEquals(r, "1000");
    }

    TEST(BigNumberAdd, DifferentLengthsNoCarry) {
        const BigNumber a = BigNumber::fromString("5000");
        const BigNumber b = BigNumber::fromString("20");
        const BigNumber r = a + b;
        helpers::expectBigNumberEquals(r, "5020");
    }

    TEST(BigNumberAdd, DifferentLengthsWithCarry) {
        const BigNumber a = BigNumber::fromString("1090");
        const BigNumber b = BigNumber::fromString("20");
        const BigNumber r = a + b;
        helpers::expectBigNumberEquals(r, "1110");
    }

    TEST(BigNumberAdd, AddingZero) {
        const BigNumber a = BigNumber::fromString("123456");
        const BigNumber zero = BigNumber::fromString("0");

        helpers::expectBigNumberEquals(a + zero, "123456");
        helpers::expectBigNumberEquals(zero + a, "123456");
    }

    TEST(BigNumberAdd, LeadingZerosAreIgnored) {
        const BigNumber a = BigNumber::fromString("000123");
        const BigNumber b = BigNumber::fromString("000007");
        const BigNumber r = a + b;
        helpers::expectBigNumberEquals(r, "130");
    }

    TEST(BigNumberAdd, LargeNumbers) {
        const BigNumber a = BigNumber::fromString("123456789012345678901234567890");
        const BigNumber b = BigNumber::fromString("987654321098765432109876543210");
        const BigNumber r = a + b;

        helpers::expectBigNumberEquals(r, "1111111110111111111011111111100");
    }

    TEST(BigNumberAdd, Commutativity) {
        const BigNumber a = BigNumber::fromString("918273645");
        const BigNumber b = BigNumber::fromString("192837465");

        const BigNumber r1 = a + b;
        const BigNumber r2 = b + a;

        helpers::expectBigNumberEquals(r1, "1111111110");
        helpers::expectBigNumberEquals(r2, "1111111110");
    }

    TEST(BigNumberAdd, ManyCarries) {
        const BigNumber a = BigNumber::fromString("55555555555555555555");
        const BigNumber b = BigNumber::fromString("55555555555555555555");
        const BigNumber r = a + b;

        helpers::expectBigNumberEquals(r, "111111111111111111110");
    }

    TEST(BigNumberAdd, RandomizedSmall) {
        for (int i = 0; i < 100; i++) {
            const int x = rand() % 10000;
            const int y = rand() % 10000;

            BigNumber a = BigNumber::fromString(std::to_string(x));
            BigNumber b = BigNumber::fromString(std::to_string(y));
            BigNumber r = a + b;

            helpers::expectBigNumberEquals(r, std::to_string(x + y));
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
            std::string expected = helpers::bigIntegerStringAdd(s1, s2);

            BigNumber result = a + b;
            helpers::expectBigNumberEquals(result, expected);
        }
    }
} // namespace arbys::bignumbers::tests::add
