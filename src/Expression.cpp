#include "Expression.h"


namespace es {
    bool Expression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
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

    bool NumberExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *numExpr = dynamic_cast<const NumberExpression *>(&other)) {
            return value() == numExpr->value() && isIntLiteral() == numExpr->isIntLiteral();
        }
        return false;
    }

    VariableExpression::VariableExpression(Token name) noexcept :
        m_name(std::move(name)) {
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

    SimpleBinaryExpression::SimpleBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    const Token &SimpleBinaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &SimpleBinaryExpression::left() const noexcept {
        return *m_left;
    }

    const Expression &SimpleBinaryExpression::right() const noexcept {
        return *m_right;
    }

    bool SimpleBinaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const SimpleBinaryExpression *>(&other)) {
            return op() == binaryExpr->op() &&
                left() == binaryExpr->left() && right() == binaryExpr->right();
        }
        return false;
    }

    BitwiseBinaryExpression::BitwiseBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    const Token &BitwiseBinaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &BitwiseBinaryExpression::left() const noexcept {
        return *m_left;
    }

    const Expression &BitwiseBinaryExpression::right() const noexcept {
        return *m_right;
    }

    bool BitwiseBinaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const BitwiseBinaryExpression *>(&other)) {
            return op() == binaryExpr->op() &&
                left() == binaryExpr->left() && right() == binaryExpr->right();
        }
        return false;
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

    bool ComparisonExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const ComparisonExpression *>(&other)) {
            return op() == binaryExpr->op() &&
                left() == binaryExpr->left() && right() == binaryExpr->right();
        }
        return false;
    }

    UnaryExpression::UnaryExpression(Token op, ExpressionPtr operand) noexcept :
        m_op(std::move(op)), m_operand(std::move(operand)) {
    }

    const Token &UnaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &UnaryExpression::operand() const noexcept {
        return *m_operand;
    }

    bool UnaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const UnaryExpression *>(&other)) {
            return op() == binaryExpr->op() && operand() == binaryExpr->operand();
        }
        return false;
    }
}
