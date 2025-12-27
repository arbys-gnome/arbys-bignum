#include "arbys/bignum/big_int.h"

#include "big_int_internal.h"
#include "config.h"
#include "detail.h"

#include <algorithm>
#include <expected>

namespace arbys::bignum::detail {

/// Compare absolute values for division algorithm
[[nodiscard]] static std::strong_ordering cmp_abs_range(
  const big_int &a,
  size_t         a_start,
  size_t         a_len,
  const big_int &b,
  size_t         b_len
) noexcept {
    if (a_len != b_len) {
        return a_len <=> b_len;
    }

    const auto &a_limbs = big_int_access::limbs(a);
    const auto &b_limbs = big_int_access::limbs(b);

    // Compare from most significant to least significant
    for (size_t i = a_len; i-- > 0;) {
        if (const auto cmp = a_limbs[a_start + i] <=> b_limbs[i]; cmp != 0) {
            return cmp;
        }
    }
    return std::strong_ordering::equal;
}

/// Subtract b from a[start..start+len] in-place
/// Precondition: a[start..start+len] >= b
static void sub_abs_inplace(std::vector<limb_t> &a, const size_t start, const big_int &b, const size_t b_len) noexcept {
    const auto &b_limbs = big_int_access::limbs(b);
    dlimb_t     borrow  = 0;

    // Subtract common limbs
    for (size_t i = 0; i < b_len; ++i) {
        const dlimb_t a_val = a[start + i];
        const dlimb_t b_val = b_limbs[i];
        const dlimb_t diff  = a_val - b_val - borrow;

        a[start + i] = static_cast<limb_t>(diff);
        borrow       = (diff > a_val) ? 1 : 0; // Underflow detection
    }

    // Propagate borrow through remaining limbs
    for (size_t i = b_len; borrow && (start + i < a.size()); ++i) {
        if (a[start + i] == 0) {
            a[start + i] = ~limb_t{0}; // 0xFFFFFFFF
        } else {
            a[start + i]--;
            borrow = 0;
        }
    }
}

/// Normalize a number by shifting left until leading limb >= BASE/2
/// Returns the shift amount (0-31 bits)
[[nodiscard]] static unsigned calculate_normalization_shift(const limb_t leading_limb) noexcept {
    if (leading_limb == 0)
        return 0;

    unsigned shift = 0;
    limb_t   mask  = limb_t{1} << (LIMB_BITS - 1);

    while ((leading_limb & mask) == 0) {
        shift++;
        mask >>= 1;
    }

    return shift;
}

/// Shift vector left by 'shift' bits, returns carry
[[nodiscard]] static limb_t shift_left(std::vector<limb_t> &vec, const size_t len, const unsigned shift) noexcept {
    if (shift == 0)
        return 0;

    dlimb_t carry = 0;
    for (size_t i = 0; i < len; ++i) {
        const dlimb_t val = (dlimb_t{vec[i]} << shift) | carry;
        vec[i]            = static_cast<limb_t>(val);
        carry             = val >> LIMB_BITS;
    }
    return static_cast<limb_t>(carry);
}

/// Shift vector right by 'shift' bits
static void shift_right(std::vector<limb_t> &vec, const size_t len, const unsigned shift) noexcept {
    if (shift == 0)
        return;

    for (size_t i = 0; i < len; ++i) {
        vec[i] = vec[i] >> shift;
        if (i + 1 < len) {
            vec[i] |= vec[i + 1] << (LIMB_BITS - shift);
        }
    }
}

[[nodiscard]] static std::expected<DivisionResult, errors::ArithmeticError> div_single_limb(
  const big_int &dividend,
  const limb_t   divisor
) noexcept {
    const size_t m              = big_int_access::length(dividend);
    const auto  &dividend_limbs = big_int_access::limbs(dividend);

    std::vector<limb_t> quotient(m);
    dlimb_t             remainder = 0;

    // Divide from most significant to least significant
    for (size_t i = m; i-- > 0;) {
        const dlimb_t current = (remainder << LIMB_BITS) | dividend_limbs[i];
        quotient[i]           = static_cast<limb_t>(current / divisor);
        remainder             = current % divisor;
    }

    big_int q = big_int_access::create(false, std::move(quotient));
    big_int r = big_int::from_integer(static_cast<limb_t>(remainder));

    return DivisionResult{std::move(q), std::move(r)};
}

// Knuth's Algorithm D
[[nodiscard]] static std::expected<DivisionResult, errors::ArithmeticError> div_multi_limb(
  const big_int &dividend,
  const big_int &divisor
) noexcept {
    const size_t m = big_int_access::length(dividend);
    const size_t n = big_int_access::length(divisor);

    const auto &dividend_limbs = big_int_access::limbs(dividend);
    const auto &divisor_limbs  = big_int_access::limbs(divisor);

    // D1: Normalize - shift divisor so leading limb >= BASE/2
    const unsigned shift = calculate_normalization_shift(divisor_limbs[n - 1]);

    // Allocate normalized operands
    std::vector<limb_t> u(m + 1, 0); // Normalized dividend (extra limb for overflow)
    std::vector<limb_t> v(n, 0);     // Normalized divisor

    // Normalize dividend
    std::ranges::copy_n(dividend_limbs.begin(), m, u.begin());
    u[m] = shift_left(u, m, shift);

    // Normalize divisor
    std::ranges::copy_n(divisor_limbs.begin(), n, v.begin());
    shift_left(v, n, shift);

    // Allocate quotient
    std::vector<limb_t> q(m - n + 1, 0);

    const limb_t v1 = v[n - 1];
    const limb_t v2 = (n > 1) ? v[n - 2] : 0;

    // D2-D7: Main division loop
    for (size_t j = m - n + 1; j-- > 0;) {
        // D3: Calculate trial quotient digit
        const dlimb_t u_high = (dlimb_t{u[j + n]} << LIMB_BITS) | u[j + n - 1];
        dlimb_t       qhat   = u_high / v1;
        dlimb_t       rhat   = u_high % v1;

        // Refine qhat to ensure it doesn't overshoot
        while (qhat >= BASE || (qhat * v2 > (rhat << LIMB_BITS) + u[j + n - 2])) {
            qhat--;
            rhat += v1;
            if (rhat >= BASE)
                break;
        }

        // D4: Multiply and subtract (qhat * divisor from dividend)
        dlimb_t carry  = 0;
        dlimb_t borrow = 0;

        for (size_t i = 0; i < n; ++i) {
            const dlimb_t prod    = qhat * v[i] + carry;
            carry                 = prod >> LIMB_BITS;
            const limb_t prod_low = static_cast<limb_t>(prod);

            const dlimb_t diff = dlimb_t{u[j + i]} - prod_low - borrow;
            u[j + i]           = static_cast<limb_t>(diff);
            borrow             = (diff >> LIMB_BITS) & 1;
        }

        const dlimb_t final_diff = dlimb_t{u[j + n]} - carry - borrow;
        u[j + n]                 = static_cast<limb_t>(final_diff);

        // D5: Test remainder - store quotient digit
        q[j] = static_cast<limb_t>(qhat);

        // D6: Add back if we overshot (negative result)
        if ((final_diff >> LIMB_BITS) & 1) {
            q[j]--;
            dlimb_t carry_back = 0;
            for (size_t i = 0; i < n; ++i) {
                const dlimb_t sum = dlimb_t{u[j + i]} + v[i] + carry_back;
                u[j + i]          = static_cast<limb_t>(sum);
                carry_back        = sum >> LIMB_BITS;
            }
            u[j + n] += static_cast<limb_t>(carry_back);
        }
    }

    // D8: Unnormalize remainder
    u.resize(n);
    shift_right(u, n, shift);

    // Build results
    big_int quotient  = big_int_access::create(false, std::move(q));
    big_int remainder = big_int_access::create(false, std::move(u));

    return DivisionResult{std::move(quotient), std::move(remainder)};
}

// ============================================================================
// Public Division API
// ============================================================================

[[nodiscard]] std::expected<DivisionResult, errors::ArithmeticError> div_mod_abs(
  const big_int &dividend,
  const big_int &divisor
) noexcept {
    const size_t m = big_int_access::length(dividend);
    const size_t n = big_int_access::length(divisor);

    // Check for division by zero
    if (divisor.is_zero()) {
        return std::unexpected(errors::ArithmeticError::DivisionByZero);
    }

    // Early return: dividend < divisor
    if (m < n) {
        return DivisionResult{big_int(), dividend};
    }

    // Early return: dividend == divisor
    if (m == n && dividend == divisor) {
        return DivisionResult{big_int::from_integer(1), big_int()};
    }

    // Fast path: single-limb divisor
    if (n == 1) {
        return div_single_limb(dividend, big_int_access::limbs(divisor)[0]);
    }

    // General case: multi-limb division
    return div_multi_limb(dividend, divisor);
}

[[nodiscard]] std::expected<big_int, errors::ArithmeticError> div_abs(
  const big_int &dividend,
  const big_int &divisor
) noexcept {
    auto result = div_mod_abs(dividend, divisor);
    if (!result) {
        return std::unexpected(result.error());
    }
    return std::move(result->quotient);
}

[[nodiscard]] std::expected<big_int, errors::ArithmeticError> mod_abs(
  const big_int &dividend,
  const big_int &divisor
) noexcept {
    auto result = div_mod_abs(dividend, divisor);
    if (!result) {
        return std::unexpected(result.error());
    }
    return std::move(result->remainder);
}

} // namespace arbys::bignumbers::detail
