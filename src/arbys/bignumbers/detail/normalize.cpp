#include <vector>

#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"

namespace arbys::bignumbers::detail {

    void normalize_abs(std::vector<limb_t>& limbs) {
        int carry = 0;

        for (unsigned int &limb : limbs) {
            int value = limb + carry;
            carry = value / 10;
            value %= 10;

            if (value < 0) {
                int borrow = (-value + 9) / 10;
                value += borrow * 10;
                carry -= borrow;
            }

            limb = static_cast<limb_t>(value);
        }

        while (carry > 0) {
            limbs.push_back(static_cast<limb_t>(carry % 10));
            carry /= 10;
        }

        // trim leading zeros
        while (limbs.size() > 1 && limbs.back() == 0)
            limbs.pop_back();
    }

    void normalize_abs_simple(std::vector<limb_t>& limbs) {
        // The canonical form has no leading zeros
        while (limbs.size() > 1 && limbs.back() == 0)
            limbs.pop_back();
    }

    void normalize(bool& is_negative, std::vector<limb_t>& limbs) {
        // The canonical form has no leading zeros
        while (limbs.size() > 1 && limbs.back() == 0)
            limbs.pop_back();

        // Canonical zero: zero is never negative
        if (limbs.size() == 1 && limbs[0] == 0)
            is_negative = false;
    }
}
