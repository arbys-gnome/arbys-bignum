#pragma once

#include <string>
#include <gtest/gtest.h>

#define EXPECT_BI_EQ(a, b) \
EXPECT_PRED_FORMAT2(::arbys::bignum::tests::helpers::big_int_eq, a, b)

namespace arbys::bignum {
    class big_int;
}

namespace arbys::bignum::tests::helpers {
    ::testing::AssertionResult big_int_eq(
        const char *expr1,
        const char *expr2,
        const big_int &bn1,
        const big_int &bn2
    );

    ::testing::AssertionResult big_int_eq(
        const char *expr1,
        const char *expr2,
        const big_int &bn,
        const std::string &str
    );

    std::string big_int_string_div(const std::string &a, const std::string &b);
    void expect_compare_eq(const big_int& a, const big_int& b, std::strong_ordering expected);
    std::string make_increasing(size_t len);
    void expect_eq(const big_int &bn, std::string s);
    void expect_eq(const big_int &bn1, const big_int &bn2);
    std::string big_int_string_add(const std::string& a, const std::string& b);


    template<typename T, typename E>
    void expect_ok(const std::expected<T, E>& exp) {
        EXPECT_TRUE(exp.has_value());
    }

    template<typename T, typename E>
    void expect_err(const std::expected<T, E>& exp, E expected_error) {
        ASSERT_FALSE(exp.has_value());
        EXPECT_EQ(exp.error(), expected_error);
    }

} // namespace arbys::bignum::tests::helpers