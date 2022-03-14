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
        DeclaredType returnType,
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

    const DeclaredType &FunctionDeclaration::returnType() const noexcept {
        return m_returnType;
    }

    const Statement *FunctionDeclaration::body() const noexcept {
        return m_body ? &*m_body : nullptr;
    }

    const Token &FunctionDeclaration::errorToken() const noexcept {
        return keyword();
    }

    bool FunctionDeclaration::equals(const Statement &other) const noexcept {
        const auto &otherFun = static_cast<const FunctionDeclaration &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return name() == otherFun.name() &&
            params() == otherFun.params() &&
            returnType() == otherFun.returnType() &&
            keyword() == otherFun.keyword() &&
            modifiers() == otherFun.modifiers() &&
            ((body() == nullptr && otherFun.body() == nullptr) ||
                (*body() == *otherFun.body()));
    }

    ConditionalStatement::ConditionalStatement(
        Token ifKeyword, ExpressionPtr condition, StatementPtr ifBody,
        std::optional<Token> elseKeyword, std::optional<StatementPtr> elseBody) :
        m_keyword{std::move(ifKeyword)}, m_condition{std::move(condition)}, m_ifBody{std::move(ifBody)},
        m_elseKeyword{std::move(elseKeyword)},m_elseBody{elseBody.has_value() ? std::move(elseBody.value()) : nullptr} {
    }

    const Token &ConditionalStatement::ifKeyword() const noexcept {
        return m_keyword;
    }

    const Expression &ConditionalStatement::condition() const noexcept {
        return *m_condition;
    }

    const Statement &ConditionalStatement::ifBody() const noexcept {
        return *m_ifBody;
    }

    const std::optional<Token> &ConditionalStatement::elseKeyword() const noexcept {
        return m_elseKeyword;
    }

    const Statement *ConditionalStatement::elseBody() const noexcept {
        return m_elseBody.get();
    }

    const Token &ConditionalStatement::errorToken() const noexcept {
        return ifKeyword();
    }

    bool ConditionalStatement::equals(const Statement &other) const noexcept {
        const auto &otherCond = static_cast<const ConditionalStatement &>(other);  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return ifKeyword() == otherCond.ifKeyword() &&
            condition() == otherCond.condition() &&
            ifBody() == otherCond.ifBody() &&
            elseKeyword() == otherCond.elseKeyword() &&
            ((elseBody() == nullptr && otherCond.elseBody() == nullptr) ||
                (*elseBody() == *otherCond.elseBody()));
    }

    BlockStatement::BlockStatement(Token brace, std::vector<StatementPtr> body) noexcept :
        m_brace(std::move(brace)), m_body(std::move(body)) {
    }

    const Token &BlockStatement::brace() const noexcept {
        return m_brace;
    }

    const std::vector<StatementPtr> &BlockStatement::body() const noexcept {
        return m_body;
    }

    const Token &BlockStatement::errorToken() const noexcept {
        return brace();
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

    const Token &ExpressionStatement::errorToken() const noexcept {
        return expr().errorToken();
    }

    bool ExpressionStatement::equals(const Statement &other) const noexcept {
        const auto &otherStmt = static_cast<const ExpressionStatement &>(other); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
        return expr() == otherStmt.expr();
    }
}
