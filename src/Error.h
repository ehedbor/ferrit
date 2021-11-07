#pragma once

#include "Token.h"

#include <concepts>
#include <string>
#include <stdexcept>
#include <vector>


namespace ferrit {

    /**
     * Class to represent compile-time errors in the user's program.
     * This includes syntax errors, parsing errors, and compile errors.
     *
     * Runtime errors are not represented with this class.
     */
    class Error : public std::runtime_error {
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
        /**
         * Construct a new Error.
         *
         * @param cause the location of the error
         * @param what a formatted message containing information about the error
         */
        Error(Token cause, const std::string &what) noexcept;

    public:
        /**
         * Returns the token at which this error occurred.
         */
        [[nodiscard]] const Token &cause() const noexcept;

        /**
         * Returns a message describing this error.
         *
         * This includes the content of the short message plus additional information.
         */
        [[nodiscard]] std::string longMessage() const;

    private:
        static std::string formatToken(const Token &token);

    private:
        Token m_cause;
    };

/**
 * Generates a static function to return a user-friendly name for this error.
 */
#define PRETTY_NAME_IMPL(name)                                                 \
    [[nodiscard]] static constexpr const char *prettyName() noexcept {         \
        return name;                                                           \
    } static_assert(true)

    /**
     * Represents a syntax error in which an unexpected character was present.
     */
    class Error::UnexpectedChar final : public Error {
    public:
        UnexpectedChar(Token cause, char ch) noexcept;

        PRETTY_NAME_IMPL("unexpected-char");
    };

    /**
     * Represents a syntax error in which an element (such as a string literal) was not terminated.
     */
    class Error::UnterminatedElement final : public Error {
    public:
        UnterminatedElement(Token cause, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("unterminated-element");
    };

    /**
     * Represents a syntax error in which an element (such as a char literal) was empty.
     */
    class Error::EmptyElement final : public Error {
    public:
        EmptyElement(Token cause, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("empty-element");
    };

    /**
     * Represents a syntax error in which a char literal contained more than one character.
     */
    class Error::CharLiteralTooBig final : public Error {
    public:
        explicit CharLiteralTooBig(Token cause) noexcept;
        PRETTY_NAME_IMPL("char-literal-too-big");
    };

    /**
     * Represents a syntax error in which a newline was unexpectedly present in an element
     * (such as a string literal).
     */
    class Error::UnexpectedNewline final : public Error {
    public:
        UnexpectedNewline(Token cause, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("unexpected-newline");
    };

    /**
     * Represents a syntax error in which a string or char literal contained an unexpected escape sequence.
     */
    class Error::IllegalEscapeSequence final : public Error {
    public:
        IllegalEscapeSequence(Token cause, char sequence, const std::string &element) noexcept;
        PRETTY_NAME_IMPL("illegal-escape-sequence");
    };

    /**
     * Represents a syntax error in which a literal (such as an int or float literal) had an unknown suffix.
     */
    class Error::UnknownLiteralSuffix final : public Error {
    public:
        UnknownLiteralSuffix(Token cause, const std::string &element, const std::string &suffix) noexcept;
        PRETTY_NAME_IMPL("unknown-literal-suffix");
    };

    /**
     * Represents a parser error.
     */
    class Error::ParseError final : public Error {
    public:
        ParseError(Token cause, const std::string &expected) noexcept;
        PRETTY_NAME_IMPL("parse-error");
    };

#undef PRETTY_NAME_IMPL
}
