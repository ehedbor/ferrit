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

    const Token &VariableExpression::name() const noexcept {
        return m_name;
    }


    SimpleBinaryExpression::SimpleBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    VisitResult SimpleBinaryExpression::accept(ExpressionVisitor &visitor) const {
        return visitor.visitBinary(*this);
    }

    bool SimpleBinaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const SimpleBinaryExpression *>(&other)) {
            return op() == binaryExpr->op() &&
                left() == binaryExpr->left() && right() == binaryExpr->right();
        }
        return false;
    }

    bool SimpleBinaryExpression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
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


    BitwiseBinaryExpression::BitwiseBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    VisitResult BitwiseBinaryExpression::accept(ExpressionVisitor &visitor) const {
        return visitor.visitBinary(*this);
    }

    bool BitwiseBinaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const BitwiseBinaryExpression *>(&other)) {
            return op() == binaryExpr->op() &&
                left() == binaryExpr->left() && right() == binaryExpr->right();
        }
        return false;
    }

    bool BitwiseBinaryExpression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
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


    CompareBinaryExpression::CompareBinaryExpression(Token op, ExpressionPtr left, ExpressionPtr right) noexcept :
        m_op(std::move(op)), m_left(std::move(left)), m_right(std::move(right)) {
    }

    VisitResult CompareBinaryExpression::accept(ExpressionVisitor &visitor) const {
        return visitor.visitBinary(*this);
    }

    bool CompareBinaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const CompareBinaryExpression *>(&other)) {
            return op() == binaryExpr->op() &&
                left() == binaryExpr->left() && right() == binaryExpr->right();
        }
        return false;
    }

    bool CompareBinaryExpression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
    }

    const Token &CompareBinaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &CompareBinaryExpression::left() const noexcept {
        return *m_left;
    }

    const Expression &CompareBinaryExpression::right() const noexcept {
        return *m_right;
    }


    UnaryExpression::UnaryExpression(Token op, ExpressionPtr operand) noexcept :
        m_op(std::move(op)), m_operand(std::move(operand)) {
    }

    VisitResult UnaryExpression::accept(ExpressionVisitor &visitor) const {
        return visitor.visitUnary(*this);
    }

    bool UnaryExpression::operator==(const Expression &other) const noexcept {
        if (this == &other) return true;
        if (auto *binaryExpr = dynamic_cast<const UnaryExpression *>(&other)) {
            return op() == binaryExpr->op() && operand() == binaryExpr->operand();
        }
        return false;
    }

    bool UnaryExpression::operator!=(const Expression &other) const noexcept {
        return !(*this == other);
    }

    const Token &UnaryExpression::op() const noexcept {
        return m_op;
    }

    const Expression &UnaryExpression::operand() const noexcept {
        return *m_operand;
    }


    ExpressionVisitor::~ExpressionVisitor() noexcept = default;
}
