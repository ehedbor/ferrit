#pragma once

#include <iosfwd>
#include "Statement.h"
#include "Expression.h"


namespace es {
    class AstPrinter : private StatementVisitor, private ExpressionVisitor {
    public:
        explicit AstPrinter(std::ostream &out, std::vector<StatementPtr> &ast) noexcept;

        void print() noexcept;

    private:
        void visitNumber(const NumberExpression &expr) noexcept override;
        void visitVariable(const VariableExpression &expr) noexcept override;
        void visitExpr(const ExpressionStatement &stmt) noexcept override;
        void visitBlock(const BlockStatement &stmt) noexcept override;
        void visitFunction(const FunctionStatement &stmt) noexcept override;

        void printIndent() noexcept;

    private:
        std::ostream &m_out;
        std::vector<StatementPtr> &m_ast;
        unsigned int m_depth{0};
    };
}
