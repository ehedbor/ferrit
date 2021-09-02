#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "AstElements.h"
#include "Expression.h"
#include "Token.h"
#include "Visitor.h"

namespace ferrit {
    class Statement;
    class FunctionDeclaration;
    class FunctionDefinition;
    class Block;
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

        virtual VisitResult visitFunDeclaration(const FunctionDeclaration &funDecl) = 0;
        virtual VisitResult visitFunDefinition(const FunctionDefinition &funDef) = 0;
        virtual VisitResult visitBlock(const Block &block) = 0;
        virtual VisitResult visitExprStmt(const ExpressionStatement &exprStmt) = 0;
    };

    /**
     * Base class for any AST construct that can be compiled, but does not produce a value.
     */
    class Statement {
    public:
        virtual ~Statement() noexcept = default;

        MAKE_BASE_VISITABLE(StatementVisitor);

        virtual bool operator==(const Statement &other) const noexcept = 0;
        virtual bool operator!=(const Statement &other) const noexcept;
    };

    /**
     * Represents a function's forward-declaration.
     *
     * @see FunctionDefinition
     */
    class FunctionDeclaration : public Statement {
    public:
        FunctionDeclaration(
            std::vector<Token> modifiers, Token keyword, Token name,
            std::vector<Parameter> params, Type returnType) noexcept;

        [[nodiscard]] const std::vector<Token> &modifiers() const noexcept;
        [[nodiscard]] const Token &keyword() const noexcept;
        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const std::vector<Parameter> &params() const noexcept;
        [[nodiscard]] const Type &returnType() const noexcept;

        MAKE_VISITABLE(StatementVisitor, FunDeclaration);
        bool operator==(const Statement &other) const noexcept override;

    private:
        std::vector<Token> m_modifiers;
        Token m_keyword;
        Token m_name;
        std::vector<Parameter> m_params;
        Type m_returnType;
    };

    /**
     * Represents a function definition.
     *
     * @see FunctionDeclaration
     */
    class FunctionDefinition : public Statement {
    public:
        FunctionDefinition(std::unique_ptr<FunctionDeclaration> declaration, StatementPtr body);

        [[nodiscard]] const FunctionDeclaration &declaration() const noexcept;
        [[nodiscard]] const Statement &body() const noexcept;

        MAKE_VISITABLE(StatementVisitor, FunDefinition);
        bool operator==(const Statement &other) const noexcept override;

    private:
        std::unique_ptr<FunctionDeclaration> m_declaration;
        StatementPtr m_body;
    };

    /**
     * Represents a group of statements.
     */
    class Block : public Statement {
    public:
        explicit Block(std::vector<StatementPtr> body) noexcept;

        [[nodiscard]] const std::vector<StatementPtr> &body() const noexcept;

        MAKE_VISITABLE(StatementVisitor, Block);
        bool operator==(const Statement &other) const noexcept override;

    private:
        std::vector<StatementPtr> m_body;
    };

    /**
     * Represents an expression whose value is unused (for instance, a function call in a block).
     */
    class ExpressionStatement : public Statement {
    public:
        explicit ExpressionStatement(ExpressionPtr expr) noexcept;

        [[nodiscard]] const Expression &expr() const noexcept;

        MAKE_VISITABLE(StatementVisitor, ExprStmt);
        bool operator==(const Statement &other) const noexcept override;

    private:
        ExpressionPtr m_expr;
    };
}
