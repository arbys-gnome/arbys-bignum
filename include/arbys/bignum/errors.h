#pragma once

#include <stdexcept>
#include <string>

namespace arbys::bignum::errors {
// TODO: write documentation

/// Errors that can occur during parsing
enum class ParseError {
    EmptyInput,
    InvalidCharacter,
    InvalidSeparator,
    NoDigits,
    InvalidBase,
};

/// Errors that can occur during arithmetic operations
enum class ArithmeticError {
    DivisionByZero,
    Overflow,
    Underflow,
    InvalidOperation,
};

/// Convert ParseError to string
[[nodiscard]] constexpr std::string_view to_string(const ParseError error) noexcept {
    switch (error) {
    case ParseError::EmptyInput:
        return "Empty input string";
    case ParseError::InvalidCharacter:
        return "Invalid character in input";
    case ParseError::InvalidSeparator:
        return "Invalid separator in input";
    case ParseError::NoDigits:
        return "No digits found";
    case ParseError::InvalidBase:
        return "Invalid base";
    }
    return "Unknown parse error";
}

/// Convert ArithmeticError to string
[[nodiscard]] constexpr std::string_view to_string(const ArithmeticError error) noexcept {
    switch (error) {
    case ArithmeticError::DivisionByZero:
        return "Division by zero";
    case ArithmeticError::Overflow:
        return "Arithmetic overflow";
    case ArithmeticError::Underflow:
        return "Arithmetic underflow";
    case ArithmeticError::InvalidOperation:
        return "Invalid operation";
    }
    return "Unknown arithmetic error";
}

struct parse_error : public std::invalid_argument {
    explicit parse_error(const std::string &msg) : std::invalid_argument(msg) {}
};

struct divide_by_zero_error : public std::runtime_error {
    divide_by_zero_error() : std::runtime_error("Division by zero") {}
};

} // namespace arbys::bignum::errors