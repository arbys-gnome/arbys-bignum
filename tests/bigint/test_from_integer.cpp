#include "arbys/bignumbers/big_int.h"

#include "../helpers/helpers.h"

#include <gtest/gtest.h>

#include <limits>

namespace arbys::bignumbers::tests {

TEST(FromIntegerTest, FromPositiveInt) {
    constexpr long long nr = std::numeric_limits<int>::max();
    const big_int       bi = big_int::from_integer(nr);
    EXPECT_BI_EQ(bi, std::to_string(nr));
}

TEST(FromIntegerTest, FromNegativeInt) {
    constexpr long long nr = std::numeric_limits<int>::min();
    const big_int       bi = big_int::from_integer(nr);
    EXPECT_BI_EQ(bi, std::to_string(nr));
}

TEST(FromIntegerTest, FromPositiveLong) {
    constexpr long long nr = std::numeric_limits<long>::max();
    const big_int       bi = big_int::from_integer(nr);
    EXPECT_BI_EQ(bi, std::to_string(nr));
}

TEST(FromIntegerTest, FromNegativeLong) {
    constexpr long long nr = std::numeric_limits<long>::min();
    const big_int       bi = big_int::from_integer(nr);
    EXPECT_BI_EQ(bi, std::to_string(nr));
}

TEST(FromIntegerTest, FromPositiveLongLong) {
    constexpr long long nr = std::numeric_limits<long long>::max();
    const big_int       bi = big_int::from_integer(nr);
    EXPECT_BI_EQ(bi, std::to_string(nr));
}

TEST(FromIntegerTest, FromNegativeLongLong) {
    constexpr long long nr = std::numeric_limits<long long>::min();
    const big_int       bi = big_int::from_integer(nr);
    EXPECT_BI_EQ(bi, std::to_string(nr));
}
} // namespace arbys::bignumbers::tests
