#include "arbys/bignumbers/bigint.h"
#include "bigint_impl.h"
#include "detail.h"

namespace arbys::bignumbers::detail {

    BigInt add_abs(const BigInt& lhs, const BigInt& rhs) {
        const BigInt* bigger = (BigIntImpl::length(lhs) >= BigIntImpl::length(rhs)) ? &lhs : &rhs;
        const BigInt* smaller = (bigger == &lhs) ? &rhs : &lhs;

        const auto& bigger_digits = BigIntImpl::digits(*bigger);
        const auto& smaller_digits = BigIntImpl::digits(*smaller);
        const size_t bigger_len = BigIntImpl::length(*bigger);
        const size_t smaller_len = BigIntImpl::length(*smaller);

        // +1 for possible carry
        std::vector<limb_t> digits(bigger_len + 1, 0);
        limb_t carry = 0;

        for (size_t i = 0; i < smaller_len; ++i) {
            const uint16_t sum = bigger_digits[i] + smaller_digits[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }

        for (size_t i = smaller_len; i < bigger_len; ++i) {
            const uint16_t sum = bigger_digits[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }

        if (carry) {
            digits[bigger_len] = carry;
        }

        normalize_abs(digits);

        // Use factory method to create result
        return BigIntImpl::create(false, digits, digits.size());
    }
}
