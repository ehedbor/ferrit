#pragma once

#include <optional>
#include <vector>

#include <result.hpp>

#include "Error.h"
#include "Expression.h"
#include "Statement.h"
#include "Token.h"

namespace es {
    class ParseError;

    using ParseResult = cpp::result<std::vector<StatementPtr>, std::vector<ParseError>>;
    using StmtResult = cpp::result<StatementPtr, ParseError>;
    using ExprResult = cpp::result<ExpressionPtr, ParseError>;
    using TokenResult = cpp::result<const Token &, ParseError>;

    class Parser {
    public:
        explicit Parser(std::vector<Token> tokens) noexcept;

        [[nodiscard]] ParseResult parse() noexcept;

    private:
        [[nodiscard]] StmtResult parseDeclaration() noexcept;
        [[nodiscard]] StmtResult parseFunctionDeclaration(const std::vector<Token>& modifiers) noexcept;
        [[nodiscard]] std::vector<Token> parseModifiers() noexcept;
        [[nodiscard]] cpp::result<std::vector<Parameter>, ParseError> parseParameters() noexcept;
        [[nodiscard]] cpp::result<Parameter, ParseError> parseParameter() noexcept;
        [[nodiscard]] cpp::result<Type, ParseError> parseType() noexcept;

        [[nodiscard]] StmtResult parseStatement() noexcept;
        [[nodiscard]] StmtResult parseBlock() noexcept;

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


        [[nodiscard]] ExprResult parsePrimary() noexcept;
        [[nodiscard]] ExprResult parseVariable() noexcept;
        [[nodiscard]] ExprResult parseNumber() noexcept;

        void synchronize() noexcept;

        struct FoundTerminators {
            bool newline = false;
            bool semicolon = false;
            bool eof = false;

            [[nodiscard]] bool any() const noexcept { return newline || semicolon || eof; }
        };
        FoundTerminators skipTerminators(bool allowSemicolons) noexcept;

        [[nodiscard]] TokenResult consume(TokenType expected, const std::string &errMsg) noexcept;
        [[nodiscard]] bool match(TokenType expected) noexcept;
        [[nodiscard]] bool check(TokenType expected) noexcept;

        const Token &advance() noexcept;
        [[nodiscard]] const Token &current() const noexcept;
        [[nodiscard]] const Token &previous() const noexcept;
        [[nodiscard]] bool isAtEnd() const noexcept;


    private:
        std::vector<Token> m_tokens;
        std::size_t m_current{0};
    };

    class ParseError : public Error {
    public:
        ParseError() noexcept = default;
        ParseError(Token cause, std::string msg) noexcept;

        [[nodiscard]] const Token &cause() const noexcept;

    protected:
        void printTo(std::ostream &out) const override;

    private:
        Token m_cause{};
    };
}
