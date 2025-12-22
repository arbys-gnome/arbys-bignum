#ifndef BIGNUMBERS_ERRORS_H
#define BIGNUMBERS_ERRORS_H

#include <stdexcept>
#include <string>

namespace arbys::bignumbers {
    enum class ParseError {
        EmptyInput,
        InvalidCharacter,
        InvalidSeparator,
        NoDigits,
    };

    struct parse_error : public std::invalid_argument {
        explicit parse_error(const std::string& msg)
            : std::invalid_argument(msg) {}
    };

    struct divide_by_zero_error : public std::runtime_error {
        divide_by_zero_error() : std::runtime_error("Division by zero") {}
    };

}
#endif //BIGNUMBERS_ERRORS_H