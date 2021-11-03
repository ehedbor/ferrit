#pragma once

#include <concepts>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include "Token.h"

namespace ferrit {
    enum class WarningLevel {
        Ignored,
        Warning,
        Error
    };

    class Error {
    public:
        class UnexpectedChar;
        class UnterminatedElement;
        class EmptyElement;
        class CharLiteralTooBig;
        class UnexpectedNewline;
        class IllegalEscapeSequence;
        class UnknownLiteralSuffix;
        class ParseError;

    protected:
        Error(Token cause, std::vector<Token> stackTrace, std::string shortMessage) noexcept;

    public:
        virtual ~Error() noexcept = default;

    public:
        [[nodiscard]] const Token &cause() const noexcept;
        [[nodiscard]] const std::vector<Token> &stackTrace() const noexcept;
        [[nodiscard]] const std::string &shortMessage() const noexcept;
        [[nodiscard]] std::string message() const;

    private:
        static std::string formatToken(const Token &token);

    private:
        Token m_cause;
        std::vector<Token> m_stackTrace;
        std::string m_shortMessage;
    };


#define PRETTY_NAME_IMPL(name)                                                 \
    [[nodiscard]] static constexpr const char *prettyName() noexcept {         \
        return name;                                                           \
    } static_assert(true)

    class Error::UnexpectedChar final : public Error {
    public:
        UnexpectedChar(Token cause, std::vector<Token> stackTrace, char ch) noexcept;
        PRETTY_NAME_IMPL("unexpected-char");
    };

    class Error::UnterminatedElement final : public Error {
    public:
        UnterminatedElement(Token cause, std::vector<Token> stackTrace, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("unterminated-element");
    };

    class Error::EmptyElement final : public Error {
    public:
        EmptyElement(Token cause, std::vector<Token> stackTrace, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("empty-element");
    };

    class Error::CharLiteralTooBig final : public Error {
    public:
        CharLiteralTooBig(Token cause, std::vector<Token> stackTrace) noexcept;
        PRETTY_NAME_IMPL("char-literal-too-big");
    };

    class Error::UnexpectedNewline final : public Error {
    public:
        UnexpectedNewline(Token cause, std::vector<Token> stackTrace, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("unexpected-newline");
    };

    class Error::IllegalEscapeSequence final : public Error {
    public:
        IllegalEscapeSequence(
            Token cause, std::vector<Token> stackTrace,
            char sequence,
            const std::string &element) noexcept;
        PRETTY_NAME_IMPL("illegal-escape-sequence");
    };

    class Error::UnknownLiteralSuffix final : public Error {
    public:
        UnknownLiteralSuffix(
            Token cause, std::vector<Token> stackTrace,
            const std::string &element,
            const std::string &suffix) noexcept;
        PRETTY_NAME_IMPL("unknown-literal-suffix");
    };

    class Error::ParseError final : public Error {
    public:
        ParseError(Token cause, std::vector<Token> stackTrace, const std::string &expected) noexcept;
        PRETTY_NAME_IMPL("parse-error");
    };
}
