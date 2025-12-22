#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/bigint.h"
#include "../helpers/helpers.h"

namespace arbys::bignumbers::tests {
    TEST(BigIntCompare, EqualNumbers) {
        helpers::expect_compare_eq(BigInt::from_string("0").value(), BigInt::from_string("0").value(), std::strong_ordering::equal);
        helpers::expect_compare_eq(BigInt::from_string("123").value(), BigInt::from_string("123").value(), std::strong_ordering::equal);
        helpers::expect_compare_eq(BigInt::from_string("-999").value(), BigInt::from_string("-999").value(), std::strong_ordering::equal);
    }

    TEST(BigIntCompare, PositiveVsPositive) {
        helpers::expect_compare_eq(BigInt::from_string("1").value(), BigInt::from_string("2").value(), std::strong_ordering::less);
        helpers::expect_compare_eq(BigInt::from_string("10").value(), BigInt::from_string("2").value(), std::strong_ordering::greater);
        helpers::expect_compare_eq(BigInt::from_string("999").value(), BigInt::from_string("1000").value(), std::strong_ordering::less);
    }

    TEST(BigIntCompare, NegativeVsNegative) {
        helpers::expect_compare_eq(BigInt::from_string("-1").value(), BigInt::from_string("-2").value(), std::strong_ordering::greater);
        helpers::expect_compare_eq(BigInt::from_string("-10").value(), BigInt::from_string("-2").value(), std::strong_ordering::less);
        helpers::expect_compare_eq(BigInt::from_string("-1000").value(), BigInt::from_string("-999").value(), std::strong_ordering::less);
    }

    TEST(BigIntCompare, PositiveVsNegative) {
        helpers::expect_compare_eq(BigInt::from_string("1").value(), BigInt::from_string("-1").value(), std::strong_ordering::greater);
        helpers::expect_compare_eq(BigInt::from_string("0").value(), BigInt::from_string("-1").value(), std::strong_ordering::greater);
        helpers::expect_compare_eq(BigInt::from_string("123").value(), BigInt::from_string("-999").value(), std::strong_ordering::greater);
    }

    TEST(BigIntCompare, NegativeVsPositive) {
        helpers::expect_compare_eq(BigInt::from_string("-1").value(), BigInt::from_string("1").value(), std::strong_ordering::less);
        helpers::expect_compare_eq(BigInt::from_string("-100").value(), BigInt::from_string("0").value(), std::strong_ordering::less);
    }

    TEST(BigIntCompare, SameAbsoluteDifferentSign) {
        helpers::expect_compare_eq(BigInt::from_string("5").value(), BigInt::from_string("-5").value(), std::strong_ordering::greater);
        helpers::expect_compare_eq(BigInt::from_string("-5").value(), BigInt::from_string("5").value(), std::strong_ordering::less);
    }

    TEST(BigIntCompare, LargeNumbers) {
        helpers::expect_compare_eq(
            BigInt::from_string("123456789123456789123456789").value(),
            BigInt::from_string("123456789123456789123456788").value(),
            std::strong_ordering::greater
        );

        helpers::expect_compare_eq(
            BigInt::from_string("-999999999999999999").value(),
            BigInt::from_string("-1000000000000000000").value(),
            std::strong_ordering::greater
        );
    }

    TEST(BigIntCompare, ZeroHandling) {
        helpers::expect_compare_eq(BigInt::from_string("0").value(), BigInt::from_string("-0").value(), std::strong_ordering::equal);
        helpers::expect_compare_eq(BigInt::from_string("0").value(), BigInt::from_string("1").value(), std::strong_ordering::less);
        helpers::expect_compare_eq(BigInt::from_string("-1").value(), BigInt::from_string("0").value(), std::strong_ordering::less);
    }
} // namespace arbys::bignumbers::tests
