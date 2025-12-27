#include <vector>

#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"

namespace arbys::bignumbers::detail {

    void normalize_abs(std::vector<limb_t>& limbs) {
        dlimb_t carry = 0;

        for (limb_t& limb : limbs) {
            const dlimb_t value = static_cast<dlimb_t>(limb) + carry;
            limb = static_cast<limb_t>(value);  // Keep lower LIMB_BITS bits
            carry = value >> LIMB_BITS;         // Propagate upper bits as carry
        }

        // Handle remaining carry
        while (carry > 0) {
            limbs.emplace_back(static_cast<limb_t>(carry));
            carry >>= LIMB_BITS;
        }

        // Trim leading zeros
        while (limbs.size() > 1 && limbs.back() == 0) {
            limbs.pop_back();
        }
    }

    void normalize_abs_simple(std::vector<limb_t>& limbs) {
        // The canonical form has no leading zeros
        while (limbs.size() > 1 && limbs.back() == 0) {
            limbs.pop_back();
        }
    }

    void normalize(bool& is_negative, std::vector<limb_t>& limbs) {
        // The canonical form has no leading zeros
        while (limbs.size() > 1 && limbs.back() == 0) {
            limbs.pop_back();
        }

        // Canonical zero: zero is never negative
        if (limbs.size() == 1 && limbs[0] == 0) {
            is_negative = false;
        }
    }
}
