#pragma once

#include <ostream>
#include <optional>
#include <string>
#include <vector>
#include <result.hpp>
#include "Token.h"
#include "Error.h"


namespace es {
    class LexError;
    using LexResult = cpp::result<Token, LexError>;

    /**
     * Converts source code into a list of tokens.
     */
    class Lexer {
    public:
        explicit Lexer(std::string code) noexcept;

        /**
         * Scans the next token from the source code.
         */
        [[nodiscard]] LexResult lex() noexcept;

    private:
        /**
         * Skips ASCII whitespace characters up until the next newline.
         *
         * @return \c LexError if something goes wrong, a newline \c Token if one is found, or \c std::nullopt otherwise.
         */
        [[nodiscard]] cpp::result<std::optional<Token>, LexError> skipWhitespace() noexcept;

        /**
         * Ignores a line comment (starting with // and ending at the next newline).
         */
        void ignoreLineComment() noexcept;

        /**
         * Ignores a block comment (starting with \c /&#42; and ending at \c &#42;/)
         *
         * @return nothing on success, \c LexError if the block comment is not terminated.
         */
        [[nodiscard]] cpp::result<void, LexError> ignoreBlockComment() noexcept;

        /**
         * Scans and returns a string literal.
         */
        [[nodiscard]] LexResult lexString() noexcept;

        /**
         * Scans and returns a char literal.
         */
        [[nodiscard]] LexResult lexChar() noexcept;

        /**
         * Advances the lexer, returning an error if the next char cannot
         * appear in a string-like literal (such as a \c char or \c string).
         *
         * @param literalType the type of literal (i.e. <tt>"string literal"</tt>)
         * @return nothing on success, \c LexError on failure.
         */
        [[nodiscard]] cpp::result<void, LexError> advanceStringChar(const std::string &literalType) noexcept;

        [[nodiscard]] LexResult lexNumber() noexcept;
        [[nodiscard]] LexResult lexIdentifier() noexcept;
        [[nodiscard]] static TokenType getTokenTypeForIdent(const std::string &lexeme) noexcept;

        [[nodiscard]] Token makeToken(TokenType type) const noexcept;
        [[nodiscard]] LexError makeError(const std::string &msg) const noexcept;

        /**
         * Gets the current character without advancing the \c Lexer.
         *
         * @return the current \c char, or \c std::nullopt if no \c char is being pointed to.
         */
        [[nodiscard]] std::optional<char> peek() const noexcept;

        /**
         * Gets the next character without advancing the \c Lexer.
         *
         * @return the next \c char, or \c std::nullopt if no \c char is being pointed to.
         */
        [[nodiscard]] std::optional<char> peekNext() const noexcept;

        /**
         * Consumes and returns the currently-referenced character.
         */
        std::optional<char> advance() noexcept;

        /**
         * Checks to see if the current character is the same as the given character,
         * and advances the \c Lexer if they are.
         */
        [[nodiscard]] bool match(char expected) noexcept;

        /**
         * Returns true if the \c Lexer has reached the end of the source code.
         */
        [[nodiscard]] bool isAtEnd() const noexcept;

        /**
         * Returns the type of newline located at \c current()
         * @return 0 if no newline, 1 for \c '\\n', 2 for \c "\\r\\n"
         */
        [[nodiscard]] int getCurrentNewlineType() const noexcept;

        /**
         * Returns \c true if the given character is an ASCII digit.
         */
        [[nodiscard]] static bool isDigit(char ch) noexcept;

        /**
         * Returns \c true if the given character is an ASCII letter or underscore.
         */
        [[nodiscard]] static bool isIdentifier(char ch) noexcept;

        /**
         * Returns \c true if the given character is an ASCII letter, ASCII digit or underscore.
         */
        [[nodiscard]] static bool isIdentifierStart(char ch) noexcept;

    private:
        std::string m_code;
        std::size_t m_start{0};
        std::size_t m_current{0};
        SourceLocation m_location{1, 1};
    };

    class LexError : public Error {
    public:
        LexError() noexcept = default;
        LexError(std::string msg, SourceLocation location) noexcept;

        [[nodiscard]] SourceLocation location() const noexcept;

        bool operator==(const LexError &other) const noexcept;
        bool operator!=(const LexError &other) const noexcept;

    protected:
        void printTo(std::ostream &out) const override;

    private:
        SourceLocation m_location{};
    };
}
