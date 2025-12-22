#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/bigint.h"
#include "../helpers/helpers.h"

namespace arbys::bignumbers::tests {
    TEST(BigIntAdd, SimpleNoCarry) {
        const BigInt a = BigInt::from_string("123").value();
        const BigInt b = BigInt::from_string("321").value();
        const BigInt r = a + b;
        helpers::expect_eq(r, "444");
    }

    TEST(BigIntAdd, SimpleCarry) {
        const BigInt a = BigInt::from_string("19").value();
        const BigInt b = BigInt::from_string("34").value();
        const BigInt r = a + b;
        helpers::expect_eq(r, "53");
    }

    TEST(BigIntAdd, CarryAcrossMultipleDigits) {
        const BigInt a = BigInt::from_string("999").value();
        const BigInt b = BigInt::from_string("1").value();
        const BigInt r = a + b;
        helpers::expect_eq(r, "1000");
    }

    TEST(BigIntAdd, DifferentLengthsNoCarry) {
        const BigInt a = BigInt::from_string("5000").value();
        const BigInt b = BigInt::from_string("20").value();
        const BigInt r = a + b;
        helpers::expect_eq(r, "5020");
    }

    TEST(BigIntAdd, DifferentLengthsWithCarry) {
        const BigInt a = BigInt::from_string("1090").value();
        const BigInt b = BigInt::from_string("20").value();
        const BigInt r = a + b;
        helpers::expect_eq(r, "1110");
    }

    TEST(BigIntAdd, AddingZero) {
        const BigInt a = BigInt::from_string("123456").value();
        const BigInt zero = BigInt::from_string("0").value();

        helpers::expect_eq(a + zero, "123456");
        helpers::expect_eq(zero + a, "123456");
    }

    TEST(BigIntAdd, LeadingZerosAreIgnored) {
        const BigInt a = BigInt::from_string("000123").value();
        const BigInt b = BigInt::from_string("000007").value();
        const BigInt r = a + b;
        helpers::expect_eq(r, "130");
    }

    TEST(BigIntAdd, LargeNumbers) {
        const BigInt a = BigInt::from_string("123456789012345678901234567890").value();
        const BigInt b = BigInt::from_string("987654321098765432109876543210").value();
        const BigInt r = a + b;

        helpers::expect_eq(r, "1111111110111111111011111111100");
    }

    TEST(BigIntAdd, Commutativity) {
        const BigInt a = BigInt::from_string("918273645").value();
        const BigInt b = BigInt::from_string("192837465").value();

        const BigInt r1 = a + b;
        const BigInt r2 = b + a;

        helpers::expect_eq(r1, "1111111110");
        helpers::expect_eq(r2, "1111111110");
    }

    TEST(BigIntAdd, ManyCarries) {
        const BigInt a = BigInt::from_string("55555555555555555555").value();
        const BigInt b = BigInt::from_string("55555555555555555555").value();
        const BigInt r = a + b;

        helpers::expect_eq(r, "111111111111111111110");
    }

    TEST(BigIntAdd, RandomizedSmall) {
        for (int i = 0; i < 100; i++) {
            const int x = rand() % 10000;
            const int y = rand() % 10000;

            BigInt a = BigInt::from_string(std::to_string(x)).value();
            BigInt b = BigInt::from_string(std::to_string(y)).value();
            BigInt r = a + b;

            helpers::expect_eq(r, std::to_string(x + y));
        }
    }

    TEST(BigIntAdd, RandomizedLarge) {
        for (int i = 0; i < 20; i++) {
            std::string s1, s2;

            for (int d = 0; d < 100; d++)
                s1 += '0' + (rand() % 10);

            for (int d = 0; d < 100; d++)
                s2 += '0' + (rand() % 10);

            BigInt a = BigInt::from_string(s1).value();
            BigInt b = BigInt::from_string(s2).value();

            // compute expected using builtin big-int via std::string
            std::string expected = helpers::big_int_string_add(s1, s2);

            BigInt result = a + b;
            helpers::expect_eq(result, expected);
        }
    }
} // namespace arbys::bignumbers::tests::add
