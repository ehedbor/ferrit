#pragma once

#include <optional>
#include <vector>
#include <result.hpp>
#include "Token.h"
#include "Expression.h"
#include "Statement.h"


struct ParseError;

using ParseResult = cpp::result<std::vector<StatementPtr>, std::vector<ParseError>>;
using StmtResult = cpp::result<StatementPtr, ParseError>;
using ExprResult = cpp::result<ExpressionPtr, ParseError>;
using TokenResult = cpp::result<const Token &, ParseError>;

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    [[nodiscard]] ParseResult parse();

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

    [[nodiscard]] ExprResult parsePrimary() noexcept;
    [[nodiscard]] ExprResult parseVariable() noexcept;
    [[nodiscard]] ExprResult parseNumber() noexcept;

    void synchronize() noexcept;

    bool skipTerminators(bool softEndingsOnly) noexcept;

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

struct ParseError {
public:
    ParseError() = default;
    ParseError(Token cause, std::string msg);

    friend std::ostream &operator<<(std::ostream &out, const ParseError& error);

public:
    Token cause{};
    std::string msg{};
};
