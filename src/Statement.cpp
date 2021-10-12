#include "Statement.h"

#include <utility>


namespace ferrit {
    bool Statement::operator==(const Statement &other) const noexcept {
        if (this == &other) {
            return true;
        } else if (typeid(*this) != typeid(other)) {
            return false;
        } else {
            return this->equals(other);
        }
    }
    
    FunctionDeclaration::FunctionDeclaration(
        std::vector<Token> modifiers,
        Token keyword,
        Token name,
        std::vector<Parameter> params,
        Type returnType,
        std::optional<StatementPtr> body) noexcept :
        m_modifiers(std::move(modifiers)),
        m_keyword(std::move(keyword)),
        m_name(std::move(name)),
        m_params(std::move(params)),
        m_returnType(std::move(returnType)),
        m_body(body.has_value() ? std::move(body.value()) : nullptr) {
    }

    const std::vector<Token> &FunctionDeclaration::modifiers() const noexcept {
        return m_modifiers;
    }

    const Token &FunctionDeclaration::keyword() const noexcept {
        return m_keyword;
    }

    const Token &FunctionDeclaration::name() const noexcept {
        return m_name;
    }

    const std::vector<Parameter> &FunctionDeclaration::params() const noexcept {
        return m_params;
    }

    const Type &FunctionDeclaration::returnType() const noexcept {
        return m_returnType;
    }

    const Statement *FunctionDeclaration::body() const noexcept {
        return m_body ? &*m_body : nullptr;
    }

    bool FunctionDeclaration::equals(const Statement &other) const noexcept {
        const auto &otherFun = static_cast<const FunctionDeclaration &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return name() == otherFun.name() &&
            params() == otherFun.params() &&
            returnType() == otherFun.returnType() &&
            keyword() == otherFun.keyword() &&
            modifiers() == otherFun.modifiers() &&
            *body() == *otherFun.body();
    }

    BlockStatement::BlockStatement(std::vector<StatementPtr> body) noexcept :
        m_body(std::move(body)) {
    }

    const std::vector<StatementPtr> &BlockStatement::body() const noexcept {
        return m_body;
    }

    bool BlockStatement::equals(const Statement &other) const noexcept {
        const auto &otherBlock = static_cast<const BlockStatement &>(other);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return body() == otherBlock.body();
    }

    ExpressionStatement::ExpressionStatement(ExpressionPtr expr) noexcept :
        m_expr(std::move(expr)) {
    }

    const Expression &ExpressionStatement::expr() const noexcept {
        return *m_expr;
    }

    bool ExpressionStatement::equals(const Statement &other) const noexcept {
        const auto &otherStmt = static_cast<const ExpressionStatement &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return expr() == otherStmt.expr();
    }
}
