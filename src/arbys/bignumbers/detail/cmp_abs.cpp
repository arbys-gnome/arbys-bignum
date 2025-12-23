#include <compare>
#include <ranges>

#include "arbys/bignumbers/bigint.h"
#include "bigint_impl.h"

namespace arbys::bignumbers::detail {

    std::strong_ordering cmp_abs(const BigInt &lhs, const BigInt &rhs) {
        if (BigIntImpl::length(lhs) != BigIntImpl::length(rhs)) {
            return (BigIntImpl::length(lhs) < BigIntImpl::length(rhs))
                ? std::strong_ordering::less
                : std::strong_ordering::greater;
        }

        const auto& lhs_digits = BigIntImpl::digits(lhs);
        const auto& rhs_digits = BigIntImpl::digits(rhs);

        // Compare from MSD (reverse iterators)
        return std::lexicographical_compare_three_way(
            lhs_digits.rbegin(), lhs_digits.rend(),
            rhs_digits.rbegin(), rhs_digits.rend()
        );
    }

}
