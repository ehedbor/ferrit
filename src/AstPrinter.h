#pragma once

#include <iosfwd>
#include "Statement.h"
#include "Expression.h"


namespace es {
    /**
     * Prints a text-based representation of the ast to an output stream.
     */
    class AstPrinter : public StatementVisitor, public ExpressionVisitor {
    public:
        explicit AstPrinter(std::ostream &out) noexcept;

        void print(const std::vector<StatementPtr> &ast);

    public:
        VisitResult visitFunDeclaration(const FunctionDeclaration &funDecl) override;
        VisitResult visitFunDefinition(const FunctionDefinition &funDef) override;
        VisitResult visitBlock(const Block &block) override;
        VisitResult visitExprStmt(const ExpressionStatement &exprStmt) override;

        VisitResult visitSimpleBinary(const SimpleBinaryExpression &binExpr) override;
        VisitResult visitBitwiseBinary(const BitwiseBinaryExpression &bitBinExpr) override;
        VisitResult visitComparison(const ComparisonExpression &cmpExpr) override;
        VisitResult visitUnary(const UnaryExpression &unaryExpr) override;
        VisitResult visitVariable(const VariableExpression &varExpr) override;
        VisitResult visitNumber(const NumberExpression &numExpr) override;

        void handleBinary(
            const std::string &name, const Token &op,
            const Expression &left, const Expression &right);

        void printIndent() noexcept;

    private:
        std::ostream &m_out;
        unsigned int m_depth{0};
    };
}
