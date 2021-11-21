#include "ParseError.h"

namespace ferrit {
    ParseError::ParseError(Token cause, const std::string &what) noexcept :
        Error(std::move(cause), what) {
    }

    ParseError::UnexpectedChar::UnexpectedChar(
        Token cause, char ch) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: unexpected character '{}'", ch)) {
    }

    ParseError::UnterminatedElement::UnterminatedElement(
        Token cause, const std::string &element) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: unterminated {}", element)) {
    }

    ParseError::EmptyElement::EmptyElement(
        Token cause, const std::string &element) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: empty {}", element)) {
    }

    ParseError::CharLiteralTooBig::CharLiteralTooBig(
        Token cause) noexcept :
        ParseError(std::move(cause),
            "syntax error: too many characters in char literal") {
    }

    ParseError::UnexpectedNewline::UnexpectedNewline(
        Token cause, const std::string &element) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: unexpected newline in {}", element)) {
    }

    ParseError::IllegalEscapeSequence::IllegalEscapeSequence(
        Token cause, char sequence, const std::string &element) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: illegal escape sequence '\\{}' in {}", sequence, element)) {
    }

    ParseError::UnknownLiteralSuffix::UnknownLiteralSuffix(
        Token cause, const std::string &element, const std::string &suffix) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: unknown suffix '{1}' for {0}", element, suffix)) {
    }

    ParseError::ExpectedElement::ExpectedElement(
        Token cause, const std::string &expected) noexcept :
        ParseError(std::move(cause),
            std::format("syntax error: {}", expected)) {
    }
}