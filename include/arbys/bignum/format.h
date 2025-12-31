#pragma once

#include <format>
#include <string>

#include "big_int.h"

template <> struct std::formatter<arbys::bignum::big_int> {
    constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

    auto format(const arbys::bignum::big_int &bi, std::format_context &ctx) const { return bi.format_to(ctx.out()); }
};
