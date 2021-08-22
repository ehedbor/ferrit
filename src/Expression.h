#pragma once

#include <memory>
#include "Token.h"

class ExpressionVisitor;
class Expression;

using ExpressionPtr = std::unique_ptr<Expression>;


class Expression {
public:
    virtual ~Expression() noexcept = 0;

    virtual void accept(ExpressionVisitor &visitor) const = 0;
};

class NumberExpression : public Expression {
public:
    NumberExpression(Token value, bool isIntLiteral);

    void accept(ExpressionVisitor &visitor) const override;

    [[nodiscard]] const Token &value() const;
    [[nodiscard]] bool isIntLiteral() const;

private:
    Token m_value;
    bool m_isIntLiteral;
};

class VariableExpression : public Expression {
public:
    explicit VariableExpression(Token name);

    void accept(ExpressionVisitor &visitor) const override;

    [[nodiscard]] const Token &name() const;

private:
    Token m_name;
};

class ExpressionVisitor {
public:
    virtual ~ExpressionVisitor() noexcept = 0;

    virtual void visitNumber(const NumberExpression &expr) = 0;
    virtual void visitVariable(const VariableExpression &expr) = 0;
};