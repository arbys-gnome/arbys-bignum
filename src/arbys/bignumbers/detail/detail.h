#ifndef SRC_DETAILS_DETAILS_HPP_
#define SRC_DETAILS_DETAILS_HPP_

#include <expected>
#include <locale>
#include <string_view>
#include <vector>

#include "arbys/bignumbers/errors.h"
#include "arbys/bignumbers/bigint.h"

namespace arbys::bignumbers::detail {

    constexpr bool is_space(unsigned char c) noexcept {
        return std::isspace(c);
    }

    /// Returns a view into the original character sequence.
    /// The caller must ensure the underlying storage outlives the returned view.
    constexpr std::string_view trim_view(std::string_view sv) noexcept {
        while (!sv.empty() && is_space(sv.front()))
            sv.remove_prefix(1);

        while (!sv.empty() && is_space(sv.back()))
            sv.remove_suffix(1);

        return sv;
    }

    std::expected<BigInt, ParseError>
    parse_digits(std::string_view input, bool isNegative);

    void normalize_abs(std::vector<limb_t>& digits);
    void normalize(bool& is_negative, std::vector<limb_t>& digits);
    void normalize(BigInt& bi);

    std::strong_ordering cmp_abs(const BigInt &lhs, const BigInt &rhs);


    BigInt add_abs(const BigInt& lhs, const BigInt& rhs);

    BigInt sub_abs(const BigInt& a, const BigInt& b);

    // Helper to split a BigInt into two halves at position mid
    std::pair<BigInt, BigInt> split_at(const BigInt &num, std::ptrdiff_t mid);

    // Shift left by n digits (multiply by base^n)
    BigInt shift_left(const BigInt &num, size_t n);

    // Karatsuba multiplication for unsigned BigInts
    BigInt karatsuba_multiply(const BigInt &lhs, const BigInt &rhs);

    // Simple O(n^2) multiplication for base case
    BigInt simple_multiply(const BigInt &lhs, const BigInt &rhs);

    BigInt mul_abs(const BigInt &lhs, const BigInt &rhs);
} // namespace detail

#endif