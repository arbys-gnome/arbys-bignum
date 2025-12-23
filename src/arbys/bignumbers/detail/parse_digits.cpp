#include <algorithm>
#include <expected>
#include <locale>
#include <string_view>
#include <vector>

#include "bigint_impl.h"
#include "arbys/bignumbers/bigint.h"
#include "arbys/bignumbers/errors.h"

namespace arbys::bignumbers::detail {

    std::expected<BigInt, ParseError>
    parse_digits(std::string_view input, bool isNegative) {
        std::vector<limb_t> digits;
        digits.reserve(input.size());

        bool seen_non_zero = false;

        for (const char c : input) {
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return std::unexpected(ParseError::InvalidCharacter);

            if (c != '0')
                seen_non_zero = true;

            if (seen_non_zero)
                digits.push_back(c - '0');
        }

        if (digits.empty()) {
            digits.push_back(0);
            isNegative = false;
        }

        std::ranges::reverse(digits);
        return BigIntImpl::create(isNegative, digits, digits.size());
    }
}
