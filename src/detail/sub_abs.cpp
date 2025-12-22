#include "arbys/bignumbers/bigint.h"
#include "bigint_impl.h"
#include "detail.h"

namespace arbys::bignumbers::detail {

    BigInt sub_abs(const BigInt& a, const BigInt& b) {
        const auto& a_digits = BigIntImpl::digits(a);
        const auto& b_digits = BigIntImpl::digits(b);
        const size_t a_len = BigIntImpl::length(a);
        const size_t b_len = BigIntImpl::length(b);

        std::vector<limb_t> result_digits(a_len, 0);
        int borrow = 0;

        for (size_t i = 0; i < b_len; ++i) {
            int diff = a_digits[i] - b_digits[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result_digits[i] = static_cast<limb_t>(diff);
        }

        for (size_t i = b_len; i < a_len; ++i) {
            int diff = a_digits[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result_digits[i] = static_cast<limb_t>(diff);
        }

        while (result_digits.size() > 1 && result_digits.back() == 0) {
            result_digits.pop_back();
        }

        return BigIntImpl::create(false, std::move(result_digits), result_digits.size());
    }

}
