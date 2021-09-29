#pragma once

#include <optional>
#include <vector>

#include <result.hpp>

#include "Error.h"
#include "Expression.h"
#include "Statement.h"
#include "Token.h"

namespace ferrit {
    using ParseResult = cpp::result<std::vector<StatementPtr>, std::vector<Error>>;
    using StmtResult = cpp::result<StatementPtr, Error>;
    using ExprResult = cpp::result<ExpressionPtr, Error>;
    using TokenResult = cpp::result<const Token &, Error>;

    /**
     * Converts a stream of tokens into an abstract syntax tree.
     */
    class Parser {
    public:
        explicit Parser(std::vector<Token> tokens) noexcept;

        /**
         * Parses an entire stream of tokens, representing an entire file.
         *
         * @note Top-level expressions are currently not permitted.
         */
        [[nodiscard]] ParseResult parse() noexcept;

    private:
        // Declarations
        [[nodiscard]] StmtResult parseDeclaration() noexcept;
        [[nodiscard]] StmtResult parseFunctionDeclaration(const std::vector<Token>& modifiers) noexcept;

        // Supporting AST elements
        [[nodiscard]] std::vector<Token> parseModifiers() noexcept;
        [[nodiscard]] cpp::result<std::vector<Parameter>, Error> parseParameters() noexcept;
        [[nodiscard]] cpp::result<Parameter, Error> parseParameter() noexcept;
        [[nodiscard]] cpp::result<Type, Error> parseType() noexcept;

        // Other statements
        [[nodiscard]] StmtResult parseStatement() noexcept;
        [[nodiscard]] StmtResult parseBlock() noexcept;

        // Operators
        [[nodiscard]] ExprResult parseExpression() noexcept;
        [[nodiscard]] ExprResult parseDisjunction() noexcept;
        [[nodiscard]] ExprResult parseConjunction() noexcept;
        [[nodiscard]] ExprResult parseBitwiseOr() noexcept;
        [[nodiscard]] ExprResult parseBitwiseXor() noexcept;
        [[nodiscard]] ExprResult parseBitwiseAnd() noexcept;
        [[nodiscard]] ExprResult parseEquality() noexcept;
        [[nodiscard]] ExprResult parseComparison() noexcept;
        [[nodiscard]] ExprResult parseBitwiseShift() noexcept;
        [[nodiscard]] ExprResult parseAdditive() noexcept;
        [[nodiscard]] ExprResult parseMultiplicative() noexcept;
        [[nodiscard]] ExprResult parseUnary() noexcept;

        // Simple expressions
        [[nodiscard]] ExprResult parsePrimary() noexcept;
        [[nodiscard]] ExprResult parseVariable() noexcept;
        [[nodiscard]] ExprResult parseNumber() noexcept;

        /**
         * Attempts to recover from an error by skipping tokens until finding
         * one that is likely to start a new line. This is to allow for finding
         * multiple parse errors at once.
         */
        void synchronize() noexcept;

        struct FoundTerminators {
            bool newline = false;
            bool semicolon = false;
            bool eof = false;

            [[nodiscard]] bool any() const noexcept { return newline || semicolon || eof; }
        };
        /**
         * Skips all line terminators encountered until the next
         * non-line terminator \c Token is found.
         *
         * @param allowSemicolons if semicolons are permitted
         * @return which kinds of terminators were found
         */
        FoundTerminators skipTerminators(bool allowSemicolons) noexcept;

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current \c Token matches the given \c TokenType. If it does,
         * the \c Parser is advanced and the next \c Token is returned.
         * Otherwise, a \c ParseError is returned.
         */
        [[nodiscard]] TokenResult consume(TokenType expected, const std::string &errMsg) noexcept;

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current \c Token matches the given \c TokenType. If it does,
         * the \c Parser is advanced.
         *
         * @return \c true if the current \c Token matches the expected \c TokenType
         */
        [[nodiscard]] bool match(TokenType expected) noexcept;

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current \c Token's type matches the given \c TokenType.
         */
        [[nodiscard]] bool check(TokenType expected) noexcept;

        /**
         * Advances the \c Parser (unless EOF is reached)
         * and then returns the current \c Token.
         */
        const Token &advance() noexcept;

        [[nodiscard]] const Token &current() const noexcept;
        [[nodiscard]] const Token &previous() const noexcept;
        [[nodiscard]] bool isAtEnd() const noexcept;

        Error makeError(const std::vector<std::string> &fmtArgs) const noexcept;

    private:
        std::vector<Token> m_tokens;
        std::size_t m_current{0};
    };
}
