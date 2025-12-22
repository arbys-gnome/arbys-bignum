#include <vector>

#include "arbys/bignumbers/bigint.h"
#include "bigint_impl.h"

namespace arbys::bignumbers::detail {

    void normalize_abs(std::vector<limb_t>& digits) {
        // The canonical form has no leading zeros
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
    }

    void normalize(bool& is_negative, std::vector<limb_t>& digits) {
        // The canonical form has no leading zeros
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();

        // Canonical zero: zero is never negative
        if (digits.size() == 1 && digits[0] == 0)
            is_negative = false;
    }

    void normalize(BigInt& bi) {
        auto& digits = BigIntImpl::digits(bi);

        // The canonical form has no leading zeros
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }

        // Update length to match actual digit count
        BigIntImpl::set_length(bi, digits.size());

        // Canonical zero: zero is never negative
        if (bi.is_zero()) {
            BigIntImpl::set_negative(bi, false);
        }
    }

}
