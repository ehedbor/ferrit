#include "Statement.h"

#include <utility>


namespace es {
    bool Statement::operator!=(const Statement &other) const noexcept {
        return !(*this == other);
    }

    FunctionDeclaration::FunctionDeclaration(
        std::vector<Token> modifiers, Token keyword, Token name,
        std::vector<Parameter> params, Type returnType) noexcept :
        m_modifiers(std::move(modifiers)), m_keyword(std::move(keyword)), m_name(std::move(name)),
        m_params(std::move(params)), m_returnType(std::move(returnType)) {
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

    bool FunctionDeclaration::operator==(const Statement &other) const noexcept {
        if (this == &other) return true;
        if (auto decl = dynamic_cast<const FunctionDeclaration *>(&other)) {
            return modifiers() == decl->modifiers() && keyword() == decl->keyword() &&
                name() == decl->name() && params() == decl->params() && returnType() == decl->returnType();
        }
        return false;
    }

    FunctionDefinition::FunctionDefinition(std::unique_ptr<FunctionDeclaration> declaration, StatementPtr body) :
        m_declaration(std::move(declaration)), m_body(std::move(body)) {
    }

    const FunctionDeclaration &FunctionDefinition::declaration() const noexcept {
        return *m_declaration;
    }

    const Statement &FunctionDefinition::body() const noexcept {
        return *m_body;
    }

    bool FunctionDefinition::operator==(const Statement &other) const noexcept {
        if (this == &other) return true;
        if (auto def = dynamic_cast<const FunctionDefinition *>(&other)) {
            return declaration() == def->declaration() && body() == def->body();
        }
        return false;
    }

    Block::Block(std::vector<StatementPtr> body) noexcept :
        m_body(std::move(body)) {
    }

    const std::vector<StatementPtr> &Block::body() const noexcept {
        return m_body;
    }

    bool Block::operator==(const Statement &other) const noexcept {
        if (this == &other) return true;
        if (auto blockStmt = dynamic_cast<const Block *>(&other)) {
            return body() == blockStmt->body();
        }
        return false;
    }

    ExpressionStatement::ExpressionStatement(ExpressionPtr expr) noexcept :
        m_expr(std::move(expr)) {
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
}
