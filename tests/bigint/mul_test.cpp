#include <gtest/gtest.h>
#include "arbys/bignumbers/bigint.h"
#include "../helpers/helpers.h"


namespace arbys::bignumbers::tests {

    class BigIntMulTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

    TEST_F(BigIntMulTest, MultiplyByZero) {
        const auto a = BigInt::from_string("12345").value();
        const auto b = BigInt::from_string("0").value();
        const auto result = a * b;

        helpers::expect_eq(result, "0");
    }

    TEST_F(BigIntMulTest, ZeroMultiplyByNumber) {
        const auto a = BigInt::from_string("0").value();
        const auto b = BigInt::from_string("98765").value();
        const auto result = a * b;

        helpers::expect_eq(result, "0");
    }

    TEST_F(BigIntMulTest, ZeroMultiplyByZero) {
        const auto a = BigInt::from_string("0").value();
        const auto b = BigInt::from_string("0").value();
        const auto result = a * b;

        helpers::expect_eq(result, "0");
    }

    TEST_F(BigIntMulTest, MultiplyByOne) {
        const auto a = BigInt::from_string("12345").value();
        const auto b = BigInt::from_string("1").value();
        const auto result = a * b;

        helpers::expect_eq(result, "12345");
    }

    TEST_F(BigIntMulTest, OneMultiplyByNumber) {
        const auto a = BigInt::from_string("1").value();
        const auto b = BigInt::from_string("98765").value();
        const auto result = a * b;

        helpers::expect_eq(result, "98765");
    }

    TEST_F(BigIntMulTest, SmallPositiveNumbers) {
        const auto a = BigInt::from_string("123").value();
        const auto b = BigInt::from_string("456").value();
        const auto result = a * b;

        helpers::expect_eq(result, "56088");
    }

    TEST_F(BigIntMulTest, TwoDigitMultiplication) {
        const auto a = BigInt::from_string("99").value();
        const auto b = BigInt::from_string("99").value();
        const auto result = a * b;

        helpers::expect_eq(result, "9801");
    }

    TEST_F(BigIntMulTest, SingleDigitMultiplication) {
        const auto a = BigInt::from_string("7").value();
        const auto b = BigInt::from_string("8").value();
        const auto result = a * b;

        helpers::expect_eq(result, "56");
    }

    TEST_F(BigIntMulTest, PositiveTimesNegative) {
        const auto a = BigInt::from_string("123").value();
        const auto b = BigInt::from_string("-456").value();
        const auto result = a * b;

        helpers::expect_eq(result, "-56088");
    }

    TEST_F(BigIntMulTest, NegativeTimesPositive) {
        const auto a = BigInt::from_string("-123").value();
        const auto b = BigInt::from_string("456").value();
        const auto result = a * b;

        helpers::expect_eq(result, "-56088");
    }

    TEST_F(BigIntMulTest, NegativeTimesNegative) {
        const auto a = BigInt::from_string("-123").value();
        const auto b = BigInt::from_string("-456").value();
        const auto result = a * b;

        helpers::expect_eq(result, "56088");
    }

    TEST_F(BigIntMulTest, NegativeByZero) {
        const auto a = BigInt::from_string("-123").value();
        const auto b = BigInt::from_string("0").value();
        const auto result = a * b;

        helpers::expect_eq(result, "0");
    }

    TEST_F(BigIntMulTest, NegativeByOne) {
        const auto a = BigInt::from_string("-12345").value();
        const auto b = BigInt::from_string("1").value();
        const auto result = a * b;

        helpers::expect_eq(result, "-12345");
    }

    TEST_F(BigIntMulTest, NegativeByNegativeOne) {
        const auto a = BigInt::from_string("-12345").value();
        const auto b = BigInt::from_string("-1").value();
        const auto result = a * b;

        helpers::expect_eq(result, "12345");
    }

    TEST_F(BigIntMulTest, MediumSizeNumbers) {
        const auto a = BigInt::from_string("123456789").value();
        const auto b = BigInt::from_string("987654321").value();
        const auto result = a * b;

        helpers::expect_eq(result, "121932631112635269");
    }

    TEST_F(BigIntMulTest, PowersOfTen) {
        const auto a = BigInt::from_string("1000").value();
        const auto b = BigInt::from_string("1000").value();
        const auto result = a * b;

        helpers::expect_eq(result, "1000000");
    }

    TEST_F(BigIntMulTest, PowersOfTenLarge) {
        const auto a = BigInt::from_string("1000000").value();
        const auto b = BigInt::from_string("1000000").value();
        const auto result = a * b;

        helpers::expect_eq(result, "1000000000000");
    }

    TEST_F(BigIntMulTest, LargeNumbers) {
        const auto a = BigInt::from_string("12345678901234567890").value();
        const auto b = BigInt::from_string("98765432109876543210").value();
        const auto result = a * b;

        helpers::expect_eq(result, "1219326311370217952237463801111263526900");
    }

    TEST_F(BigIntMulTest, VeryLargeNumbers) {
        const auto a = BigInt::from_string("123456789012345678901234567890").value();
        const auto b = BigInt::from_string("987654321098765432109876543210").value();
        const auto result = a * b;

        helpers::expect_eq(result,
                  "121932631137021795226185032733622923332237463801111263526900");
    }

