#include "Statement.h"

#include <utility>


namespace es {
    Statement::~Statement() noexcept = default;


    ExpressionStatement::ExpressionStatement(ExpressionPtr expr) noexcept :
        m_expr(std::move(expr)) {
    }

    void ExpressionStatement::accept(StatementVisitor &visitor) const {
        visitor.visitExpr(*this);
    }

    const Expression &ExpressionStatement::expr() const noexcept {
        return *m_expr;
    }

    bool ExpressionStatement::operator==(const Statement &other) const noexcept {
        if (this == &other) return true;
        if (auto exprStmt = dynamic_cast<const ExpressionStatement *>(&other)) {
            return expr() == exprStmt->expr();
        }
        return false;
    }

    bool ExpressionStatement::operator!=(const Statement &other) const noexcept {
        return !(*this == other);
    }


    BlockStatement::BlockStatement(std::vector<StatementPtr> body) noexcept :
        m_body(std::move(body)) {
    }

    void BlockStatement::accept(StatementVisitor &visitor) const {
        visitor.visitBlock(*this);
    }

    const std::vector<StatementPtr> &BlockStatement::body() const noexcept {
        return m_body;
    }

    bool BlockStatement::operator==(const Statement &other) const noexcept {
        if (this == &other) return true;
        if (auto blockStmt = dynamic_cast<const BlockStatement *>(&other)) {
            return body() == blockStmt->body();
        }
        return false;
    }

    bool BlockStatement::operator!=(const Statement &other) const noexcept {
        return !(*this == other);
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

    const std::vector<Token> &FunctionStatement::modifiers() const noexcept {
        return m_modifiers;
    }

    const Token &FunctionStatement::keyword() const noexcept {
        return m_keyword;
    }

    const Token &FunctionStatement::name() const noexcept {
        return m_name;
    }

    const std::vector<Parameter> &FunctionStatement::params() const noexcept {
        return m_params;
    }

    const Type &FunctionStatement::returnType() const noexcept {
        return m_returnType;
    }

    const Statement &FunctionStatement::body() const noexcept {
        return *m_body;
    }

    bool FunctionStatement::operator==(const Statement &other) const noexcept {
        if (this == &other) return true;
        if (auto funcStmt = dynamic_cast<const FunctionStatement *>(&other)) {
            return modifiers() == funcStmt->modifiers() && keyword() == funcStmt->keyword() &&
                name() == funcStmt->name() && params() == funcStmt->params() &&
                returnType() == funcStmt->returnType() && body() == funcStmt->body();
        }
        return false;
    }

    bool FunctionStatement::operator!=(const Statement &other) const noexcept {
        return !(*this == other);
    }


    StatementVisitor::~StatementVisitor() noexcept = default;
}
