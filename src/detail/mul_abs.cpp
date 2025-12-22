#include "arbys/bignumbers/bigint.h"
#include "bigint_impl.h"
#include "detail.h"
#include <algorithm>
#include <ranges>

namespace arbys::bignumbers::detail {

    std::pair<BigInt, BigInt> split_at(const BigInt& num, std::ptrdiff_t mid) {
        const auto& digits = BigIntImpl::digits(num);

        if (mid >= static_cast<std::ptrdiff_t>(digits.size())) {
            return {BigIntImpl::create(false, {}, 0), num};
        }

        const std::vector<limb_t> low_digits(digits.begin(), digits.begin() + mid);
        const std::vector<limb_t> high_digits(digits.begin() + mid, digits.end());

        auto low = BigIntImpl::create(false, low_digits, low_digits.size());
        auto high = BigIntImpl::create(false, high_digits, high_digits.size());

        return {high, low};
    }

    BigInt shift_left(const BigInt& num, std::ptrdiff_t n) {
        if (num.is_zero() || n == 0) {
            return num;
        }

        const auto& num_digits = BigIntImpl::digits(num);
        const size_t num_len = BigIntImpl::length(num);

        std::vector<limb_t> result_digits(num_len + n, 0);
        std::ranges::copy(num_digits, result_digits.begin() + n);

        return BigIntImpl::create(
            BigIntImpl::is_negative(num),
            result_digits,
            result_digits.size()
        );
    }

    BigInt simple_multiply(const BigInt& lhs, const BigInt& rhs) {
        if (lhs.is_zero() || rhs.is_zero()) {
            return BigIntImpl::create(false, {0}, 1);
        }

        const auto& lhs_digits = BigIntImpl::digits(lhs);
        const auto& rhs_digits = BigIntImpl::digits(rhs);

        std::vector<limb_t> result_digits(lhs_digits.size() + rhs_digits.size(), 0);

        for (size_t i = 0; i < lhs_digits.size(); ++i) {
            uint64_t carry = 0;

            for (size_t j = 0; j < rhs_digits.size(); ++j) {
                const uint64_t prod = result_digits[i + j] +
                                      static_cast<uint64_t>(lhs_digits[i]) * rhs_digits[j] +
                                      carry;

                result_digits[i + j] = static_cast<limb_t>(prod % 10);
                carry = prod / 10;
            }

            if (carry > 0) {
                result_digits[i + rhs_digits.size()] += static_cast<limb_t>(carry);
            }
        }

        while (result_digits.size() > 1 && result_digits.back() == 0) {
            result_digits.pop_back();
        }

        return BigIntImpl::create(false, result_digits, result_digits.size());
    }

    BigInt karatsuba_multiply(const BigInt& lhs, const BigInt& rhs) {
        constexpr size_t karatsuba_threshold = 32;

        const size_t lhs_len = BigIntImpl::length(lhs);
        const size_t rhs_len = BigIntImpl::length(rhs);

        if (lhs_len <= karatsuba_threshold || rhs_len <= karatsuba_threshold) {
            return simple_multiply(lhs, rhs);
        }

        const size_t max_len = std::max(lhs_len, rhs_len);
        const auto mid = static_cast<ptrdiff_t>(max_len / 2);

        auto [high1, low1] = split_at(lhs, mid);
        auto [high2, low2] = split_at(rhs, mid);

        const BigInt z0 = karatsuba_multiply(low1, low2);
        const BigInt z2 = karatsuba_multiply(high1, high2);

        const BigInt sum1 = low1 + high1;
        const BigInt sum2 = low2 + high2;
        const BigInt z1 = karatsuba_multiply(sum1, sum2) - z2 - z0;

        return shift_left(z2, 2 * mid) + shift_left(z1, mid) + z0;
    }

    BigInt mul_abs(const BigInt& lhs, const BigInt& rhs) {
        return karatsuba_multiply(lhs, rhs);
    }

}
