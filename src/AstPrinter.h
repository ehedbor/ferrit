#pragma once

#include "Expression.h"
#include "Statement.h"

#include <concepts>
#include <iosfwd>


namespace ferrit {
    /**
     * Prints a text-based representation of a Ferrit program to an output stream.
     */
    class AstPrinter : public StatementVisitor, public ExpressionVisitor {
    public:
        explicit AstPrinter(std::ostream &out) noexcept;

        void print(const std::vector<StatementPtr> &program);

    public:
        VisitResult visitFunctionDecl(const FunctionDeclaration &funDecl) override;
        VisitResult visitBlockStmt(const BlockStatement &blockStmt) override;
        VisitResult visitExpressionStmt(const ExpressionStatement &exprStmt) override;

        VisitResult visitBinaryExpr(const BinaryExpression &binExpr) override;
        VisitResult visitComparisonExpr(const ComparisonExpression &cmpExpr) override;
        VisitResult visitUnaryExpr(const UnaryExpression &unaryExpr) override;
        VisitResult visitCallExpr(const CallExpression &callExpr) override;
        VisitResult visitVariableExpr(const VariableExpression &varExpr) override;
        VisitResult visitNumberExpr(const NumberExpression &numExpr) override;

    private:
        void printLine(const std::string &line);

        template <typename F> requires std::invocable<F>
        inline void indent(F &&block);

    private:
        static constexpr int INDENTATION_LEVEL = 2;

        std::ostream &m_out;
        unsigned int m_depth{0};
    };

    template <typename F> requires std::invocable<F>
    void AstPrinter::indent(F &&block) {
        m_depth++;
        std::invoke(block);
        m_depth--;
    }
}
