#pragma once

#include "AstElements.h"
#include "Expression.h"
#include "Token.h"
#include "Visitor.h"

#include <memory>
#include <optional>
#include <vector>


namespace ferrit {
    class Statement;
    class FunctionDeclaration;
    class BlockStatement;
    class ExpressionStatement;

    using StatementPtr = std::unique_ptr<Statement>;

    /**
     * Allows for traversal of a hierarchy of \c Statement nodes.
     *
     * @see ExpressionVisitor
     */
    class StatementVisitor {
    public:
        virtual ~StatementVisitor() noexcept = default;

        virtual VisitResult visitFunctionDecl(const FunctionDeclaration &funDecl) = 0;
        virtual VisitResult visitBlockStmt(const BlockStatement &blockStmt) = 0;
        virtual VisitResult visitExpressionStmt(const ExpressionStatement &exprStmt) = 0;
    };

    /**
     * Base class for any AST construct that can be compiled, but does not produce a value.
     */
    class Statement {
    public:
        virtual ~Statement() noexcept = default;

        MAKE_BASE_VISITABLE(StatementVisitor);

        bool operator==(const Statement &other) const noexcept;

    protected:
        [[nodiscard]] virtual bool equals(const Statement &other) const noexcept = 0;
    };

    /**
     * Represents a function declaration.
     */
    class FunctionDeclaration final : public Statement {
    public:
        FunctionDeclaration(
            std::vector<Token> modifiers,
            Token keyword,
            Token name,
            std::vector<Parameter> params,
            DeclaredType returnType,
            std::optional<StatementPtr> body = {}) noexcept;

        [[nodiscard]] const std::vector<Token> &modifiers() const noexcept;
        [[nodiscard]] const Token &keyword() const noexcept;
        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const std::vector<Parameter> &params() const noexcept;
        [[nodiscard]] const DeclaredType &returnType() const noexcept;
        [[nodiscard]] const Statement *body() const noexcept;

        MAKE_VISITABLE(StatementVisitor, FunctionDecl);

    protected:
        [[nodiscard]] bool equals(const Statement &other) const noexcept override;

    private:
        std::vector<Token> m_modifiers;
        Token m_keyword;
        Token m_name;
        std::vector<Parameter> m_params;
        DeclaredType m_returnType;
        StatementPtr m_body;
    };

    /**
     * Represents a group of statements.
     */
    class BlockStatement final : public Statement {
    public:
        explicit BlockStatement(std::vector<StatementPtr> body) noexcept;

        [[nodiscard]] const std::vector<StatementPtr> &body() const noexcept;

        MAKE_VISITABLE(StatementVisitor, BlockStmt);

    protected:
        [[nodiscard]] bool equals(const Statement &other) const noexcept override;

    private:
        std::vector<StatementPtr> m_body;
    };

    /**
     * Represents an expression whose value is unused (for instance, a function call in a block).
     */
    class ExpressionStatement final : public Statement {
    public:
        explicit ExpressionStatement(ExpressionPtr expr) noexcept;

        [[nodiscard]] const Expression &expr() const noexcept;

        MAKE_VISITABLE(StatementVisitor, ExpressionStmt);

    protected:
        [[nodiscard]] bool equals(const Statement &other) const noexcept override;

    private:
        ExpressionPtr m_expr;
    };
}