    TEST_F(BigIntMulTest, HugeNumbers) {
        const std::string num1(100, '9');  // 100 nines
        const std::string num2(100, '9');  // 100 nines

        const auto a = BigInt::from_string(num1).value();
        const auto b = BigInt::from_string(num2).value();
        const auto result = a * b;

        // 999...999 (100 digits) * 999...999 (100 digits)
        // = 999...998000...001 (200 digits)
        std::string expected(99, '9');
        expected += "8";
        expected += std::string(99, '0');
        expected += "1";

        helpers::expect_eq(result, expected);
    }

    TEST_F(BigIntMulTest, DifferentSizesSmallByLarge) {
        const auto a = BigInt::from_string("5").value();
        const auto b = BigInt::from_string("123456789012345678901234567890").value();
        const auto result = a * b;

        helpers::expect_eq(result, "617283945061728394506172839450");
    }

    TEST_F(BigIntMulTest, DifferentSizesLargeBySmall) {
        const auto a = BigInt::from_string("123456789012345678901234567890").value();
        const auto b = BigInt::from_string("5").value();
        const auto result = a * b;

        helpers::expect_eq(result, "617283945061728394506172839450");
    }

    TEST_F(BigIntMulTest, DifferentSizesMedium) {
        const auto a = BigInt::from_string("123456").value();
        const auto b = BigInt::from_string("7890123456789").value();
        const auto result = a * b;

        helpers::expect_eq(result, "974083081481342784");
    }

    TEST_F(BigIntMulTest, SameNumberMultipliedByItself) {
        const auto a = BigInt::from_string("123456789").value();
        const auto result = a * a;

        helpers::expect_eq(result, "15241578750190521");
    }

    TEST_F(BigIntMulTest, NegativeSameNumber) {
        const auto a = BigInt::from_string("-999").value();
        const auto result = a * a;

        helpers::expect_eq(result, "998001");
    }

    TEST_F(BigIntMulTest, TrailingZeros) {
        const auto a = BigInt::from_string("12000").value();
        const auto b = BigInt::from_string("34000").value();
        const auto result = a * b;

        helpers::expect_eq(result, "408000000");
    }

    TEST_F(BigIntMulTest, LeadingAndTrailingZeros) {
        const auto a = BigInt::from_string("1000").value();
        const auto b = BigInt::from_string("2000").value();
        const auto result = a * b;

        helpers::expect_eq(result, "2000000");
    }

    TEST_F(BigIntMulTest, CommutativitySmall) {
        const auto a = BigInt::from_string("123").value();
        const auto b = BigInt::from_string("456").value();

        const auto result1 = a * b;
        const auto result2 = b * a;

        helpers::expect_eq(result1, result2);
    }

    TEST_F(BigIntMulTest, CommutativityLarge) {
        const auto a = BigInt::from_string("123456789012345").value();
        const auto b = BigInt::from_string("987654321098765").value();

        const auto result1 = a * b;
        const auto result2 = b * a;

        helpers::expect_eq(result1, result2);
    }

    TEST_F(BigIntMulTest, CommutativityWithNegatives) {
        const auto a = BigInt::from_string("-123456").value();
        const auto b = BigInt::from_string("789012").value();

        const auto result1 = a * b;
        const auto result2 = b * a;

        helpers::expect_eq(result1, result2);
    }

    TEST_F(BigIntMulTest, Associativity) {
        const auto a = BigInt::from_string("123").value();
        const auto b = BigInt::from_string("456").value();
        const auto c = BigInt::from_string("789").value();

        const auto result1 = (a * b) * c;
        const auto result2 = a * (b * c);

        helpers::expect_eq(result1, result2);
    }

    TEST_F(BigIntMulTest, OperatorOverload) {
        const auto a = BigInt::from_string("123").value();
        const auto b = BigInt::from_string("456").value();

        const auto result_method = a.mul(b);
        const auto result_operator = a * b;

        helpers::expect_eq(result_method, result_operator);
    }

    TEST_F(BigIntMulTest, SmallFactorial) {
        // 5! = 120
        auto result = BigInt::from_string("1").value();
        for (int i = 2; i <= 5; ++i) {
            const auto factor = BigInt::from_string(std::to_string(i)).value();
            result = result * factor;
        }

        helpers::expect_eq(result, "120");
    }

    TEST_F(BigIntMulTest, MediumFactorial) {
        // 10! = 3628800
        auto result = BigInt::from_string("1").value();
        for (int i = 2; i <= 10; ++i) {
            const auto factor = BigInt::from_string(std::to_string(i)).value();
            result = result * factor;
        }

        helpers::expect_eq(result, "3628800");
    }

    TEST_F(BigIntMulTest, LargeFactorial) {
        // 20! = 2432902008176640000
        auto result = BigInt::from_string("1").value();
        for (int i = 2; i <= 20; ++i) {
            const auto factor = BigInt::from_string(std::to_string(i)).value();
            result = result * factor;
        }

        helpers::expect_eq(result, "2432902008176640000");
    }

    TEST_F(BigIntMulTest, PowersOfTwo) {
        const auto two = BigInt::from_string("2").value();
        auto result = two;

        // 2^10 = 1024
        for (int i = 1; i < 10; ++i) {
            result = result * two;
        }

        helpers::expect_eq(result, "1024");
    }

    TEST_F(BigIntMulTest, LargePowerOfTwo) {
        const auto two = BigInt::from_string("2").value();
        auto result = two;

        // 2^20 = 1048576
        for (int i = 1; i < 20; ++i) {
            result = result * two;
        }

        helpers::expect_eq(result, "1048576");
    }

}
