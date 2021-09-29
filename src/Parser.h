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
        [[nodiscard]] ParseResult parse();

    private:
        // Declarations
        [[nodiscard]] StmtResult parseDeclaration();
        [[nodiscard]] StmtResult parseFunctionDeclaration(const std::vector<Token>& modifiers);

        // Supporting AST elements
        [[nodiscard]] std::vector<Token> parseModifiers();
        [[nodiscard]] cpp::result<std::vector<Parameter>, Error> parseParameters();
        [[nodiscard]] cpp::result<Parameter, Error> parseParameter();
        [[nodiscard]] cpp::result<Type, Error> parseType();

        // Other statements
        [[nodiscard]] StmtResult parseStatement();
        [[nodiscard]] StmtResult parseBlock();

        // Operators
        [[nodiscard]] ExprResult parseExpression();
        [[nodiscard]] ExprResult parseDisjunction();
        [[nodiscard]] ExprResult parseConjunction();
        [[nodiscard]] ExprResult parseBitwiseOr();
        [[nodiscard]] ExprResult parseBitwiseXor();
        [[nodiscard]] ExprResult parseBitwiseAnd();
        [[nodiscard]] ExprResult parseEquality();
        [[nodiscard]] ExprResult parseComparison();
        [[nodiscard]] ExprResult parseBitwiseShift();
        [[nodiscard]] ExprResult parseAdditive();
        [[nodiscard]] ExprResult parseMultiplicative();
        [[nodiscard]] ExprResult parseUnary();

        // Simple expressions
        [[nodiscard]] ExprResult parsePrimary();
        [[nodiscard]] ExprResult parseVariable();
        [[nodiscard]] ExprResult parseNumber();

        /**
         * Attempts to recover from an error by skipping tokens until finding
         * one that is likely to start a new line. This is to allow for finding
         * multiple parse errors at once.
         */
        void synchronize();

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
        FoundTerminators skipTerminators(bool allowSemicolons);

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current \c Token matches the given \c TokenType. If it does,
         * the \c Parser is advanced and the next \c Token is returned.
         * Otherwise, a \c ParseError is returned.
         */
        [[nodiscard]] TokenResult consume(TokenType expected, const std::string &errMsg);

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current \c Token matches the given \c TokenType. If it does,
         * the \c Parser is advanced.
         *
         * @return \c true if the current \c Token matches the expected \c TokenType
         */
        [[nodiscard]] bool match(TokenType expected);

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current \c Token's type matches the given \c TokenType.
         */
        [[nodiscard]] bool check(TokenType expected);

        /**
         * Advances the \c Parser (unless EOF is reached)
         * and then returns the current \c Token.
         */
        const Token &advance();

        [[nodiscard]] const Token &current() const noexcept;
        [[nodiscard]] const Token &previous() const noexcept;
        [[nodiscard]] bool isAtEnd() const noexcept;

        template <typename... Args>
        [[nodiscard]] Error makeError(const Args&... args) const;

    private:
        std::vector<Token> m_tokens;
        std::size_t m_current{0};
    };

    template <typename... Args>
    Error Parser::makeError(const Args&... args) const {
        return Error(current(), ErrorCode::SyntaxParseError, args...);
    }
}
