#pragma once

#include <utility>
#include "Token.h"
#include "Visitor.h"

namespace ferrit {
    class Expression;
    class SimpleBinaryExpression;
    class BitwiseBinaryExpression;
    class ComparisonExpression;
    class UnaryExpression;
    class VariableExpression;
    class NumberExpression;

    using ExpressionPtr = std::unique_ptr<Expression>;

    /**
     * Allows for traversal of a hierarchy of \c Expression nodes.
     *
     * @see StatementVisitor
     */
    class ExpressionVisitor {
    public:
        virtual ~ExpressionVisitor() noexcept = default;

        virtual VisitResult visitSimpleBinary(const SimpleBinaryExpression &binExpr) = 0;
        virtual VisitResult visitBitwiseBinary(const BitwiseBinaryExpression &bitBinExpr) = 0;
        virtual VisitResult visitComparison(const ComparisonExpression &cmpExpr) = 0;
        virtual VisitResult visitUnary(const UnaryExpression &expr) = 0;
        virtual VisitResult visitVariable(const VariableExpression &varExpr) = 0;
        virtual VisitResult visitNumber(const NumberExpression &numExpr) = 0;
    };

    /**
     * Base class for any AST construct that can produce a value when compiled.
     */
    class Expression {
    public:
        virtual ~Expression() noexcept = default;

        MAKE_BASE_VISITABLE(ExpressionVisitor);

        virtual bool operator==(const Expression &other) const noexcept = 0;
        virtual bool operator!=(const Expression &other) const noexcept;
    };

    /**
    * Represents the logical and/or operators as well as the additive and multiplicative operators.
    */
    class SimpleBinaryExpression : public Expression {
    public:
        SimpleBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;

        MAKE_VISITABLE(ExpressionVisitor, SimpleBinary);
        bool operator==(const Expression &other) const noexcept override;

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

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;

        MAKE_VISITABLE(ExpressionVisitor, BitwiseBinary);
        bool operator==(const Expression &other) const noexcept override;

    private:
        Token m_op;
        ExpressionPtr m_left;
        ExpressionPtr m_right;
    };

    /**
     * Represents the two equality operators as well as the four comparison operators.
     */
    class ComparisonExpression : public Expression {
    public:
        ComparisonExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;

        MAKE_VISITABLE(ExpressionVisitor, Comparison);
        bool operator==(const Expression &other) const noexcept override;

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

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &operand() const noexcept;

        MAKE_VISITABLE(ExpressionVisitor, Unary);
        bool operator==(const Expression &other) const noexcept override;

    private:
        Token m_op;
        ExpressionPtr m_operand;
    };

    /**
     * Represents a direct variable access (ie just writing the variable's name in an expression).
     */
    class VariableExpression : public Expression {
    public:
        explicit VariableExpression(Token name) noexcept;

        [[nodiscard]] const Token &name() const noexcept;

        MAKE_VISITABLE(ExpressionVisitor, Variable);
        bool operator==(const Expression &other) const noexcept override;

    private:
        Token m_name;
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

        [[nodiscard]] const Token &value() const noexcept;
        [[nodiscard]] bool isIntLiteral() const noexcept;

        MAKE_VISITABLE(ExpressionVisitor, Number);
        bool operator==(const Expression &other) const noexcept override;

    private:
        Token m_value;
        bool m_isIntLiteral;
    };
}
