#pragma once

#include "../ErrorReporter.h"
#include "../Expression.h"
#include "../Statement.h"
#include "Chunk.h"

#include <memory>
#include <optional>


namespace ferrit {
    class BytecodeCompiler : private StatementVisitor, private ExpressionVisitor {
    public:
        explicit BytecodeCompiler(std::shared_ptr<const ErrorReporter> errorReporter);

        std::optional<Chunk> compile(const std::vector<StatementPtr> &ast);

    private:
        VisitResult visitFunctionDecl(const FunctionDeclaration &funDecl) override;
        VisitResult visitBlockStmt(const BlockStatement &blockStmt) override;
        VisitResult visitExpressionStmt(const ExpressionStatement &exprStmt) override;

        VisitResult visitBinaryExpr(const BinaryExpression &binExpr) override;
        VisitResult visitComparisonExpr(const ComparisonExpression &cmpExpr) override;
        VisitResult visitUnaryExpr(const UnaryExpression &unaryExpr) override;
        VisitResult visitCallExpr(const CallExpression &callExpr) override;
        VisitResult visitVariableExpr(const VariableExpression &varExpr) override;
        VisitResult visitNumberExpr(const NumberExpression &numExpr) override;

    private:
        void emit(OpCode opCode, int line);
        void emit(OpCode opCode, std::uint8_t arg, int line);

        void emitConstant(Value value, int line);
        std::uint8_t makeConstant(Value value);

    private:
        std::shared_ptr<const ErrorReporter> m_errorReporter;
        Chunk m_chunk{};
    };
}
