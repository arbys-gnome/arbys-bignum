#include <gtest/gtest.h>

#include "../../include/arbys/bignum/big_int.h"
#include "../helpers/helpers.h"

namespace arbys::bignum::tests {

TEST(BigIntDivWithLongLongTest, SimpleLongLongDivision) {
    const big_int   a = big_int::from_string("1000").value();
    const long long d = 8;
    const big_int   r = a / d;
    EXPECT_BI_EQ(r, "125");
}

TEST(BigIntDivWithLongLongTest, LongLongNonExactDivision) {
    const big_int a = big_int::from_string("99").value();
    const big_int r = a / 4LL;
    EXPECT_BI_EQ(r, "24");
}

TEST(BigIntDivWithLongLongTest, DivideWithNegativeLL) {
    const big_int a = big_int::from_string("100").value();
    const big_int r = a/ -3LL;
    EXPECT_BI_EQ(r, "-33");
}

TEST(BigIntDivWithLongLongTest, DivisionByZeroLLThrows) {
    const big_int       lhs = big_int::from_string("100").value();
    constexpr long long rhs = 0LL;
    EXPECT_THROW(lhs / rhs, std::domain_error);
}

TEST(BigIntDivWithLongLongTest, VeryLargebig_intDivLL) {
    const std::string s = "999999999999999999999999999999999999";

    const big_int   a = big_int::from_string(s).value();
    const long long d = 1234567;

    const std::string expected = "810000591300431649315104000025";
    const big_int     r        = a / d;
    EXPECT_BI_EQ(r, expected);
}

TEST(BigIntDivTest, SmallExactDivision) {
    const big_int a = big_int::from_string("100").value();
    const big_int b = big_int::from_string("25").value();
    const big_int r = a / b;
    EXPECT_BI_EQ(r, "4");
}

TEST(BigIntDivTest, SmallNonExactDivision) {
    const big_int a = big_int::from_string("101").value();
    const big_int b = big_int::from_string("25").value();
    const big_int r = a / b;
    EXPECT_BI_EQ(r, "4"); // truncates
}

TEST(BigIntDivTest, LargerExactDivision) {
    const big_int a = big_int::from_string("987654321").value();
    const big_int b = big_int::from_string("3").value();
    const big_int r = a/ b;
    EXPECT_BI_EQ(r, "329218107");
}

TEST(BigIntDivTest, DivisionByLargerNumber) {
    const big_int a = big_int::from_string("12345").value();
    const big_int b = big_int::from_string("999999999").value();
    const big_int r = a / b;
    EXPECT_BI_EQ(r, "0");
}

TEST(BigIntDivTest, DivideBySelf) {
    const big_int a = big_int::from_string("123456").value();
    const big_int r = a / a;
    EXPECT_BI_EQ(r, "1");
}

TEST(BigIntDivTest, LongLargeNumbers) {
    const std::string s1 = "99999999999999999999999999";
    const std::string s2 = "123456789";

    const big_int a = big_int::from_string(s1).value();
    const big_int b = big_int::from_string(s2).value();

    const std::string expected = "810000007371000067"; // calculated online
    const big_int     r        = a / b;
    EXPECT_BI_EQ(r, expected);
}

TEST(BigIntDivTest, DivisionByZeroThrows) {
    const big_int a    = big_int::from_string("12345").value();
    const big_int zero = big_int::from_string("0").value();

    EXPECT_THROW(a / zero, std::domain_error);
}

TEST(BigIntDivTest, MultipleDivisions) {
    const big_int n = big_int::from_string("40000490494094049049049409").value();
    const big_int u = 3;
    const big_int v = 7;
    const big_int w = 21;
    const big_int a = n / u; // 13333496831364683016349803
    const std::string ea = "13333496831364683016349803";
    EXPECT_BI_EQ(a, ea);
    const big_int b = n / v; // 5714355784870578435578487
    const std::string eb = "5714355784870578435578487";
    EXPECT_BI_EQ(b, eb);
    const big_int c = n / w; // 1904785261623526145192829
    const std::string ec = "1904785261623526145192829";
    EXPECT_BI_EQ(c, ec);

}

} // namespace arbys::bignum::tests