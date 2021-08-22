#include "Expression.h"

Expression::~Expression() noexcept = default;

NumberExpression::NumberExpression(Token value, bool isIntLiteral) :
    m_value(std::move(value)), m_isIntLiteral(isIntLiteral) {

}

void NumberExpression::accept(ExpressionVisitor &visitor) const {
    visitor.visitNumber(*this);
}

const Token &NumberExpression::value() const {
    return m_value;
}

bool NumberExpression::isIntLiteral() const {
    return m_isIntLiteral;
}

VariableExpression::VariableExpression(Token name) :
    m_name(std::move(name)) {

}

void VariableExpression::accept(ExpressionVisitor &visitor) const {
    visitor.visitVariable(*this);
}

const Token &VariableExpression::name() const {
    return m_name;
}

ExpressionVisitor::~ExpressionVisitor() noexcept = default;