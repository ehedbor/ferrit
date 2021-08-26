#pragma once

#include <iosfwd>
#include "Statement.h"
#include "Expression.h"


namespace es {
    class AstPrinter : private ExpressionVisitor, private StatementVisitor {
    public:
        explicit AstPrinter(std::ostream &out, std::vector<StatementPtr> &ast) noexcept;

        void print() noexcept;

    private:
        VisitResult visitNumber(const NumberExpression &expr) noexcept override;
        VisitResult visitVariable(const VariableExpression &expr) noexcept override;
        VisitResult visitBinary(const SimpleBinaryExpression &expr) noexcept override;
        VisitResult visitBinary(const BitwiseBinaryExpression &expr) noexcept override;
        VisitResult visitBinary(const CompareBinaryExpression &expr) noexcept override;
        VisitResult visitUnary(const UnaryExpression &expr) noexcept override;

        VisitResult visitExprStmt(const ExpressionStatement &stmt) noexcept override;
        VisitResult visitBlock(const BlockStatement &stmt) noexcept override;
        VisitResult visitFunction(const FunctionStatement &stmt) noexcept override;

        void handleBinary(
            const std::string &name, const Token &op,
            const Expression &left, const Expression &right) noexcept;
        void printIndent() noexcept;

    private:
        std::ostream &m_out;
        std::vector<StatementPtr> &m_ast;
        unsigned int m_depth{0};
    };
}
