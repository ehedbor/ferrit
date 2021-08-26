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

    class Statement {
    public:
        virtual ~Statement() noexcept = 0;

        virtual VisitResult accept(StatementVisitor &visitor) const = 0;

        virtual bool operator==(const Statement &other) const noexcept = 0;
        virtual bool operator!=(const Statement &other) const noexcept = 0;
    };

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

    class BlockStatement : public Statement {
    public:
        explicit BlockStatement(std::vector<StatementPtr> body) noexcept;

        VisitResult accept(StatementVisitor &visitor) const override;

        [[nodiscard]] const std::vector<StatementPtr> &body() const noexcept;

        bool operator==(const Statement &other) const noexcept override;
        bool operator!=(const Statement &other) const noexcept override;

    private:
        std::vector<StatementPtr> m_body;
    };

    class FunctionStatement : public Statement {
    public:
        FunctionStatement(
            std::vector<Token> modifiers, Token keyword, Token name,
            std::vector<Parameter> params, Type returnType, std::optional<StatementPtr> body);

        VisitResult accept(StatementVisitor &visitor) const override;

        [[nodiscard]] const std::vector<Token> &modifiers() const noexcept;
        [[nodiscard]] const Token &keyword() const noexcept;
        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const std::vector<Parameter> &params() const noexcept;
        [[nodiscard]] const Type &returnType() const noexcept;
        [[nodiscard]] std::optional<const Statement *> body() const noexcept;

        bool operator==(const Statement &other) const noexcept override;
        bool operator!=(const Statement &other) const noexcept override;

    private:
        std::vector<Token> m_modifiers;
        Token m_keyword;
        Token m_name;
        std::vector<Parameter> m_params;
        Type m_returnType;
        std::optional<StatementPtr> m_body;
    };

    class StatementVisitor {
    public:
        virtual ~StatementVisitor() noexcept = 0;

        virtual VisitResult visitExprStmt(const ExpressionStatement &stmt) = 0;
        virtual VisitResult visitBlock(const BlockStatement &stmt) = 0;
        virtual VisitResult visitFunction(const FunctionStatement &stmt) = 0;
    };
}
