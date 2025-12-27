#include <ranges>

#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"
#include "detail.h"

namespace arbys::bignumbers::detail {

    // this function assumes that a > b
    big_int sub_abs(const big_int &bigger, const big_int &smaller) {
        const auto &bigger_limbs = big_int_access::limbs(bigger);
        const auto &smaller_limbs = big_int_access::limbs(smaller);

        const size_t bigger_len = big_int_access::length(bigger);
        const size_t smaller_len = big_int_access::length(smaller);

        std::vector<limb_t> result;
        result.reserve(bigger_len);

        s_dlimb_t borrow = 0;

        // subtract common limbs
        for (size_t i = 0; i < smaller_len; ++i) {
            s_dlimb_t diff =
                static_cast<s_dlimb_t>(bigger_limbs[i])
              - static_cast<s_dlimb_t>(smaller_limbs[i])
              - borrow;

            if (diff < 0) {
                diff += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.push_back(static_cast<limb_t>(diff));
        }

        // propagate borrow through remaining limbs
        for (size_t i = smaller_len; i < bigger_len; ++i) {
            s_dlimb_t diff =
                static_cast<s_dlimb_t>(bigger_limbs[i])
              - borrow;

            if (diff < 0) {
                diff += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.push_back(static_cast<limb_t>(diff));
        }

        // borrow must be zero here
        // assert(borrow == 0);

        normalize_abs(result);
        return big_int_access::create(false, std::move(result));
    }

}
