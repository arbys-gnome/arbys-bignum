#include <cassert>
#include <ranges>

#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"
#include "detail.h"

namespace arbys::bignumbers::detail {

/// Subtracts the absolute values: |lhs| - |rhs|
/// Precondition: |lhs| >= |rhs| (undefined behavior otherwise)
big_int sub_abs(const big_int &lhs, const big_int &rhs) {
    const auto &lhs_limbs = big_int_access::limbs(lhs);
    const auto &rhs_limbs = big_int_access::limbs(rhs);

    const size_t lhs_len = big_int_access::length(lhs);
    const size_t rhs_len = big_int_access::length(rhs);

    std::vector<limb_t> result;
    result.reserve(lhs_len);

    s_dlimb_t borrow = 0;

    // subtract common limbs
    for (size_t i = 0; i < rhs_len; ++i) {
        s_dlimb_t diff = static_cast<s_dlimb_t>(lhs_limbs[i]) - static_cast<s_dlimb_t>(rhs_limbs[i]) - borrow;

        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.push_back(static_cast<limb_t>(diff));
    }

    // propagate borrow through remaining limbs
    for (size_t i = rhs_len; i < lhs_len; ++i) {
        s_dlimb_t diff = static_cast<s_dlimb_t>(lhs_limbs[i]) - borrow;

        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.push_back(static_cast<limb_t>(diff));
    }

    // borrow must be zero here
    assert(borrow == 0 && "sub_abs precondition violated: lhs < rhs");

    trim_leading_zeros(result);
    return big_int_access::create_abs(std::move(result));
}

} // namespace arbys::bignumbers::detail
