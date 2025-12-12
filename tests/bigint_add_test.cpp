#include <gtest/gtest.h>

#include "bigint.h"
#include "helpers/helpers.h"

namespace arbys::bignumbers::tests {
    TEST(BigIntAdd, SimpleNoCarry) {
        const BigInt a = BigInt::fromString("123");
        const BigInt b = BigInt::fromString("321");
        const BigInt r = a + b;
        helpers::expectBigIntEquals(r, "444");
    }

    TEST(BigIntAdd, SimpleCarry) {
        const BigInt a = BigInt::fromString("19");
        const BigInt b = BigInt::fromString("34");
        const BigInt r = a + b;
        helpers::expectBigIntEquals(r, "53");
    }

    TEST(BigIntAdd, CarryAcrossMultipleDigits) {
        const BigInt a = BigInt::fromString("999");
        const BigInt b = BigInt::fromString("1");
        const BigInt r = a + b;
        helpers::expectBigIntEquals(r, "1000");
    }

    TEST(BigIntAdd, DifferentLengthsNoCarry) {
        const BigInt a = BigInt::fromString("5000");
        const BigInt b = BigInt::fromString("20");
        const BigInt r = a + b;
        helpers::expectBigIntEquals(r, "5020");
    }

    TEST(BigIntAdd, DifferentLengthsWithCarry) {
        const BigInt a = BigInt::fromString("1090");
        const BigInt b = BigInt::fromString("20");
        const BigInt r = a + b;
        helpers::expectBigIntEquals(r, "1110");
    }

    TEST(BigIntAdd, AddingZero) {
        const BigInt a = BigInt::fromString("123456");
        const BigInt zero = BigInt::fromString("0");

        helpers::expectBigIntEquals(a + zero, "123456");
        helpers::expectBigIntEquals(zero + a, "123456");
    }

    TEST(BigIntAdd, LeadingZerosAreIgnored) {
        const BigInt a = BigInt::fromString("000123");
        const BigInt b = BigInt::fromString("000007");
        const BigInt r = a + b;
        helpers::expectBigIntEquals(r, "130");
    }

    TEST(BigIntAdd, LargeNumbers) {
        const BigInt a = BigInt::fromString("123456789012345678901234567890");
        const BigInt b = BigInt::fromString("987654321098765432109876543210");
        const BigInt r = a + b;

        helpers::expectBigIntEquals(r, "1111111110111111111011111111100");
    }

    TEST(BigIntAdd, Commutativity) {
        const BigInt a = BigInt::fromString("918273645");
        const BigInt b = BigInt::fromString("192837465");

        const BigInt r1 = a + b;
        const BigInt r2 = b + a;

        helpers::expectBigIntEquals(r1, "1111111110");
        helpers::expectBigIntEquals(r2, "1111111110");
    }

    TEST(BigIntAdd, ManyCarries) {
        const BigInt a = BigInt::fromString("55555555555555555555");
        const BigInt b = BigInt::fromString("55555555555555555555");
        const BigInt r = a + b;

        helpers::expectBigIntEquals(r, "111111111111111111110");
    }

    TEST(BigIntAdd, RandomizedSmall) {
        for (int i = 0; i < 100; i++) {
            const int x = rand() % 10000;
            const int y = rand() % 10000;

            BigInt a = BigInt::fromString(std::to_string(x));
            BigInt b = BigInt::fromString(std::to_string(y));
            BigInt r = a + b;

            helpers::expectBigIntEquals(r, std::to_string(x + y));
        }
    }

    TEST(BigIntAdd, RandomizedLarge) {
        for (int i = 0; i < 20; i++) {
            std::string s1, s2;

            for (int d = 0; d < 100; d++)
                s1 += '0' + (rand() % 10);

            for (int d = 0; d < 100; d++)
                s2 += '0' + (rand() % 10);

            BigInt a = BigInt::fromString(s1);
            BigInt b = BigInt::fromString(s2);

            // compute expected using builtin big-int via std::string
            std::string expected = helpers::bigIntegerStringAdd(s1, s2);

            BigInt result = a + b;
            helpers::expectBigIntEquals(result, expected);
        }
    }
} // namespace arbys::bignumbers::tests::add
