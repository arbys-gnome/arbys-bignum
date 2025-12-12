#include <gtest/gtest.h>

#include "bigint.h"
#include "helpers/helpers.h"

namespace arbys::bignumbers::tests {
    TEST(BigIntCompare, EqualNumbers) {
        const BigInt a = BigInt::fromString("123456");
        const BigInt b = BigInt::fromString("123456");
        helpers::expectCompare(a, b, std::strong_ordering::equal);
    }

    TEST(BigIntCompare, DifferentLengthsShorterIsSmaller) {
        const BigInt a = BigInt::fromString("999");
        const BigInt b = BigInt::fromString("1000");
        helpers::expectCompare(a, b, std::strong_ordering::less);
        helpers::expectCompare(b, a, std::strong_ordering::greater);
    }

    TEST(BigIntCompare, SameLengthDifferentDigits) {
        const BigInt a = BigInt::fromString("12345");
        const BigInt b = BigInt::fromString("12346");
        helpers::expectCompare(a, b, std::strong_ordering::less);
        helpers::expectCompare(b, a, std::strong_ordering::greater);
    }

    TEST(BigIntCompare, LeadingZerosAreIgnored) {
        const BigInt a = BigInt::fromString("0000123");
        const BigInt b = BigInt::fromString("123");
        helpers::expectCompare(a, b, std::strong_ordering::equal);
    }

    TEST(BigIntCompare, LeadingZerosWithDifferentValues) {
        const BigInt a = BigInt::fromString("0000124");
        const BigInt b = BigInt::fromString("123");
        helpers::expectCompare(a, b, std::strong_ordering::greater);
    }

    TEST(BigIntCompare, VeryLargeNumbers) {
        const BigInt a = BigInt::fromString("999999999999999999999999999999");
        const BigInt b = BigInt::fromString("1000000000000000000000000000000");
        helpers::expectCompare(a, b, std::strong_ordering::less);
        helpers::expectCompare(b, a, std::strong_ordering::greater);
    }

    TEST(BigIntCompare, CompareSingleDigit) {
        const BigInt a = BigInt::fromString("8");
        const BigInt b = BigInt::fromString("3");
        helpers::expectCompare(a, b, std::strong_ordering::greater);
    }

    TEST(BigIntCompare, CompareZeroes) {
        const BigInt a = BigInt::fromString("0");
        const BigInt b = BigInt::fromString("00000");
        helpers::expectCompare(a, b, std::strong_ordering::equal);
    }

    TEST(BigIntCompare, CompareMixed) {
        const BigInt a = BigInt::fromString("502030");
        const BigInt b = BigInt::fromString("502029");
        helpers::expectCompare(a, b, std::strong_ordering::greater);
    }
} // namespace arbys::bignumbers::tests
