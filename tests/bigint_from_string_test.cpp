#include <gtest/gtest.h>

#include "bigint.h"
#include "helpers/helpers.h"

namespace arbys::bignumbers::tests {
    TEST(BigIntFromString, NoSeparatorDigitsOnly) {
        const std::string s = helpers::makeIncreasing(10);

        // Leading zeros removed
        helpers::expectBigIntEquals(BigInt::fromString("000" + s, ""), s);

        // Whitespace ignored
        helpers::expectBigIntEquals(BigInt::fromString("  \t " + s + "  \n ", ""), s);
    }

    TEST(BigIntFromString, NoSeparatorRejectsNonDigits) {
        EXPECT_THROW(BigInt::fromString("12a34", ""), std::invalid_argument);
        EXPECT_THROW(BigInt::fromString("12-34", ""), std::invalid_argument);
        EXPECT_THROW(BigInt::fromString("   ", ""), std::invalid_argument);
    }

    TEST(BigIntFromString, RegexSeparatorCommaSpace) {
        const std::string sep = " *, *";

        // Leading zeros in the first tokens are ignored
        const auto bn = BigInt::fromString("0001,02,003,004", sep);
        helpers::expectBigIntEquals(bn, "102003004");
    }

    TEST(BigIntFromString, SeparatorPreservesInternalZeros) {
        const std::string sep = "[,; ]+";

        // Internal zeros in tokens preserved, first token leading zeros removed
        const auto bn = BigInt::fromString("0001,0020,0030", sep);
        helpers::expectBigIntEquals(bn, "100200030");
    }

    TEST(BigIntFromString, SeparatorRejectsInvalidTokens) {
        const std::string sep = ",";

        EXPECT_THROW(BigInt::fromString("1,a,2", sep), std::invalid_argument);
        EXPECT_THROW(BigInt::fromString(",,", sep), std::invalid_argument);
        EXPECT_THROW(BigInt::fromString("a,b,c", sep), std::invalid_argument);
    }

    TEST(BigIntFromString, LeadingZerosOnlyTokensIgnored) {
        const std::string sep = ",";

        // Only the first token's leading zeros are removed, completely zero tokens ignored
        const auto bn = BigInt::fromString("0000,0001,0020,0003", sep);
        helpers::expectBigIntEquals(bn, "100200003");
    }

    TEST(BigIntFromString, LargeNumberWithSpaces) {
        const std::string sep = " +";
        const std::string s = helpers::makeIncreasing(50);

        std::string spaced;
        for (const char c : s) spaced += std::string("  ") + c + " ";

        const auto bn = BigInt::fromString(spaced, sep);
        helpers::expectBigIntEquals(bn, s);
    }

    TEST(BigIntFromString, ZeroValue) {
        EXPECT_NO_THROW(BigInt::fromString("0000", ","));
        EXPECT_NO_THROW(BigInt::fromString("    0000   ", ","));
    }
} // namespace arbys::bignumbers::tests::from_string