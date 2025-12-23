#include <expected>

#include "arbys/bignumbers/bigint.h"
#include "arbys/bignumbers/errors.h"
#include "../detail/detail.h"

namespace arbys::bignumbers {
    std::expected<BigInt, ParseError>
    BigInt::from_string(std::string_view input) {
        input = detail::trim_view(input);
        if (input.empty())
            return std::unexpected(ParseError::EmptyInput);

        bool isNegative = false;
        if (input.front() == '-') {
            isNegative = true;
            input.remove_prefix(1);
        }

        if (input.empty())
            return std::unexpected(ParseError::NoDigits);

        return detail::parse_digits(input, isNegative);
    }

    std::expected<BigInt, ParseError>
    BigInt::from_string(std::string_view input, char separator) {
        input = detail::trim_view(input);
        if (input.empty())
            return std::unexpected(ParseError::EmptyInput);

        bool isNegative = false;
        if (input.front() == '-') {
            isNegative = true;
            input.remove_prefix(1);
        }

        std::string compact;
        compact.reserve(input.size());

        for (char c : input) {
            if (c == separator)
                continue;
            compact.push_back(c);
        }

        if (compact.empty())
            return std::unexpected(ParseError::NoDigits);

        return detail::parse_digits(compact, isNegative);
    }

    std::expected<BigInt, ParseError>
    BigInt::from_string(std::string_view input, std::string_view separator) {
        if (separator.empty())
            return from_string(input);

        input = detail::trim_view(input);
        if (input.empty())
            return std::unexpected(ParseError::EmptyInput);

        bool isNegative = false;
        if (input.front() == '-') {
            isNegative = true;
            input.remove_prefix(1);
        }

        std::string compact;
        compact.reserve(input.size());

        while (!input.empty()) {
            auto pos = input.find(separator);
            auto chunk = input.substr(0, pos);

            for (char c : chunk) {
                if (!std::isdigit(static_cast<unsigned char>(c)))
                    return std::unexpected(ParseError::InvalidCharacter);
                compact.push_back(c);
            }

            if (pos == std::string_view::npos)
                break;

            input.remove_prefix(pos + separator.size());
        }

        if (compact.empty())
            return std::unexpected(ParseError::NoDigits);

        return detail::parse_digits(compact, isNegative);
    }

}
