#ifndef BIGNUMBERS_BIGINT_IMPL_H
#define BIGNUMBERS_BIGINT_IMPL_H

#include "arbys/bignumbers/bigint.h"

namespace arbys::bignumbers {

class BigIntImpl {
public:
    BigIntImpl() = delete;
    BigIntImpl(const BigIntImpl&) = delete;
    BigIntImpl& operator=(const BigIntImpl&) = delete;

    static const std::vector<limb_t>& digits(const BigInt& bi) {
        return bi.digits_;
    }

    static std::vector<limb_t>& digits(BigInt& bi) {
        return bi.digits_;
    }

    static size_t length(const BigInt& bi) {
        return bi.length_;
    }

    static void set_length(BigInt& bi, size_t len) {
        bi.length_ = len;
    }

    static bool is_negative(const BigInt& bi) {
        return bi.is_negative_;
    }

    static void set_negative(BigInt& bi, bool neg) {
        bi.is_negative_ = neg;
    }

    static BigInt create(bool is_negative, const std::vector<limb_t> &digits, size_t length) {
        return { is_negative, digits, length };
    }
};

} // namespace arbys::bignumbers

#endif // BIGNUMBERS_BIGINT_IMPL_H
