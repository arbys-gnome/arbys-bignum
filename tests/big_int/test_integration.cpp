#include "arbys/bignum/big_int.h"
#include "../helpers/helpers.h"

#include <gtest/gtest.h>

namespace arbys::bignum::tests {

TEST(BigIntIntegrationTest, ChainedOperations) {
    const big_int n = big_int::from_string("40000490494094049049049409").value();
    const big_int u = 3;
    const big_int v = 7;
    EXPECT_BI_EQ(n - n / u - n / v + n / (u * v), "22857423139482313742313948");
}

}