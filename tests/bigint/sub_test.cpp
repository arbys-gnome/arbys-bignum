#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/big_int.h"
#include "../helpers/helpers.h"

namespace arbys::bignumbers::tests::sub {
    TEST(BigIntSub, SimpleNoBorrow) {
        const big_int a = big_int::from_string("123").value();
        const big_int b = big_int::from_string("11").value();
        const big_int r = a - b;
        helpers::expect_eq(r, "112");
    }

    TEST(BigIntSub, SimpleBorrow) {
        const big_int a = big_int::from_string("223").value();
        const big_int b = big_int::from_string("104").value();
        const big_int r = a - b;
        helpers::expect_eq(r, "119");
    }

    TEST(BigNumbrSub, SimpleLongCarry) {
        const big_int a = big_int::from_string("20003").value();
        const big_int b = big_int::from_string("1004").value();
        const big_int r = a - b;
        helpers::expect_eq(r, "18999");
    }

    TEST(BigIntSub, SubToZero) {
        const big_int a = big_int::from_string("0").value();
        const big_int b = big_int::from_string("0").value();
        const big_int r = a - b;
        helpers::expect_eq(r, "0");

        const big_int c = big_int::from_string("100000001122344").value();
        const big_int d = big_int::from_string("100000001122344").value();
        const big_int r2 = a - b;
        helpers::expect_eq(r2, "0");
    }
} // namespace arbys::bignumbers::tests::sub
