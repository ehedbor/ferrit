#pragma once

#include <iosfwd>
#include "Statement.h"
#include "Expression.h"


namespace es {
    /**
     * Prints a text-based representation of the ast to an output stream.
     */
    class AstPrinter : private ExpressionVisitor, private StatementVisitor {
    public:
        explicit AstPrinter(std::ostream &out, std::vector<StatementPtr> &ast) noexcept;

        void print();

    private:
        VisitResult visitNumber(const NumberExpression &numExpr) override;
        VisitResult visitVariable(const VariableExpression &varExpr) override;
        VisitResult visitBinary(const SimpleBinaryExpression &binaryExpr) override;
        VisitResult visitBinary(const BitwiseBinaryExpression &binaryExpr) override;
        VisitResult visitBinary(const CompareBinaryExpression &binaryExpr) override;
        VisitResult visitUnary(const UnaryExpression &unaryExpr) override;

        VisitResult visitExprStmt(const ExpressionStatement &exprStmt) override;
        VisitResult visitBlock(const Block &block) override;
        VisitResult visitFunDeclaration(const FunctionDeclaration &funDecl) override;
        VisitResult visitFunDefinition(const FunctionDefinition &funDeF) override;

        void handleBinary(
            const std::string &name, const Token &op,
            const Expression &left, const Expression &right);
        void printIndent() noexcept;

    private:
        std::ostream &m_out;
        std::vector<StatementPtr> &m_ast;
        unsigned int m_depth{0};
    };
}
