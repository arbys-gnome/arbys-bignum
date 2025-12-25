#include "arbys/bignumbers/big_int.h"
#include "big_int_internal.h"
#include "detail.h"
#include <algorithm>
#include <ranges>

namespace arbys::bignumbers::detail {

    std::pair<big_int, big_int> split_at(const big_int& num, const std::ptrdiff_t mid) {
        const auto& digits = big_int_access::limbs(num);

        if (mid >= static_cast<std::ptrdiff_t>(digits.size())) {
            return {big_int_access::create(false, {}), num};
        }

        const std::vector<limb_t> low_digits(digits.begin(), digits.begin() + mid);
        const std::vector<limb_t> high_digits(digits.begin() + mid, digits.end());

        auto low = big_int_access::create(false, low_digits);
        auto high = big_int_access::create(false, high_digits);

        return {high, low};
    }

    big_int shift_left(const big_int& num, std::ptrdiff_t n) {
        if (num.is_zero() || n == 0) {
            return num;
        }

        const auto& num_digits = big_int_access::limbs(num);
        const size_t num_len = big_int_access::length(num);

        std::vector<limb_t> result_digits(num_len + n, 0);
        std::ranges::copy(num_digits, result_digits.begin() + n);

        return big_int_access::create(big_int_access::is_negative(num),result_digits);
    }

    big_int simple_multiply(const big_int& lhs, const big_int& rhs) {
        if (lhs.is_zero() || rhs.is_zero()) {
            return big_int_access::create(false, {0});
        }

        const auto& lhs_digits = big_int_access::limbs(lhs);
        const auto& rhs_digits = big_int_access::limbs(rhs);

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

            size_t pos = i + rhs_digits.size();
            while (carry > 0) {
                const uint64_t sum = result_digits[pos] + carry;
                result_digits[pos] = static_cast<limb_t>(sum % 10);
                carry = sum / 10;
                ++pos;
            }
        }

        while (result_digits.size() > 1 && result_digits.back() == 0) {
            result_digits.pop_back();
        }

        return big_int_access::create(false, result_digits);
    }

    big_int karatsuba_multiply(const big_int& lhs, const big_int& rhs) {
        // TODO: fix karatsuba
        constexpr size_t karatsuba_threshold = 32;

        const size_t lhs_len = big_int_access::length(lhs);
        const size_t rhs_len = big_int_access::length(rhs);

        if (lhs_len <= karatsuba_threshold || rhs_len <= karatsuba_threshold) {
            return simple_multiply(lhs, rhs);
        }

        const size_t max_len = std::max(lhs_len, rhs_len);
        const auto mid = static_cast<ptrdiff_t>(max_len / 2);

        auto [high1, low1] = split_at(lhs, mid);
        auto [high2, low2] = split_at(rhs, mid);

        const big_int z0 = karatsuba_multiply(low1, low2);
        const big_int z2 = karatsuba_multiply(high1, high2);

        const big_int sum1 = low1 + high1;
        const big_int sum2 = low2 + high2;
        const big_int z1 = karatsuba_multiply(sum1, sum2) - z2 - z0;

        auto result = shift_left(z2, 2 * mid) + shift_left(z1, mid) + z0;

        return result;
    }

    big_int mul_abs(const big_int& lhs, const big_int& rhs) {
        return simple_multiply(lhs, rhs);
    }

}
