#ifndef SRC_DETAILS_DETAILS_HPP_
#define SRC_DETAILS_DETAILS_HPP_

#include "arbys/bignumbers/big_int.h"
#include "arbys/bignumbers/errors.h"
#include "arbys/bignumbers/results.h"
#include "config.h"

#include <expected>
#include <locale>
#include <string_view>
#include <vector>

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

    std::expected<big_int, errors::ParseError>
    parse_limbs(std::string_view input, bool is_negative);

    std::expected<big_int, errors::ParseError>
    parse_limbs_optimized(std::string_view input, bool is_negative);

    void normalize_abs(std::vector<limb_t>& digits);
    void normalize_abs_simple(std::vector<limb_t>& digits);
    void normalize(bool& is_negative, std::vector<limb_t>& digits);

    std::strong_ordering cmp_abs(const big_int &lhs, const big_int &rhs);

    big_int add_abs(const big_int& lhs, const big_int& rhs);

    big_int sub_abs(const big_int& bigger, const big_int& smaller);

    // Helper to split a BigInt into two halves at position mid
    std::pair<big_int, big_int> split_at(const big_int &num, std::ptrdiff_t mid);

    // Shift left by n digits (multiply by base^n)
    big_int shift_left(const big_int &num, size_t n);

    // Karatsuba multiplication for unsigned BigInts
    big_int karatsuba_multiply(const big_int &lhs, const big_int &rhs);

    // Simple O(n^2) multiplication for base case
    big_int simple_multiply(const big_int &lhs, const big_int &rhs);

    big_int mul_abs(const big_int &lhs, const big_int &rhs);

    [[nodiscard]] std::expected<DivisionResult, errors::ArithmeticError>
    div_mod_abs(const big_int& dividend, const big_int& divisor) noexcept;

    [[nodiscard]] std::expected<big_int, errors::ArithmeticError>
    div_abs(const big_int& dividend, const big_int& divisor) noexcept;

    [[nodiscard]] std::expected<big_int, errors::ArithmeticError>
    mod_abs(const big_int& dividend, const big_int& divisor) noexcept;

} // namespace detail

#endif