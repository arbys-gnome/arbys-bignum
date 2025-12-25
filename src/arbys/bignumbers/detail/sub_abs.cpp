#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"
#include "detail.h"

namespace arbys::bignumbers::detail {

    big_int sub_abs(const big_int& a, const big_int& b) {
        const auto& a_limbs = big_int_access::limbs(a);
        const auto& b_limbs = big_int_access::limbs(b);
        const size_t a_len = big_int_access::length(a);
        const size_t b_len = big_int_access::length(b);

        std::vector<limb_t> result_limbs(a_len, 0);
        int borrow = 0;

        for (size_t i = 0; i < b_len; ++i) {
            int diff = a_limbs[i] - b_limbs[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result_limbs[i] = static_cast<limb_t>(diff);
        }

        for (size_t i = b_len; i < a_len; ++i) {
            int diff = a_limbs[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result_limbs[i] = static_cast<limb_t>(diff);
        }

        normalize_abs(result_limbs);

        return big_int_access::create(false, result_limbs);
    }

}
