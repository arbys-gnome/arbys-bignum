#include <gtest/gtest.h>

#include "bignumber.h"
#include "helpers/helpers.h"

namespace arbys::bignumbers::tests::from_string {
    TEST(BigNumberFromString, NoSeparatorDigitsOnly) {
        const std::string s = helpers::makeIncreasing(10);

        // Leading zeros removed
        helpers::expectBigNumberEquals(BigNumber::fromString("000" + s, ""), s);

        // Whitespace ignored
        helpers::expectBigNumberEquals(BigNumber::fromString("  \t " + s + "  \n ", ""), s);
    }

    TEST(BigNumberFromString, NoSeparatorRejectsNonDigits) {
        EXPECT_THROW(BigNumber::fromString("12a34", ""), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString("12-34", ""), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString("   ", ""), std::invalid_argument);
    }

    TEST(BigNumberFromString, RegexSeparatorCommaSpace) {
        const std::string sep = " *, *";

        // Leading zeros in the first tokens are ignored
        const auto bn = BigNumber::fromString("0001,02,003,004", sep);
        helpers::expectBigNumberEquals(bn, "102003004");
    }

    TEST(BigNumberFromString, SeparatorPreservesInternalZeros) {
        const std::string sep = "[,; ]+";

        // Internal zeros in tokens preserved, first token leading zeros removed
        const auto bn = BigNumber::fromString("0001,0020,0030", sep);
        helpers::expectBigNumberEquals(bn, "100200030");
    }

    TEST(BigNumberFromString, SeparatorRejectsInvalidTokens) {
        const std::string sep = ",";

        EXPECT_THROW(BigNumber::fromString("1,a,2", sep), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString(",,", sep), std::invalid_argument);
        EXPECT_THROW(BigNumber::fromString("a,b,c", sep), std::invalid_argument);
    }

    TEST(BigNumberFromString, LeadingZerosOnlyTokensIgnored) {
        const std::string sep = ",";

        // Only the first token's leading zeros are removed, completely zero tokens ignored
        const auto bn = BigNumber::fromString("0000,0001,0020,0003", sep);
        helpers::expectBigNumberEquals(bn, "100200003");
    }

    TEST(BigNumberFromString, LargeNumberWithSpaces) {
        const std::string sep = " +";
        const std::string s = helpers::makeIncreasing(50);

        std::string spaced;
        for (const char c : s) spaced += std::string("  ") + c + " ";

        const auto bn = BigNumber::fromString(spaced, sep);
        helpers::expectBigNumberEquals(bn, s);
    }

    TEST(BigNumberFromString, ZeroValue) {
        EXPECT_NO_THROW(BigNumber::fromString("0000", ","));
        EXPECT_NO_THROW(BigNumber::fromString("    0000   ", ","));
    }
} // namespace arbys::bignumbers::tests::from_string