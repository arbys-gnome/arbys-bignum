#ifndef BIGNUMBERS_HELPERS_H
#define BIGNUMBERS_HELPERS_H

#include <string>

namespace arbys::bignumbers {
    class BigInt;
}

namespace arbys::bignumbers::tests::helpers {
    std::string big_int_string_div(const std::string &a, const std::string &b);
    void expect_compare_eq(const BigInt& a, const BigInt& b, std::strong_ordering expected);
    std::string make_increasing(size_t len);
    void expect_eq(const BigInt &bn, std::string s);
    void expect_eq(const BigInt &bn, const BigInt &other);
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

} // namespace arbys::bignumbers::tests::helpers

#endif //BIGNUMBERS_HELPERS_H
