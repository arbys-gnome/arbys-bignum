#include <ranges>

#include "arbys/bignumbers/bigint.h"

namespace arbys::bignumbers {
    BigInt BigInt::sub(const BigInt &other) const {
        if (this->is_negative_ != other.is_negative_) {
            // TODO: change sign of other('s copy)
            return this->add(other);
        }
        const BigInt* bigger;
        const BigInt* smaller;

        if (this->length_ < other.length_) {
            smaller = this;
            bigger  = &other;
        } else {
            smaller = &other;
            bigger  = this;
        }

        std::vector<limb_t> digits;
        digits.resize(bigger->length_);

        int borrow = 0;
        size_t i = 0;
        for (; i < smaller->length_; ++i) {
            if (bigger->digits_[i] - borrow >= smaller->digits_[i]) {
                digits[i] = bigger->digits_[i] - smaller->digits_[i] - borrow;
                borrow = 0;
            } else {
                digits[i] = 10 + bigger->digits_[i] - smaller->digits_[i] - borrow;
                borrow = 1;
            }
        }

        for (; i < bigger->length_; ++i) {
            if (borrow) {
                if (bigger->digits_[i] == 0) {
                    digits[i] = 9;
                } else {
                    digits[i] = bigger->digits_[i] - borrow;
                    borrow = 0;
                }
            } else {
                digits[i] = bigger->digits_[i];
            }
        }

        size_t length = digits.size();
        for (const auto d : digits | std::views::drop(1) | std::views::reverse) {
            if (d == 0) length--;
        }

        return { false, digits, length };
    }

    BigInt BigInt::operator-(const BigInt &other) const {
        return sub(other);
    }

}
