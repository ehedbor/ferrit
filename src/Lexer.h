#pragma once

#include "Error.h"
#include "ErrorReporter.h"
#include "Token.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace ferrit {
    /**
     * Converts source code into a list of tokens.
     */
    class Lexer final {
    public:
        Lexer() noexcept = default;

        /**
         * Constructs a lexer with the given error reporter.
         *
         * @param errorReporter logger for compile errors
         */
        explicit Lexer(std::shared_ptr<ErrorReporter> errorReporter) noexcept;

    private:
        /**
         * Initializes the lexer with the given source code.
         *
         * @param code a lexically-valid source code snippet
         */
        void init(const std::string &code) noexcept;

    public:
        /**
         * Scans all tokens from the given source code.
         */
        std::optional<std::vector<Token>> lex(const std::string &code);

    private:
        /**
         * Scans the next token.
         *
         * @return the token
         * @throws LexException if an error occurs
         */
        Token lexNext();

        /**
         * Skips ASCII whitespace characters up until the next newline.
         *
         * @return A newline token if one is found
         * @throws LexException if a comment is ill-formed
         */
        std::optional<Token> skipWhitespace();

        /**
         * Ignores a line comment.
         */
        void ignoreLineComment() noexcept;

        /**
         * Ignores a block comment.
         *
         * @throws LexException if the block comment is not terminated
         */
        void ignoreBlockComment();

        /**
         * Scans and returns a string literal.
         *
         * @return a token representing a string literal
         * @throws LexException if the string is ill-formed
         */
        Token lexString();

        /**
         * Scans and returns a char literal.
         *
         * @return a token representing a char literal
         * @throws LexException if the char is ill-formed
         */
        Token lexChar();

        /**
         * Advances the lexer, returning an error if the next char cannot
         * appear in a string-like literal (such as a \c char or \c string).
         *
         * @param literalType the type of literal (i.e. <tt>"string literal"</tt>)
         * @throws LexException if the literal is ill-formed
         */
        void advanceStringChar(const std::string &literalType);

        /**
         * Scans an integer or float literal.
         *
         * @return the literal
         * @throws LexException if the literal is ill-formed
         */
        Token lexNumber();

        /**
         * Scans an identifier or a keyword.
         *
         * @return the ident/keyword
         */
        Token lexIdentifier() noexcept;

        /**
         * Checks if the current token is a keyword or an identifier.
         *
         * @return the type of keyword, or else <tt>TokenType::Identifier</t>
         */
        [[nodiscard]] TokenType getCurrentKeywordType() noexcept;

        /**
         * Constructs a \c Token from the current substring.
         *
         * @param type the token's type
         * @return the token
         */
        [[nodiscard]] Token makeToken(TokenType type) const noexcept;

        /**
         * Constructs a \c LexException from the current substring, logging the error
         * if permitted by the compile options.
         *
         * Note that the exception is not thrown. It is the caller's decision to throw.
         *
         * @tparam E a type extending \c ferrit::Error
         * @tparam Args the type of the arguments to be passed to <tt>Err</tt>'s constructor
         * @param args the arguments to <tt>Err</tt>'s constructor
         * @return the exception
         * @see ferrit::Error
         */
        template <typename E, typename... Args>
        requires std::derived_from<E, Error> && std::constructible_from<E, Token, Args...>
        [[nodiscard]] E makeError(Args&&... args) const;

        /**
         * Gets the current character without advancing the lexer.
         *
         * @return the current char, or \c std::nullopt if no char is being pointed to.
         */
        [[nodiscard]] std::optional<char> peek() const noexcept;

        /**
         * Gets the next character without advancing the lexer.
         *
         * @return the next char, or \c std::nullopt if no char is being pointed to.
         */
        [[nodiscard]] std::optional<char> peekNext() const noexcept;

        /**
         * Gets the nth next character without advancing the lexer.
         *
         * @param n the amount of lookahead. 0 = current
         * @return the nth char, or \c std::nullopt if no char is being pointed to.
         */
        [[nodiscard]] std::optional<char> peekN(int n) const noexcept;

        /**
         * Consumes and returns the currently-referenced character.
         */
        std::optional<char> advance() noexcept;

        /**
         * Checks to see if the current character is the same as the given character,
         * and advances the lexer if they are.
         */
        [[nodiscard]] bool match(char expected) noexcept;

        /**
         * Returns true if the lexer has reached the end of the source code.
         */
        [[nodiscard]] bool isAtEnd() const noexcept;

        /**
         * Returns the type of newline that is currently being pointed to.
         *
         * @return \c 0 if no newline, \c 1 for LF-style newlines (<tt>"\\n"</tt>),
         * or \c 2 for CRLF-style newlines (<tt>"\\r\\n"</tt>).
         */
        [[nodiscard]] int getCurrentNewlineType() const noexcept;

        /**
         * Returns true if the given character is an ASCII digit.
         */
        [[nodiscard]] static bool isDigit(char ch) noexcept;

        /**
         * Returns true if the given character can appear in the middle of an identifier.
         */
        [[nodiscard]] static bool isIdentifier(char ch) noexcept;

        /**
         * Returns true if the given character can start an identifier.
         */
        [[nodiscard]] static bool isIdentifierStart(char ch) noexcept;

    private:
        std::shared_ptr<ErrorReporter> m_errorReporter{nullptr};
        std::string m_code{};
        int m_start{0};
        int m_current{0};
        SourceLocation m_location{1, 1};
    };

    template <typename E, typename... Args>
        requires std::derived_from<E, Error> && std::constructible_from<E, Token, Args...>
    [[nodiscard]] E Lexer::makeError(Args&&... args) const {
        int count = m_current - m_start;
        int startColumn = m_location.column - count;
        std::string lexeme = m_code.substr(m_start, count);
        Token token{TokenType::Error, std::move(lexeme), {m_location.line, startColumn}};

        E error{token, std::forward<Args>(args)...};
        if (m_errorReporter) {
            m_errorReporter->logError(error);
        }
        return error;
    }
}
