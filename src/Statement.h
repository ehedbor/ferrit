#pragma once

#include <memory>
#include <vector>

#include "Token.h"
#include "Expression.h"
#include "AstElements.h"


class StatementVisitor;
class Statement;
using StatementPtr = std::unique_ptr<Statement>;


class Statement {
public:
    virtual ~Statement() noexcept = 0;

    virtual void accept(StatementVisitor &visitor) const = 0;
};

class ExpressionStatement : public Statement {
public:
    explicit ExpressionStatement(ExpressionPtr expr);

    void accept(StatementVisitor &visitor) const override;

    [[nodiscard]] const Expression &expr() const;

private:
    ExpressionPtr m_expr;
};

class BlockStatement : public Statement {
public:
    explicit BlockStatement(std::vector<StatementPtr> body);

    void accept(StatementVisitor &visitor) const override;

    [[nodiscard]] const std::vector<StatementPtr> &body() const;

private:
    std::vector<StatementPtr> m_body;
};

class FunctionStatement : public Statement {
public:
    FunctionStatement(
        std::vector<Token> modifiers, Token keyword, Token name,
        std::vector<Parameter> params, Type returnType, StatementPtr body);

    void accept(StatementVisitor &visitor) const override;

    [[nodiscard]] const std::vector<Token> &modifiers() const;
    [[nodiscard]] const Token &keyword() const;
    [[nodiscard]] const Token &name() const;
    [[nodiscard]] const std::vector<Parameter> &params() const;
    [[nodiscard]] const Type &returnType() const;
    [[nodiscard]] const Statement &body() const;

private:
    std::vector<Token> m_modifiers;
    Token m_keyword;
    Token m_name;
    std::vector<Parameter> m_params;
    Type m_returnType;
    StatementPtr m_body;
};

class StatementVisitor {
public:
    virtual ~StatementVisitor() noexcept = 0;

    virtual void visitExpr(const ExpressionStatement &stmt) = 0;
    virtual void visitBlock(const BlockStatement &stmt) = 0;
    virtual void visitFunction(const FunctionStatement &stmt) = 0;
};