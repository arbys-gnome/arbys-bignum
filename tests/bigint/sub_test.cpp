#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/bigint.h"
#include "../helpers/helpers.h"

namespace arbys::bignumbers::tests::sub {
    TEST(BigIntSub, SimpleNoBorrow) {
        const BigInt a = BigInt::from_string("123").value();
        const BigInt b = BigInt::from_string("11").value();
        const BigInt r = a - b;
        helpers::expect_eq(r, "112");
    }

    TEST(BigIntSub, SimpleBorrow) {
        const BigInt a = BigInt::from_string("223").value();
        const BigInt b = BigInt::from_string("104").value();
        const BigInt r = a - b;
        helpers::expect_eq(r, "119");
    }

    TEST(BigNumbrSub, SimpleLongCarry) {
        const BigInt a = BigInt::from_string("20003").value();
        const BigInt b = BigInt::from_string("1004").value();
        const BigInt r = a - b;
        helpers::expect_eq(r, "18999");
    }

    TEST(BigIntSub, SubToZero) {
        const BigInt a = BigInt::from_string("0").value();
        const BigInt b = BigInt::from_string("0").value();
        const BigInt r = a - b;
        helpers::expect_eq(r, "0");

        const BigInt c = BigInt::from_string("100000001122344").value();
        const BigInt d = BigInt::from_string("100000001122344").value();
        const BigInt r2 = a - b;
        helpers::expect_eq(r2, "0");
    }
} // namespace arbys::bignumbers::tests::sub
