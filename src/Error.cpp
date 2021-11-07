#include "Error.h"
#include <format>
#include <unordered_map>
#include <typeinfo>

namespace ferrit {
    Error::Error(Token cause, const std::string &what) noexcept :
        std::runtime_error(what), m_cause(std::move(cause)) {
    }

    const Token &Error::cause() const noexcept {
        return m_cause;
    }

    std::string Error::longMessage() const {
        std::string result{what()};
        result += "\n";
        result += std::format("    at {}\n", formatToken(cause()));
        return result;
    }

    std::string Error::formatToken(const Token &token) {
        return std::format("{}:{}: {} \"{}\"",
            token.location.line,
            token.location.column,
            token.type,
            token.lexeme);
    }

    Error::UnexpectedChar::UnexpectedChar(Token cause, char ch) noexcept :
        Error(std::move(cause), std::format("syntax error: unexpected character '{}'", ch)) {
    }

    Error::UnterminatedElement::UnterminatedElement(Token cause, const std::string &element) noexcept :
        Error(std::move(cause), std::format("syntax error: unterminated {}", element)) {
    }

    Error::EmptyElement::EmptyElement(Token cause, const std::string &element) noexcept :
        Error(std::move(cause), std::format("syntax error: empty {}", element)) {
    }

    Error::CharLiteralTooBig::CharLiteralTooBig(Token cause) noexcept :
        Error(std::move(cause), "syntax error: too many characters in char literal") {
    }

    Error::UnexpectedNewline::UnexpectedNewline(
        Token cause,
        const std::string &element) noexcept :
        Error(std::move(cause), 
            std::format("syntax error: unexpected newline in {}", element)) {
    }

    Error::IllegalEscapeSequence::IllegalEscapeSequence(
        Token cause,
        char sequence,
        const std::string &element) noexcept :
        Error(std::move(cause), 
            std::format("syntax error: illegal escape sequence '\\{}' in {}", sequence, element)) {
    }

    Error::UnknownLiteralSuffix::UnknownLiteralSuffix(
        Token cause,
        const std::string &element,
        const std::string &suffix) noexcept :
        Error(std::move(cause), 
            std::format("syntax error: unknown suffix '{1}' for {0}", element, suffix)) {
    }

    Error::ParseError::ParseError(
        Token cause,
        const std::string &expected) noexcept :
        Error(std::move(cause), 
            std::format("syntax error: {}", expected)) {
    }
}