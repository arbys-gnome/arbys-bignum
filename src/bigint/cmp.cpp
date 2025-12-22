//
// Created by rares on 12/20/25.
//

#include <compare>

#include "arbys/bignumbers/bigint.h"
#include "../detail/detail.h"

namespace arbys::bignumbers {
    std::strong_ordering BigInt::cmp(const BigInt &other) const {
        // different signs: negative is always smaller
        if (is_negative_ != other.is_negative_) {
            return is_negative_
                ? std::strong_ordering::less
                : std::strong_ordering::greater;
        }

        // same sign: compare abs values
        const std::strong_ordering abs_cmp = detail::cmp_abs(*this, other);

        // both are negative: invert the result
        if (this->is_negative_) {
            if (abs_cmp == std::strong_ordering::less)
                return std::strong_ordering::greater;
            if (abs_cmp == std::strong_ordering::greater)
                return std::strong_ordering::less;
        }

        // both are positive:
        return abs_cmp;
    }

}
