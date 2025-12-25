#include <algorithm>
#include <expected>
#include <locale>
#include <ranges>
#include <string_view>
#include <vector>

#include "big_int_internal.h"
#include "arbys/bignumbers/big_int.h"
#include "arbys/bignumbers/errors.h"

#include <algorithm>
#include <cctype>
#include <expected>
#include <string_view>

namespace arbys::bignumbers::detail {

    /// Convert decimal string to base 2^32 representation
    /// Algorithm: Repeatedly multiply accumulator by 10 and add next digit
    [[nodiscard]] std::expected<big_int, errors::ParseError>
    parse_limbs(std::string_view input, const bool is_negative) noexcept {
        // Skip leading zeros
        while (!input.empty() && input.front() == '0') {
            input.remove_prefix(1);
        }

        // Handle zero case
        if (input.empty()) {
            return big_int_access::create(false, std::vector<limb_t>{0});
        }

        // Validate all characters are digits
        for (char c : input) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                return std::unexpected(errors::ParseError::InvalidCharacter);
            }
        }

        // Start with zero
        std::vector<limb_t> limbs{0};

        // Process each decimal digit
        for (const char c : input) {
            const auto digit = static_cast<limb_t>(c - '0');

            // Multiply current number by 10
            dlimb_t carry = 0;
            for (limb_t &limb : limbs) {
                const dlimb_t product = dlimb_t{limb} * 10 + carry;
                limb = static_cast<limb_t>(product);
                carry = product >> LIMB_BITS;
            }

            // Add carry if multiplication overflowed
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }

            // Add current digit
            carry = digit;
            for (size_t i = 0; i < limbs.size() && carry > 0; ++i) {
                const dlimb_t sum = dlimb_t{limbs[i]} + carry;
                limbs[i] = static_cast<limb_t>(sum);
                carry = sum >> LIMB_BITS;
            }

            // Add carry if addition overflowed
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }
        }

        return big_int_access::create(is_negative, std::move(limbs));
    }

    /// Parse decimal chunks for better performance
    /// Process multiple digits at once to reduce limb operations
    [[nodiscard]] std::expected<big_int, errors::ParseError>
    parse_limbs_optimized(std::string_view input, const bool is_negative) noexcept {
        // Skip leading zeros
        while (!input.empty() && input.front() == '0') {
            input.remove_prefix(1);
        }

        // Handle zero case
        if (input.empty()) {
            return big_int_access::create(false, std::vector<limb_t>{0});
        }

        // Validate all characters are digits
        for (char c : input) {
            if (!std::isdigit(static_cast<unsigned char>(c))) {
                return std::unexpected(errors::ParseError::InvalidCharacter);
            }
        }

        // Process 9 digits at a time (10^9 < 2^32)
        constexpr size_t chunk_size = 9;

        std::vector<limb_t> limbs{0};

        // Process full chunks from the end
        size_t pos = input.size();
        while (pos >= chunk_size) {
            pos -= chunk_size;
            std::string_view chunk = input.substr(pos, chunk_size);

            // Convert chunk to integer
            limb_t chunk_value = 0;
            for (const char ch : chunk) {
                chunk_value = chunk_value * 10 + (ch - '0');
            }

            // Multiply existing limbs by 10^9
            dlimb_t carry = 0;
            for (limb_t &limb : limbs) {
                constexpr limb_t chunk_multiplier = 1'000'000'000;
                const dlimb_t product = dlimb_t{limb} * chunk_multiplier + carry;
                limb = static_cast<limb_t>(product);
                carry = product >> LIMB_BITS;
            }
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }

            // Add chunk value
            carry = chunk_value;
            for (size_t i = 0; i < limbs.size() && carry > 0; ++i) {
                const dlimb_t sum = dlimb_t{limbs[i]} + carry;
                limbs[i] = static_cast<limb_t>(sum);
                carry = sum >> LIMB_BITS;
            }
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }
        }

        // Process remaining digits (less than chunk_size)
        if (pos > 0) {
            const std::string_view remaining = input.substr(0, pos);
            limb_t remaining_value = 0;
            limb_t multiplier = 1;

            for (const char it : std::ranges::reverse_view(remaining)) {
                remaining_value += (it - '0') * multiplier;
                multiplier *= 10;
            }

            // Multiply by appropriate power of 10
            dlimb_t carry = 0;
            for (limb_t &limb : limbs) {
                const dlimb_t product = dlimb_t{limb} * multiplier + carry;
                limb = static_cast<limb_t>(product);
                carry = product >> LIMB_BITS;
            }
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }

            // Add remaining value
            carry = remaining_value;
            for (size_t i = 0; i < limbs.size() && carry > 0; ++i) {
                const dlimb_t sum = dlimb_t{limbs[i]} + carry;
                limbs[i] = static_cast<limb_t>(sum);
                carry = sum >> LIMB_BITS;
            }
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }
        }

        return big_int_access::create(is_negative, std::move(limbs));
    }

    /// Convert string in given base to BigInt
    [[nodiscard]] std::expected<big_int, errors::ParseError>
    parse_limbs_base(std::string_view input, const bool is_negative, const unsigned base) noexcept {
        if (base < 2 || base > 36) {
            return std::unexpected(errors::ParseError::InvalidBase);
        }

        // For base 10, use optimized version
        if (base == 10) {
            return parse_limbs_optimized(input, is_negative);
        }

        // Skip leading zeros
        while (!input.empty() && input.front() == '0') {
            input.remove_prefix(1);
        }

        if (input.empty()) {
            return big_int_access::create(false, std::vector<limb_t>{0});
        }

        std::vector<limb_t> limbs{0};

        for (const char c : input) {
            limb_t digit;

            // Convert character to digit value
            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else if (c >= 'a' && c <= 'z') {
                digit = c - 'a' + 10;
            } else if (c >= 'A' && c <= 'Z') {
                digit = c - 'A' + 10;
            } else {
                return std::unexpected(errors::ParseError::InvalidCharacter);
            }

            if (digit >= base) {
                return std::unexpected(errors::ParseError::InvalidCharacter);
            }

            // Multiply current number by base
            dlimb_t carry = 0;
            for (limb_t &limb : limbs) {
                const dlimb_t product = dlimb_t{limb} * base + carry;
                limb = static_cast<limb_t>(product);
                carry = product >> LIMB_BITS;
            }
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }

            // Add digit
            carry = digit;
            for (size_t i = 0; i < limbs.size() && carry > 0; ++i) {
                const dlimb_t sum = dlimb_t{limbs[i]} + carry;
                limbs[i] = static_cast<limb_t>(sum);
                carry = sum >> LIMB_BITS;
            }
            if (carry > 0) {
                limbs.push_back(static_cast<limb_t>(carry));
            }
        }

        return big_int_access::create(is_negative, std::move(limbs));
    }

} // namespace arbys::bignumbers::detail
