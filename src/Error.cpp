#include "Error.h"
#include <iostream>
#include <format>
#include <unordered_map>
#include <typeinfo>

namespace ferrit {
    Error::Error(Token cause, std::vector<Token> stackTrace, std::string shortMessage) noexcept :
        m_cause(std::move(cause)), m_stackTrace(std::move(stackTrace)), m_shortMessage(std::move(shortMessage)) {

    }

    const Token &Error::cause() const noexcept {
        return m_cause;
    }

    const std::vector<Token> &Error::stackTrace() const noexcept {
        return m_stackTrace;
    }

    const std::string &Error::shortMessage() const noexcept {
        return m_shortMessage;
    }

    std::string Error::message() const {
        std::string result = shortMessage() + "\n";
        result += std::format("    at {}\n", formatToken(cause()));
        for (const auto &token : stackTrace()) {
            result += std::format("    at {}\n", formatToken(token));
        }
        return result;
    }

    std::string Error::formatToken(const Token &token) {
        return std::format("{}:{}: {} \"{}\"",
            token.location.line,
            token.location.column,
            token.type,
            token.lexeme);
    }

    void logError(const Error &error) {
        std::cout << "error: " << error.message() << std::endl;
    }

    void logWarning(const Error &error) {
        std::cout << "warning: " << error.message() << std::endl;
    }

    Error::UnexpectedChar::UnexpectedChar(
        Token cause, std::vector<Token> stackTrace, char ch) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("syntax error: unexpected character '{}'", ch)) {
    }

    Error::UnterminatedElement::UnterminatedElement(
        Token cause, std::vector<Token> stackTrace, const std::string &element) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("syntax error: unterminated {}", element)) {
    }

    Error::EmptyElement::EmptyElement(
        Token cause, std::vector<Token> stackTrace, const std::string &element) noexcept :
        Error(std::move(cause), std::move(stackTrace), std::format("syntax error: empty {}", element)) {
    }

    Error::CharLiteralTooBig::CharLiteralTooBig(
        Token cause,
        std::vector<Token> stackTrace) noexcept :
        Error(std::move(cause),
            std::move(stackTrace),
            "syntax error: too many characters in char literal") {
    }

    Error::UnexpectedNewline::UnexpectedNewline(
        Token cause,
        std::vector<Token> stackTrace,
        const std::string &element) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("syntax error: unexpected newline in {}", element)) {
    }

    Error::IllegalEscapeSequence::IllegalEscapeSequence(
        Token cause,
        std::vector<Token> stackTrace,
        char sequence,
        const std::string &element) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("syntax error: illegal escape sequence '\\{}' in {}", sequence, element)) {
    }

    Error::UnknownLiteralSuffix::UnknownLiteralSuffix(
        Token cause,
        std::vector<Token> stackTrace,
        const std::string &element,
        const std::string &suffix) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("syntax error: unknown suffix '{1}' for {0}", element, suffix)) {
    }

    Error::ParseError::ParseError(
        Token cause,
        std::vector<Token> stackTrace,
        const std::string &expected) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("syntax error: {}", expected)) {
    }
}