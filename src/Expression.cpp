#include "Expression.h"


namespace es {
    Expression::~Expression() noexcept = default;

    NumberExpression::NumberExpression(Token value, bool isIntLiteral) noexcept :
        m_value(std::move(value)), m_isIntLiteral(isIntLiteral) {
    }

    void NumberExpression::accept(ExpressionVisitor &visitor) const {
        visitor.visitNumber(*this);
    }

    const Token &NumberExpression::value() const noexcept {
        return m_value;
    }

    bool NumberExpression::isIntLiteral() const noexcept {
        return m_isIntLiteral;
    }

    VariableExpression::VariableExpression(Token name) noexcept :
        m_name(std::move(name)) {
    }

    void VariableExpression::accept(ExpressionVisitor &visitor) const {
        visitor.visitVariable(*this);
    }

    const Token &VariableExpression::name() const noexcept {
        return m_name;
    }

    ExpressionVisitor::~ExpressionVisitor() noexcept = default;
}
