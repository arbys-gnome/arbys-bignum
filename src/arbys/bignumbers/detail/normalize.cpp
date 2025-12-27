#include <vector>

#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"

namespace arbys::bignumbers::detail {

/// Removes leading zero limbs (canonical form requirement)
void trim_leading_zeros(std::vector<limb_t> &limbs) {
    while (limbs.size() > 1 && limbs.back() == 0) {
        limbs.pop_back();
    }
}

/// Propagates carries through limbs and trims leading zeros
/// Use after operations that may produce limbs >= BASE
void propagate_carries(std::vector<limb_t> &limbs) {
    dlimb_t carry = 0;

    for (limb_t &limb : limbs) {
        const dlimb_t value = static_cast<dlimb_t>(limb) + carry;
        limb                = static_cast<limb_t>(value);
        carry               = value >> LIMB_BITS;
    }

    while (carry > 0) {
        limbs.emplace_back(static_cast<limb_t>(carry));
        carry >>= LIMB_BITS;
    }

    trim_leading_zeros(limbs);
}

} // namespace arbys::bignumbers::detail
