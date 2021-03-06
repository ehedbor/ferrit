#pragma once

#include "Token.h"
#include "Visitor.h"

#include <utility>
#include <vector>

namespace ferrit {
    class Expression;
    class BinaryExpression;
    class ComparisonExpression;
    class UnaryExpression;
    class CallExpression;
    class VariableExpression;
    class NumberExpression;
    class BooleanExpression;

    using ExpressionPtr = std::unique_ptr<Expression>;

    /**
     * Allows for traversal of a hierarchy of \c Expression nodes.
     *
     * @see StatementVisitor
     */
    class ExpressionVisitor {
    public:
        virtual ~ExpressionVisitor() noexcept = default;

        virtual VisitResult visitBinaryExpr(const BinaryExpression &binExpr) = 0;
        virtual VisitResult visitComparisonExpr(const ComparisonExpression &cmpExpr) = 0;
        virtual VisitResult visitUnaryExpr(const UnaryExpression &unaryExpr) = 0;
        virtual VisitResult visitCallExpr(const CallExpression &callExpr) = 0;
        virtual VisitResult visitVariableExpr(const VariableExpression &varExpr) = 0;
        virtual VisitResult visitNumberExpr(const NumberExpression &numExpr) = 0;
        virtual VisitResult visitBoolExpr(const BooleanExpression &boolExpr) = 0;
    };

    /**
     * Base class for any AST construct that can produce a value when compiled.
     */
    class Expression {
    public:
        virtual ~Expression() noexcept = default;

        MAKE_BASE_VISITABLE(ExpressionVisitor);

        [[nodiscard]] bool operator==(const Expression &other) const noexcept;

        /**
         * Returns a suitable token for error reporting.
         */
        [[nodiscard]] virtual const Token &errorToken() const noexcept = 0;

    protected:
        [[nodiscard]] virtual bool equals(const Expression &other) const noexcept = 0;
    };

    /**
    * Represents logical operators, arithmetic operators and the concatenate operator.
    */
    class BinaryExpression final : public Expression {
    public:
        explicit BinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, BinaryExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_op;
        ExpressionPtr m_left;
        ExpressionPtr m_right;
    };

    /**
     * Represents the two equality operators as well as the four comparison operators.
     */
    class ComparisonExpression final : public Expression {
    public:
        explicit ComparisonExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &left() const noexcept;
        [[nodiscard]] const Expression &right() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, ComparisonExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_op;
        ExpressionPtr m_left;
        ExpressionPtr m_right;
    };

    /**
     * Represents the unary operators.
     */
    class UnaryExpression final : public Expression {
    public:
        explicit UnaryExpression(Token op, ExpressionPtr operand, bool isPrefix) noexcept;

        [[nodiscard]] const Token &op() const noexcept;
        [[nodiscard]] const Expression &operand() const noexcept;
        [[nodiscard]] bool isPrefix() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, UnaryExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_op;
        ExpressionPtr m_operand;
        bool m_isPrefix;
    };

    /**
     * Represents a function call or constructor.
     */
    class CallExpression final : public Expression {
    public:
        explicit CallExpression(Token paren, ExpressionPtr callee, std::vector<ExpressionPtr> arguments) noexcept;

        [[nodiscard]] const Token &paren() const noexcept;
        [[nodiscard]] const Expression &callee() const noexcept;
        [[nodiscard]] const std::vector<ExpressionPtr> &arguments() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, CallExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_paren;
        ExpressionPtr m_callee;
        std::vector<ExpressionPtr> m_arguments;
    };

    /**
     * Represents a direct variable access (ie just writing the variable's name in an expression).
     */
    class VariableExpression final : public Expression {
    public:
        explicit VariableExpression(Token name) noexcept;

        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, VariableExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_name;
    };

    /**
     * Represents a literal number (either integer or real).
     */
    class NumberExpression final : public Expression {
    public:
        explicit NumberExpression(Token value, bool isIntLiteral) noexcept;

        [[nodiscard]] const Token &value() const noexcept;
        [[nodiscard]] bool isIntLiteral() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, NumberExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_value;
        bool m_isIntLiteral;
    };

    /**
     * Represents a literal boolean (true or false).
     */
    class BooleanExpression final : public Expression {
    public:
        explicit BooleanExpression(Token value) noexcept;

        [[nodiscard]] const Token &value() const noexcept;
        [[nodiscard]] const Token &errorToken() const noexcept override;

        MAKE_VISITABLE(ExpressionVisitor, BoolExpr);

    protected:
        [[nodiscard]] bool equals(const Expression &other) const noexcept override;

    private:
        Token m_value;
    };
}
