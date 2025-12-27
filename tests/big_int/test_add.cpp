#include <gtest/gtest.h>

#include "../../include/arbys/bignum/big_int.h"
#include "../helpers/helpers.h"

namespace arbys::bignum::tests {
TEST(BigIntAdd, SimpleNoCarry) {
    const big_int a = 123;
    const big_int b = 321;
    const big_int r = a + b;
    EXPECT_BI_EQ(r, "444");
}

TEST(BigIntAdd, SimpleCarry) {
    const big_int a = 19;
    const big_int b = 34;
    const big_int r = a + b;
    EXPECT_BI_EQ(r, "53");
}

TEST(BigIntAdd, CarryAcrossMultipleDigits) {
    const big_int a = 999;
    const big_int b = 1;
    const big_int r = a + b;
    EXPECT_BI_EQ(r, "1000");
}

TEST(BigIntAdd, DifferentLengthsNoCarry) {
    const big_int a = 5000;
    const big_int b = 20;
    const big_int r = a + b;
    EXPECT_BI_EQ(r, "5020");
}

TEST(BigIntAdd, DifferentLengthsWithCarry) {
    const big_int a = 1090;
    const big_int b = 20;
    const big_int r = a + b;
    EXPECT_BI_EQ(r, "1110");
}

TEST(BigIntAdd, AddingZero) {
    const big_int a    = 123456;
    const big_int zero = 0;

    EXPECT_BI_EQ(a + zero, "123456");
    EXPECT_BI_EQ(zero + a, "123456");
}

TEST(BigIntAdd, LargeNumbers) {
    const big_int a = big_int::from_string("123456789012345678901234567890").value();
    const big_int b = big_int::from_string("987654321098765432109876543210").value();
    const big_int r = a + b;

    EXPECT_BI_EQ(r, "1111111110111111111011111111100");
}

TEST(BigIntAdd, Commutativity) {
    const big_int a = 918273645;
    const big_int b = 192837465;

    const big_int r1 = a + b;
    const big_int r2 = b + a;

    EXPECT_BI_EQ(r1, "1111111110");
    EXPECT_BI_EQ(r2, "1111111110");
}

TEST(BigIntAdd, ManyCarries) {
    const big_int a = big_int::from_string("55555555555555555555").value();
    const big_int b = big_int::from_string("55555555555555555555").value();
    const big_int r = a + b;

    EXPECT_BI_EQ(r, "111111111111111111110");
}

TEST(BigIntAdd, RandomizedSmall) {
    for (int i = 0; i < 100; i++) {
        // TODO: improve random function
        const int x = rand() % 10000;
        const int y = rand() % 10000;

        big_int a = x;
        big_int b = y;
        big_int r = a + b;

        EXPECT_BI_EQ(r, std::to_string(x + y));
    }
}

TEST(BigIntAdd, RandomizedLarge) {
    for (int i = 0; i < 20; i++) {
        std::string s1, s2;

        for (int d = 0; d < 100; d++)
            s1 += '0' + (rand() % 10);

        for (int d = 0; d < 100; d++)
            s2 += '0' + (rand() % 10);

        big_int a = big_int::from_string(s1).value();
        big_int b = big_int::from_string(s2).value();

        // compute expected using builtin big-int via std::string
        const std::string expected = helpers::big_int_string_add(s1, s2);

        big_int result = a + b;
        EXPECT_BI_EQ(result, expected);
    }
}

} // namespace arbys::bignumbers::tests
