#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "AstElements.h"
#include "Expression.h"
#include "Token.h"
#include "Visitor.h"

namespace es {
    class StatementVisitor;
    class Statement;
    using StatementPtr = std::unique_ptr<Statement>;

    /**
     * Base class for any AST construct that can NOT produce a value.
     */
    class Statement {
    public:
        virtual ~Statement() noexcept = 0;

        virtual VisitResult accept(StatementVisitor &visitor) const = 0;

        virtual bool operator==(const Statement &other) const noexcept = 0;
        virtual bool operator!=(const Statement &other) const noexcept = 0;
    };

    /**
     * Represents an expression whose value is unused (for instance, a function call in a block).
     */
    class ExpressionStatement : public Statement {
    public:
        explicit ExpressionStatement(ExpressionPtr expr) noexcept;

        VisitResult accept(StatementVisitor &visitor) const override;

        [[nodiscard]] const Expression &expr() const noexcept;

        bool operator==(const Statement &other) const noexcept override;
        bool operator!=(const Statement &other) const noexcept override;

    private:
        ExpressionPtr m_expr;
    };

    /**
     * Represents a group of statements.
     */
    class Block : public Statement {
    public:
        explicit Block(std::vector<StatementPtr> body) noexcept;

        VisitResult accept(StatementVisitor &visitor) const override;

        [[nodiscard]] const std::vector<StatementPtr> &body() const noexcept;

        bool operator==(const Statement &other) const noexcept override;
        bool operator!=(const Statement &other) const noexcept override;

    private:
        std::vector<StatementPtr> m_body;
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

        VisitResult accept(StatementVisitor &visitor) const override;

        [[nodiscard]] const std::vector<Token> &modifiers() const noexcept;
        [[nodiscard]] const Token &keyword() const noexcept;
        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const std::vector<Parameter> &params() const noexcept;
        [[nodiscard]] const Type &returnType() const noexcept;

        bool operator==(const Statement &other) const noexcept override;
        bool operator!=(const Statement &other) const noexcept override;

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
     * @note In es, functions do not need to be forward-declared (though they
     * can be if desired). However, code to call a function cannot be generated
     * unless the function is already known to exist. To support this, the
     * intermediate resolution step will "hoist" declarations to the top of the
     * file, creating a standalone @c FunctionDeclaration node.
     *
     * @see FunctionDeclaration
     */
    class FunctionDefinition : public Statement {
    public:
        FunctionDefinition(std::unique_ptr<FunctionDeclaration> declaration, StatementPtr body);

        VisitResult accept(StatementVisitor &visitor) const override;

        [[nodiscard]] const FunctionDeclaration &declaration() const noexcept;
        [[nodiscard]] const Statement &body() const noexcept;

        bool operator==(const Statement &other) const noexcept override;
        bool operator!=(const Statement &other) const noexcept override;

    private:
        std::unique_ptr<FunctionDeclaration> m_declaration;
        StatementPtr m_body;
    };


    /**
     * Allows for traversal of a hierarchy of \c Statement nodes.
     *
     * @see ExpressionVisitor
     */
    class StatementVisitor {
    public:
        virtual ~StatementVisitor() noexcept = 0;

        virtual VisitResult visitExprStmt(const ExpressionStatement &exprStmt) = 0;
        virtual VisitResult visitBlock(const Block &block) = 0;
        virtual VisitResult visitFunDefinition(const FunctionDefinition &def) = 0;
        virtual VisitResult visitFunDeclaration(const FunctionDeclaration &decl) = 0;
    };
}
