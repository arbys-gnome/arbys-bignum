#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"
#include "detail.h"

namespace arbys::bignumbers::detail {

    big_int add_abs(const big_int& lhs, const big_int& rhs) {
        const big_int* bigger = (big_int_access::length(lhs) >= big_int_access::length(rhs)) ? &lhs : &rhs;
        const big_int* smaller = (bigger == &lhs) ? &rhs : &lhs;

        const auto& bigger_limbs = big_int_access::limbs(*bigger);
        const auto& smaller_limbs = big_int_access::limbs(*smaller);
        const size_t bigger_len = big_int_access::length(*bigger);
        const size_t smaller_len = big_int_access::length(*smaller);

        // +1 for possible carry
        std::vector<limb_t> result(bigger_len + 1);
        dlimb_t carry = 0;

        // Add overlapping limbs
        for (size_t i = 0; i < smaller_len; ++i) {
            const dlimb_t sum = dlimb_t{bigger_limbs[i]} + smaller_limbs[i] + carry;
            result[i] = static_cast<limb_t>(sum);  // Lower 32 bits
            carry = sum >> LIMB_BITS;              // Upper 32 bits (0 or 1)
        }

        // Copy remaining limbs from bigger number
        for (size_t i = smaller_len; i < bigger_len; ++i) {
            const dlimb_t sum = dlimb_t{bigger_limbs[i]} + carry;
            result[i] = static_cast<limb_t>(sum);
            carry = sum >> LIMB_BITS;
        }

        // Handle final carry
        if (carry) {
            result[bigger_len] = static_cast<limb_t>(carry);
        } else {
            result.resize(bigger_len);  // No carry, remove extra slot
        }

        return big_int_access::create(false, std::move(result));
    }
}
