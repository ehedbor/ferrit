#include "Statement.h"

#include <utility>


Statement::~Statement() noexcept = default;

ExpressionStatement::ExpressionStatement(ExpressionPtr expr) :
    m_expr(std::move(expr)) {

}

void ExpressionStatement::accept(StatementVisitor &visitor) const {
    visitor.visitExpr(*this);
}

const Expression &ExpressionStatement::expr() const {
    return *m_expr;
}

BlockStatement::BlockStatement(std::vector<StatementPtr> body) :
    m_body(std::move(body)) {

}

void BlockStatement::accept(StatementVisitor &visitor) const {
    visitor.visitBlock(*this);
}

const std::vector<StatementPtr> &BlockStatement::body() const {
    return m_body;
}

FunctionStatement::FunctionStatement(
    std::vector<Token> modifiers, Token keyword, Token name,
    std::vector<Parameter> params, Type returnType, StatementPtr body) :
    m_modifiers(std::move(modifiers)), m_keyword(std::move(keyword)), m_name(std::move(name)),
    m_params(std::move(params)), m_returnType(std::move(returnType)), m_body(std::move(body)) {

}

void FunctionStatement::accept(StatementVisitor &visitor) const {
    visitor.visitFunction(*this);
}

const std::vector<Token> &FunctionStatement::modifiers() const {
    return m_modifiers;
}

const Token &FunctionStatement::keyword() const {
    return m_keyword;
}

const Token &FunctionStatement::name() const {
    return m_name;
}

const std::vector<Parameter> &FunctionStatement::params() const {
    return m_params;
}

const Type &FunctionStatement::returnType() const {
    return m_returnType;
}

const Statement &FunctionStatement::body() const {
    return *m_body;
}

StatementVisitor::~StatementVisitor() noexcept = default;