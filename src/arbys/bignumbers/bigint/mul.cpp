#include "arbys/bignumbers/bigint.h"
#include "../detail/detail.h"

namespace arbys::bignumbers {
    BigInt BigInt::mul(const BigInt &other) const {
        if (this->is_zero() || other.is_zero()) {
            return BigInt{};
        }

        const bool result_is_negative = this->is_negative_ != other.is_negative_;

        BigInt result = detail::mul_abs(*this, other);

        result.is_negative_ = result_is_negative && !result.is_zero();

        return result;
    }

    BigInt BigInt::operator*(const BigInt &other) const {
        return mul(other);
    }
}
