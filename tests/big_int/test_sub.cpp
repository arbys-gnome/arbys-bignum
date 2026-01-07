#include <gtest/gtest.h>

#include "../../include/arbys/bignum/big_int.h"
#include "../helpers/helpers.h"

namespace arbys::bignum::tests {

TEST(BigIntSubTest, SimpleNoBorrow) {
    const big_int a = 123;
    const big_int b = 11;
    const big_int r = a - b;
    EXPECT_BI_EQ(r, "112");
}

TEST(BigIntSubTest, SimpleBorrow) {
    const big_int a = 223;
    const big_int b = 104;
    const big_int r = a - b;
    EXPECT_BI_EQ(r, "119");
}

TEST(BigIntSubTest, SimpleLongCarry) {
    const big_int a = 20003;
    const big_int b = 1004;
    const big_int r = a - b;
    EXPECT_BI_EQ(r, "18999");
}

TEST(BigIntSubTest, SubToZero) {
    const big_int a = 0;
    const big_int b = 0;
    const big_int r = a - b;
    EXPECT_BI_EQ(r, "0");

    const big_int c  = big_int::from_string("100000001122344").value();
    const big_int d  = big_int::from_string("100000001122344").value();
    const big_int r2 = a - b;
    EXPECT_BI_EQ(r2, "0");
}

TEST(BigIntSubTest, MultipleSubs) {
    const big_int a = big_int::from_string("40000490494094049049049409").value();
    const big_int b = big_int::from_string("5714355784870578435578487").value();
    const big_int c = big_int::from_string("1904785261623526145192829").value();
    const big_int res = a - b - c;
    EXPECT_BI_EQ(res, "32381349447599944468278093");
}

} // namespace arbys::bignumbers::tests::sub
