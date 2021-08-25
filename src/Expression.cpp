#include "Expression.h"


namespace es {
    Expression::~Expression() noexcept = default;


    NumberExpression::NumberExpression(Token value, bool isIntLiteral) noexcept :
        m_value(std::move(value)), m_isIntLiteral(isIntLiteral) {
    }

    VisitResult NumberExpression::accept(ExpressionVisitor &visitor) const {
        return visitor.visitNumber(*this);
    }

    const Token &NumberExpression::value() const noexcept {
        return m_value;
    }

    bool NumberExpression::isIntLiteral() const noexcept {
        return m_isIntLiteral;
    }

    bool NumberExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *numExpr = dynamic_cast<const NumberExpression *>(&other)) {
            return value() == numExpr->value() && isIntLiteral() == numExpr->isIntLiteral();
        }
        return false;
    }

    bool NumberExpression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
    }


    VariableExpression::VariableExpression(Token name) noexcept :
        m_name(std::move(name)) {
    }

    VisitResult VariableExpression::accept(ExpressionVisitor &visitor) const {
        return visitor.visitVariable(*this);
    }

    const Token &VariableExpression::name() const noexcept {
        return m_name;
    }

    bool VariableExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *varExpr = dynamic_cast<const VariableExpression *>(&other)) {
            return name() == varExpr->name();
        }
        return false;
    }

    bool VariableExpression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
    }


    ExpressionVisitor::~ExpressionVisitor() noexcept = default;
}
