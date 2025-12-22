#include "arbys/bignumbers/bigint.h"
//
// Created by rares on 12/20/25.
//
namespace arbys::bignumbers {
    BigInt BigInt::add(const BigInt& other) const {
        if (this->is_negative_ != other.is_negative_) {
            // TODO: change sign of other('s copy)
            return this->sub(other);
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

        limb_t carry = 0;
        size_t i = 0;

        // Add common digits
        for (; i < smaller->length_; ++i) {
            const int sum = smaller->digits_[i] + bigger->digits_[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }

        // Add remaining digits from the bigger number
        for (; i < bigger->length_; ++i) {
            const int sum = bigger->digits_[i] + carry;
            digits[i] = sum % 10;
            carry = sum / 10;
        }

        // Extra carry at the highest digit
        if (carry) digits.push_back(carry);

        return { false, digits, digits.size() };
    }

    BigInt BigInt::operator+(const BigInt &other) const {
        return add(other);
    }
}