#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/big_int.h"
#include "../helpers/helpers.h"

namespace arbys::bignumbers::tests {
//     TEST(BigIntDivWithLongLongTest, SimpleLongLongDivision) {
//         const BigInt a = BigInt::from_string("1000").value();
//         const long long d = 8;
//         const BigInt r = a.div(d);
//         helpers::expect_eq(r, "125");
//     }
//
//     TEST(BigIntDivWithLongLongTest, LongLongNonExactDivision) {
//         BigInt a = BigInt::from_string("99").value();
//         BigInt r = a.div(4LL);
//         helpers::expect_eq(r, "24");
//     }
//
//     TEST(BigIntDivWithLongLongTest, DivideWithNegativeLL) {
//         BigInt a = BigInt::from_string("100").value();
//         BigInt r = a.div(-3LL);
//         helpers::expect_eq(r, "-33");  // if BigInt supports signed values
//     }
//
//     TEST(BigIntDivWithLongLongTest, DivisionByZeroLLThrows) {
//         const BigInt lhs = BigInt::from_string("100").value();
//         constexpr long long rhs = 0LL;
//         EXPECT_THROW(lhs / rhs, std::invalid_argument);
//     }
//
//     TEST(BigIntDivWithLongLongTest, VeryLargeBigIntDivLL) {
//         std::string s = "999999999999999999999999999999999999";
//
//         BigInt a = BigInt::from_string(s).value();
//         long long d = 1234567;
//
//         std::string expected = helpers::big_int_string_div(s, "1234567");
//         BigInt r = a.div(d);
//         helpers::expect_eq(r, expected);
//     }
//
//
//     TEST(BigIntDivTest, SmallExactDivision) {
//     BigInt a = BigInt::fromString("100");
//     BigInt b = BigInt::fromString("25");
//     BigInt r = a.div(b);
//     helpers::expectBigIntEquals(r, "4");
// }
//
// TEST(BigIntDivTest, SmallNonExactDivision) {
//     BigInt a = BigInt::fromString("101");
//     BigInt b = BigInt::fromString("25");
//     BigInt r = a.div(b);
//     helpers::expectBigIntEquals(r, "4");  // truncates
// }
//
// TEST(BigIntDivTest, LargerExactDivision) {
//     BigInt a = BigInt::fromString("987654321");
//     BigInt b = BigInt::fromString("3");
//     BigInt r = a.div(b);
//     helpers::expectBigIntEquals(r, "329218107");
// }
//
// TEST(BigIntDivTest, DivisionByLargerNumber) {
//     BigInt a = BigInt::fromString("12345");
//     BigInt b = BigInt::fromString("999999999");
//     BigInt r = a.div(b);
//     helpers::expectBigIntEquals(r, "0");
// }
//
// TEST(BigIntDivTest, DivideBySelf) {
//     BigInt a = BigInt::fromString("123456");
//     BigInt r = a.div(a);
//     helpers::expectBigIntEquals(r, "1");
// }
//
// TEST(BigIntDivTest, LongLargeNumbers) {
//     std::string s1 = "99999999999999999999999999";
//     std::string s2 = "123456789";
//
//     BigInt a = BigInt::fromString(s1);
//     BigInt b = BigInt::fromString(s2);
//
//     std::string expected = helpers::bigIntegerStringDiv(s1, s2);
//     BigInt r = a.div(b);
//     helpers::expectBigIntEquals(r, expected);
// }
//
// TEST(BigIntDivTest, DivisionByZeroThrows) {
//     BigInt a = BigInt::fromString("12345");
//     BigInt zero = BigInt::fromString("0");
//
//     EXPECT_THROW(a.div(zero), std::invalid_argument);
// }
}