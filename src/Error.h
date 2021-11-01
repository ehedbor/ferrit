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

    class Error::InvalidLiteral final : public Error {
    public:
        InvalidLiteral(Token cause, std::vector<Token> stackTrace, const std::string &literalKind) noexcept;
        PRETTY_NAME_IMPL("invalid-literal");
    };

    class Error::UnknownVariable final : public Error {
    public:
        UnknownVariable(Token cause, std::vector<Token> stackTrace, const std::string &varName) noexcept;
        PRETTY_NAME_IMPL("unknown-variable");
    };

    class Error::UnknownType final : public Error {
    public:
        UnknownType(Token cause, std::vector<Token> stackTrace, const std::string &typeName) noexcept;
        PRETTY_NAME_IMPL("unknown-variable");
    };

    class Error::InvalidState final : public Error {
    public:
        InvalidState(Token cause, std::vector<Token> stackTrace, const std::string &explanation) noexcept;
        PRETTY_NAME_IMPL("invalid-state");
    };

    class Error::InvalidOperands final : public Error {
    public:
        template <typename Arg, typename... Args>
        InvalidOperands(
            Token cause,
            std::vector<Token> stackTrace,
            const std::string &op,
            Arg &&arg,
            Args &&...args) noexcept;
        PRETTY_NAME_IMPL("invalid-arguments");

    private:
        template <typename Arg>
        static std::string formatArgs(Arg &&arg);

        template <typename Arg, typename... Args>
        static std::string formatArgs(Arg &&arg, Args &&...args);
    };

    class Error::BadFunctionCall final : public Error {
    public:
        BadFunctionCall(
            Token cause,
            std::vector<Token> stackTrace,
            const std::string &explanation) noexcept;
        PRETTY_NAME_IMPL("bad-function-call");
    };

    class Error::BadModifier final : public Error {
    public:
        BadModifier(
            Token cause,
            std::vector<Token> stackTrace,
            const Token &modifier,
            const std::string &explanation) noexcept;
        PRETTY_NAME_IMPL("bad-modifier");
    };

    class Error::Redefinition final : public Error {
    public:
        Redefinition(
            Token cause,
            std::vector<Token> stackTrace,
            const std::string &element) noexcept;
        PRETTY_NAME_IMPL("redefinition");
    };

#undef PRETTY_NAME_IMPL

    template <typename Arg, typename... Args>
    Error::InvalidOperands::InvalidOperands(
        Token cause,
        std::vector<Token> stackTrace,
        const std::string &op,
        Arg &&arg,
        Args &&...args) noexcept :
        Error(std::move(cause), std::move(stackTrace),
            std::format("error: invalid arguments to {}: {}", op,
                formatArgs(std::forward<Arg>(arg), std::forward<Args>(args)...))) {
    }

    template <typename Arg>
    std::string Error::InvalidOperands::formatArgs(Arg &&arg) {
        return std::format("'{}'", std::forward<Arg>(arg));
    }

    template <typename Arg, typename... Args>
    std::string Error::InvalidOperands::formatArgs(Arg &&arg, Args &&...args) {
        return std::format("{}, {}",
            formatArgs(std::forward<Arg>(arg)),
            formatArgs(std::forward<Args>(args)...));
    }
}
