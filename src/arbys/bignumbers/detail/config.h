#ifndef BIGNUMBERS_CONFIG_H
#define BIGNUMBERS_CONFIG_H
#include <cstdint>

namespace arbys::bignumbers::detail {
    using limb_t  = uint32_t;
    using dlimb_t = uint64_t;
    static constexpr unsigned LIMB_BITS = 32;
    static constexpr dlimb_t BASE = dlimb_t{1} << LIMB_BITS;
}

#endif //BIGNUMBERS_CONFIG_H