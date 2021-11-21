#pragma once

#include "Error.h"
#include "Token.h"

#include <string>

namespace ferrit {
    /**
     * Represents a failure to parse the user's input as a syntactically-valid
     * program. This class should not be used to report bugs in the compiler.
     */
    class ParseError : public Error {
    public:
        class UnexpectedChar;
        class UnterminatedElement;
        class EmptyElement;
        class CharLiteralTooBig;
        class UnexpectedNewline;
        class IllegalEscapeSequence;
        class UnknownLiteralSuffix;
        class ExpectedElementNotPresent;

    protected:
        using Error::Error;
    };

    /**
     * Represents a syntax error in which an unexpected character was present.
     */
    class ParseError::UnexpectedChar final : public ParseError {
    public:
        UnexpectedChar(Token cause, char ch) noexcept;
        FERRIT_ERROR_PRETTY_NAME("unexpected-char");
    };

    /**
     * Represents a syntax error in which an element (such as a string literal) was not terminated.
     */
    class ParseError::UnterminatedElement final : public ParseError {
    public:
        UnterminatedElement(Token cause, const std::string &element) noexcept;
        FERRIT_ERROR_PRETTY_NAME("unterminated-element");
    };

    /**
     * Represents a syntax error in which an element (such as a char literal) was empty.
     */
    class ParseError::EmptyElement final : public ParseError {
    public:
        EmptyElement(Token cause, const std::string &element) noexcept;
        FERRIT_ERROR_PRETTY_NAME("empty-element");
    };

    /**
     * Represents a syntax error in which a char literal contained more than one character.
     */
    class ParseError::CharLiteralTooBig final : public ParseError {
    public:
        explicit CharLiteralTooBig(Token cause) noexcept;
        FERRIT_ERROR_PRETTY_NAME("char-literal-too-big");
    };

    /**
     * Represents a syntax error in which a newline was unexpectedly present in an element
     * (such as a string literal).
     */
    class ParseError::UnexpectedNewline final : public ParseError {
    public:
        UnexpectedNewline(Token cause, const std::string &element) noexcept;
        FERRIT_ERROR_PRETTY_NAME("unexpected-newline");
    };

    /**
     * Represents a syntax error in which a string or char literal contained an unexpected escape sequence.
     */
    class ParseError::IllegalEscapeSequence final : public ParseError {
    public:
        IllegalEscapeSequence(Token cause, char sequence, const std::string &element) noexcept;
        FERRIT_ERROR_PRETTY_NAME("illegal-escape-sequence");
    };

    /**
     * Represents a syntax error in which a literal (such as an int or float literal) had an unknown suffix.
     */
    class ParseError::UnknownLiteralSuffix final : public ParseError {
    public:
        UnknownLiteralSuffix(Token cause, const std::string &element, const std::string &suffix) noexcept;
        FERRIT_ERROR_PRETTY_NAME("unknown-literal-suffix");
    };

    /**
     * Represents a parse error in which the expected element
     * was not found.
     */
    class ParseError::ExpectedElementNotPresent final : public ParseError {
    public:
        ExpectedElementNotPresent(Token cause, const std::string &expected) noexcept;
        FERRIT_ERROR_PRETTY_NAME("parse-error");
    };


}