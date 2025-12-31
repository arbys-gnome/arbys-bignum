#pragma once

#include "config.h"

#include <ranges>
#include <vector>

namespace arbys::bignum::detail {
struct big_int_impl {
    bool                is_negative_ = false;
    std::vector<limb_t> limbs_;      // stored in reverse (LSB first)
    size_t              length_ = 0; // limb count

    big_int_impl() : limbs_{0}, length_(1) {}

    big_int_impl(bool negative, std::vector<limb_t> limbs)
      : is_negative_(negative), limbs_(std::move(limbs)), length_(limbs_.size()) {
        normalize();
    }

    // Remove leading zeros and handle zero sign
    void normalize() {
        while (length_ > 1 && limbs_[length_ - 1] == 0) {
            length_--;
        }

        // Resize vector to match actual length (save memory)
        if (limbs_.size() > length_) {
            limbs_.resize(length_);
        }

        if (length_ == 1 && limbs_[0] == 0) {
            is_negative_ = false;
        }
    }

    [[nodiscard]] size_t length() const noexcept { return length_; }

    [[nodiscard]] limb_t operator[](const std::size_t index) const noexcept { return limbs_[index]; }

    [[nodiscard]] limb_t &operator[](const std::size_t index) noexcept { return limbs_[index]; }

    template <class Out> Out write_digits_to(Out out) const {
        if (length_ == 1 && limbs_[0] == 0) {
            *out++ = '0';
            return out;
        }

        // Copy limbs
        std::vector<detail::limb_t> temp     = limbs_;
        std::size_t                 temp_len = length_;

        // Extract digits in reverse (least → most significant)
        std::vector<char> digits;
        digits.reserve(temp_len * 10); // log10(2^32) ≈ 9.63

        while (temp_len > 1 || temp[0] > 0) {
            detail::dlimb_t remainder = 0;

            for (std::size_t i = temp_len; i-- > 0;) {
                const detail::dlimb_t current = (remainder << detail::LIMB_BITS) | temp[i];
                temp[i]                       = static_cast<detail::limb_t>(current / 10);
                remainder                     = current % 10;
            }

            while (temp_len > 1 && temp[temp_len - 1] == 0)
                --temp_len;

            digits.push_back(static_cast<char>('0' + remainder));
        }

        // Emit digits in correct order
        for (char digit : std::ranges::reverse_view(digits))
            *out++ = digit;

        return out;
    }
};

struct big_int_access {
    static const std::vector<limb_t> &limbs(const big_int &n) noexcept { return n.impl_->limbs_; }

    static size_t length(const big_int &n) noexcept { return n.impl_->length_; }

    static bool is_negative(const big_int &n) noexcept { return n.impl_->is_negative_; }

    // Write access for creating results
    static big_int create(bool negative, std::vector<limb_t> limbs) {
        return big_int(std::make_unique<big_int_impl>(negative, std::move(limbs)));
    }

    static big_int create_abs(std::vector<limb_t> limbs) {
        return big_int(std::make_unique<big_int_impl>(false, std::move(limbs)));
    }

    // Direct impl access
    static big_int_impl &impl(big_int &n) noexcept { return *n.impl_; }

    static const big_int_impl &impl(const big_int &n) noexcept { return *n.impl_; }
};
} // namespace arbys::bignum::detail