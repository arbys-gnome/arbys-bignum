#pragma once

#include <cstdint>

namespace arbys::bignum::detail {

using limb_t                        = uint32_t;
using dlimb_t                       = uint64_t;
using s_dlimb_t                     = int64_t;
static constexpr unsigned LIMB_BITS = 32;
static constexpr dlimb_t  BASE      = dlimb_t{1} << LIMB_BITS;

} // namespace arbys::bignum::detail