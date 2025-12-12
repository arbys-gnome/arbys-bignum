#include <gtest/gtest.h>

#include "bigint.h"
#include "helpers/helpers.h"

namespace arbys::bignumbers::tests::sub {
    TEST(BigIntSub, SimpleNoBorrow) {
        const BigInt a = BigInt::fromString("123");
        const BigInt b = BigInt::fromString("11");
        const BigInt r = a - b;
        helpers::expectBigIntEquals(r, "112");
    }

    TEST(BigIntSub, SimpleBorrow) {
        const BigInt a = BigInt::fromString("223");
        const BigInt b = BigInt::fromString("104");
        const BigInt r = a - b;
        helpers::expectBigIntEquals(r, "119");
    }

    TEST(BigNumbrSub, SimpleLongCarry) {
        const BigInt a = BigInt::fromString("20003");
        const BigInt b = BigInt::fromString("1004");
        const BigInt r = a - b;
        helpers::expectBigIntEquals(r, "18999");
    }

    TEST(BigIntSub, SubToZero) {
        const BigInt a = BigInt::fromString("0");
        const BigInt b = BigInt::fromString("0");
        const BigInt r = a - b;
        helpers::expectBigIntEquals(r, "0");

        const BigInt c = BigInt::fromString("100000001122344");
        const BigInt d = BigInt::fromString("100000001122344");
        const BigInt r2 = a - b;
        helpers::expectBigIntEquals(r2, "0");
    }
} // namespace arbys::bignumbers::tests::sub
