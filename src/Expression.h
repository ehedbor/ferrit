#pragma once

#include <utility>
#include "Token.h"
#include "Visitor.h"

namespace es {
    class ExpressionVisitor;
    class Expression;
    using ExpressionPtr = std::unique_ptr<Expression>;

    class Expression {
    public:
        virtual ~Expression() noexcept = 0;

        virtual VisitResult accept(ExpressionVisitor &visitor) const = 0;

        virtual bool operator==(const Expression &other) const noexcept = 0;
        virtual bool operator!=(const Expression &other) const noexcept = 0;
    };

    class NumberExpression : public Expression {
    public:
        NumberExpression(Token value, bool isIntLiteral) noexcept;

        VisitResult accept(ExpressionVisitor &visitor) const override;

        bool operator==(const Expression &other) const noexcept override;
        bool operator!=(const Expression &other) const noexcept override;

        [[nodiscard]] const Token &value() const noexcept;
        [[nodiscard]] bool isIntLiteral() const noexcept;

    private:
        Token m_value;
        bool m_isIntLiteral;
    };

    class VariableExpression : public Expression {
    public:
        explicit VariableExpression(Token name) noexcept;

        VisitResult accept(ExpressionVisitor &visitor) const override;

        bool operator==(const Expression &other) const noexcept override;
        bool operator!=(const Expression &other) const noexcept override;

        [[nodiscard]] const Token &name() const noexcept;

    private:
        Token m_name;
    };

    class ExpressionVisitor {
    public:
        virtual ~ExpressionVisitor() noexcept = 0;

        virtual VisitResult visitNumber(const NumberExpression &expr) = 0;
        virtual VisitResult visitVariable(const VariableExpression &expr) = 0;
    };
}
