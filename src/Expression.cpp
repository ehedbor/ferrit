#include "Expression.h"


namespace ferrit {
    bool Expression::operator==(const Expression &other) const noexcept {
        if (this == &other) {
            return true;
        } else if (typeid(*this) != typeid(other)) {
            return false;
        } else {
            return this->equals(other);
        }
    }

    BinaryExpression::BinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    const Token &BinaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &BinaryExpression::left() const noexcept {
        return *m_left;
    }

    const Expression &BinaryExpression::right() const noexcept {
        return *m_right;
    }

    bool BinaryExpression::equals(const Expression &other) const noexcept {
        const auto& binOther = static_cast<const BinaryExpression &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return op() == binOther.op() &&
            left() == binOther.left() &&
            right() == binOther.right();
    }

    ComparisonExpression::ComparisonExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    const Token &ComparisonExpression::op() const noexcept {
        return m_op;
    }

    const Expression &ComparisonExpression::left() const noexcept {
        return *m_left;
    }

    const Expression &ComparisonExpression::right() const noexcept {
        return *m_right;
    }

    bool ComparisonExpression::equals(const Expression &other) const noexcept {
        const auto &cmpOther = static_cast<const ComparisonExpression &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return op() == cmpOther.op() &&
            left() == cmpOther.left() &&
            right() == cmpOther.right();
    }

    UnaryExpression::UnaryExpression(Token op, ExpressionPtr operand, bool isPrefix) noexcept :
        m_op(std::move(op)), m_operand(std::move(operand)), m_isPrefix(isPrefix) {
    }

    const Token &UnaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &UnaryExpression::operand() const noexcept {
        return *m_operand;
    }

    bool UnaryExpression::isPrefix() const noexcept {
        return m_isPrefix;
    }

    bool UnaryExpression::equals(const Expression &other) const noexcept {
        const auto &unaryOther = static_cast<const UnaryExpression &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return op() == unaryOther.op() &&
            operand() == unaryOther.operand() &&
            isPrefix() == unaryOther.isPrefix();
    }

    VariableExpression::VariableExpression(Token name) noexcept :
        m_name(std::move(name)) {
    }

    const Token &VariableExpression::name() const noexcept {
        return m_name;
    }

    bool VariableExpression::equals(const Expression &other) const noexcept {
        const auto &varOther = static_cast<const VariableExpression &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return name() == varOther.name();
    }

    NumberExpression::NumberExpression(Token value, bool isIntLiteral) noexcept :
        m_value(std::move(value)), m_isIntLiteral(isIntLiteral) {
    }

    const Token &NumberExpression::value() const noexcept {
        return m_value;
    }

    bool NumberExpression::isIntLiteral() const noexcept {
        return m_isIntLiteral;
    }

    bool NumberExpression::equals(const Expression &other) const noexcept {
        const auto &numOther = static_cast<const NumberExpression&>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return value() == numOther.value() && isIntLiteral() == numOther.isIntLiteral();
    }

}
