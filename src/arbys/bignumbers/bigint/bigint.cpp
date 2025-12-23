#include "arbys/bignumbers/bigint.h"

#include <expected>
#include <regex>
#include <stdexcept>
#include <ranges>

namespace arbys::bignumbers {
    BigInt::BigInt() {
        // Zero initialization
        length_ = 1;
        digits_.push_back(0);
        is_negative_ = false;
    }

    BigInt::BigInt(bool is_negative, const std::vector<limb_t> &digits, size_t length)
        : digits_(digits),
          length_(length),
          is_negative_(is_negative) {
        digits_.resize(length_);
    }

    limb_t BigInt::get_digit(const size_t index) const {
        return digits_[length_ - 1 - index];
    }

    BigInt* BigInt::to_abs() {
        this->is_negative_ = false;
        return this;
    }

    std::string BigInt::to_string() const {
        std::string s;
        for (const auto &digit : digits_ | std::views::reverse) {
            s += std::to_string(digit);
        }
        return s;
    }
} // arbys::bignumbers

