#include <gtest/gtest.h>

#include "../../include/arbys/bignumbers/bigint.h"
#include "../helpers/helpers.h"

namespace arbys::bignumbers::tests {

TEST(BigIntFromString, NoSeparatorDigitsOnly) {
    const std::string s = helpers::make_increasing(10);
    {
        const auto res = BigInt::from_string("000" + s);
        helpers::expect_ok(res);
        helpers::expect_eq(res.value(), s);
    }

    {
        const auto res = BigInt::from_string("  \t " + s + "  \n ");
        helpers::expect_ok(res);
        helpers::expect_eq(res.value(), s);
    }
}

TEST(BigIntFromString, NoSeparatorRejectsNonDigits) {
    helpers::expect_err(BigInt::from_string("12a34"), ParseError::InvalidCharacter);
    helpers::expect_err(BigInt::from_string("12-34"), ParseError::InvalidCharacter);
    helpers::expect_err(BigInt::from_string("   "),   ParseError::EmptyInput);
}

TEST(BigIntFromString, CharSeparator) {
    const auto res = BigInt::from_string("0001,02,003,004", ',');
    helpers::expect_ok(res);
    helpers::expect_eq(res.value(), "102003004");
}

TEST(BigIntFromString, SeparatorPreservesInternalZeros) {
    const auto res = BigInt::from_string("0001;0020;0030", ';');
    helpers::expect_ok(res);
    helpers::expect_eq(res.value(), "100200030");
}

TEST(BigIntFromString, SeparatorRejectsInvalidTokens) {
    helpers::expect_err(BigInt::from_string("1,a,2", ','), ParseError::InvalidCharacter);
    helpers::expect_err(BigInt::from_string(",,", ','),    ParseError::NoDigits);
    helpers::expect_err(BigInt::from_string("a,b,c", ','), ParseError::InvalidCharacter);
}

TEST(BigIntFromString, LeadingZerosOnlyTokensIgnored) {
    const auto res = BigInt::from_string("0000,0001,0020,0003", ',');
    helpers::expect_ok(res);
    helpers::expect_eq(res.value(), "100200003");
}

TEST(BigIntFromString, StringSeparator) {
    const auto res = BigInt::from_string("12__34__056", "__");
    helpers::expect_ok(res);
    helpers::expect_eq(res.value(), "1234056");
}

TEST(BigIntFromString, LargeNumberWithSpaces) {
    const std::string s = helpers::make_increasing(50);

    std::string spaced;
    for (char c : s)
        spaced += " " + std::string(1, c) + " ";

    const auto res = BigInt::from_string(spaced, ' ');
    helpers::expect_ok(res);
    helpers::expect_eq(res.value(), s);
}

TEST(BigIntFromString, ZeroValueAlwaysPositive) {
    {
        const auto res = BigInt::from_string("0000");
        helpers::expect_ok(res);
        helpers::expect_eq(res.value(), "0");
        EXPECT_FALSE(res->is_negative());
    }

    {
        const auto res = BigInt::from_string("-0000");
        helpers::expect_ok(res);
        helpers::expect_eq(res.value(), "0");
        EXPECT_FALSE(res->is_negative());
    }
}

} // namespace arbys::bignumbers::tests
