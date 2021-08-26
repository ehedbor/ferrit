#pragma once

#include <utility>
#include "Token.h"
#include "Visitor.h"

namespace es {
    class ExpressionVisitor;
    class Expression;
    using ExpressionPtr = std::unique_ptr<Expression>;

    /**
     * Base class for any AST construct that can produce a value when executed.
     */
    class Expression {
    public:
        virtual ~Expression() noexcept = 0;

        virtual VisitResult accept(ExpressionVisitor &visitor) const = 0;

        virtual bool operator==(const Expression &other) const noexcept = 0;
        virtual bool operator!=(const Expression &other) const noexcept = 0;
    };

    /**
     * Represents a literal number (either integer or floating point).
     * Literals numbers are considered to be an unsized supertype of their
     * respective number types that get coerced into a sized type when used in
     * an expression.
     */
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

    /**
     * Represents a direct variable access (ie just writing the variable's name in an expression).
     */
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

    /**
     * Represents the logical and/or operators as well as the additive and multiplicative operators.
     */
    class SimpleBinaryExpression : public Expression {
    public:
        SimpleBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        VisitResult accept(ExpressionVisitor &visitor) const override;

        bool operator==(const Expression &other) const noexcept override;
        bool operator!=(const Expression &other) const noexcept override;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;

    private:
        Token m_op;
        ExpressionPtr m_left;
        ExpressionPtr m_right;
    };

    /**
     * Represents the binary bitwise operators, such as '&' and '<<'.
     */
    class BitwiseBinaryExpression : public Expression {
    public:
        BitwiseBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        VisitResult accept(ExpressionVisitor &visitor) const override;

        bool operator==(const Expression &other) const noexcept override;
        bool operator!=(const Expression &other) const noexcept override;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;

    private:
        Token m_op;
        ExpressionPtr m_left;
        ExpressionPtr m_right;
    };

    /**
     * Represents the two equality operators as well as the four comparison operators.
     */
    class CompareBinaryExpression : public Expression {
    public:
        CompareBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        VisitResult accept(ExpressionVisitor &visitor) const override;

        bool operator==(const Expression &other) const noexcept override;
        bool operator!=(const Expression &other) const noexcept override;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;

    private:
        Token m_op;
        ExpressionPtr m_left;
        ExpressionPtr m_right;
    };

    /**
     * Represents the unary operators.
     */
    class UnaryExpression : public Expression {
    public:
        UnaryExpression(Token op, ExpressionPtr operand) noexcept;

        VisitResult accept(ExpressionVisitor &visitor) const override;

        bool operator==(const Expression &other) const noexcept override;
        bool operator!=(const Expression &other) const noexcept override;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &operand() const noexcept;

    private:
        Token m_op;
        ExpressionPtr m_operand;
    };

    /**
     * Allows for traversal of a hierarchy of \c Expression nodes.
     *
     * @see StatementVisitor
     */
    class ExpressionVisitor {
    public:
        virtual ~ExpressionVisitor() noexcept = 0;

        virtual VisitResult visitNumber(const NumberExpression &expr) = 0;
        virtual VisitResult visitVariable(const VariableExpression &expr) = 0;
        virtual VisitResult visitBinary(const SimpleBinaryExpression &expr) = 0;
        virtual VisitResult visitBinary(const BitwiseBinaryExpression &expr) = 0;
        virtual VisitResult visitBinary(const CompareBinaryExpression &expr) = 0;
        virtual VisitResult visitUnary(const UnaryExpression &expr) = 0;
    };
}
