#include <compare>

#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"

namespace arbys::bignumbers::detail {

std::strong_ordering cmp_abs(const big_int &lhs, const big_int &rhs) {
    if (big_int_access::length(lhs) != big_int_access::length(rhs)) {
        return (big_int_access::length(lhs) < big_int_access::length(rhs)) ? std::strong_ordering::less
                                                                           : std::strong_ordering::greater;
    }

    const auto &lhs_digits = big_int_access::limbs(lhs);
    const auto &rhs_digits = big_int_access::limbs(rhs);

    // Compare from MSD (reverse iterators)
    return std::lexicographical_compare_three_way(
      lhs_digits.rbegin(), lhs_digits.rend(), rhs_digits.rbegin(), rhs_digits.rend()
    );
}

} // namespace arbys::bignumbers::detail
