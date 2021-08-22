#pragma once

#include <iosfwd>
#include "Statement.h"
#include "Expression.h"

class AstPrinter : private StatementVisitor, private ExpressionVisitor {
public:
    explicit AstPrinter(std::ostream &out, std::vector<StatementPtr> &ast);

    void print();

private:
    void visitNumber(const NumberExpression &expr) override;
    void visitVariable(const VariableExpression &expr) override;
    void visitExpr(const ExpressionStatement &stmt) override;
    void visitBlock(const BlockStatement &stmt) override;
    void visitFunction(const FunctionStatement &stmt) override;

    void printIndent();

private:
    std::ostream &m_out;
    std::vector<StatementPtr> &m_ast;
    unsigned int m_depth{0};
};


