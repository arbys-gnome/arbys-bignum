#include <vector>

#include "arbys/bignumbers/bigint.h"
#include "bigint_impl.h"

namespace arbys::bignumbers::detail {

    void normalize_abs(std::vector<limb_t>& digits) {
        int carry = 0;

        for (size_t i = 0; i < digits.size(); ++i) {
            int value = digits[i] + carry;
            carry = value / 10;
            value %= 10;

            if (value < 0) {
                int borrow = (-value + 9) / 10;
                value += borrow * 10;
                carry -= borrow;
            }

            digits[i] = static_cast<limb_t>(value);
        }

        while (carry > 0) {
            digits.push_back(static_cast<limb_t>(carry % 10));
            carry /= 10;
        }

        // trim leading zeros
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
    }

    void normalize_abs_simple(std::vector<limb_t>& digits) {
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

    void normalize_simple(BigInt& bi) {
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

    void normalize(BigInt& bi) {
        int carry = 0;

        auto &digits = BigIntImpl::digits(bi);
        for (size_t i = 0; i < digits.size(); ++i) {
            int value = digits[i] + carry;
            carry = value / 10;
            value %= 10;

            if (value < 0) {
                int borrow = (-value + 9) / 10;
                value += borrow * 10;
                carry -= borrow;
            }

            digits[i] = static_cast<limb_t>(value);
        }

        while (carry > 0) {
            digits.push_back(static_cast<limb_t>(carry % 10));
            carry /= 10;
        }

        // trim leading zeros
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
    }

}
