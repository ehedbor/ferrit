#include "BytecodeCompiler.h"

#include <sstream>

namespace ferrit {
    BytecodeCompiler::BytecodeCompiler(std::shared_ptr<const ErrorReporter> errorReporter) :
        m_errorReporter{std::move(errorReporter)} {
    }

    std::optional<Chunk> BytecodeCompiler::compile(const std::vector<StatementPtr> &ast) {
        try {
            return tryCompile(ast);
        } catch (const Error &) {
            return {};
        }
    }

    Chunk BytecodeCompiler::tryCompile(const std::vector<StatementPtr> &ast) {
        m_chunk = Chunk{};

        // TODO: support statements
        if (ast.empty()) {
            Token dummy{TokenType::EndOfFile, "", {1, 1}};
            throw makeError<CompileError::NotImplemented>(dummy, "empty programs");
        } else if (ast.size() > 1) {
            throw makeError<CompileError::NotImplemented>(ast[1]->errorToken(), "multiple lines");
        }

        const Statement &stmt = *ast.front();
        if (const auto *expr = dynamic_cast<const ExpressionStatement *>(&stmt)) {
            expr->accept(*this);
            emit(OpCode::Return, expr->errorToken().location.line);
            return m_chunk;
        } else {
            throw makeError<CompileError::NotImplemented>(stmt.errorToken(), "statements");
        }
    }

    VisitResult BytecodeCompiler::visitFunctionDecl(const FunctionDeclaration &funDecl) {
        throw makeError<CompileError::NotImplemented>(
            funDecl.errorToken(), "functions");
    }

    VisitResult BytecodeCompiler::visitBlockStmt(const BlockStatement &blockStmt) {
        throw makeError<CompileError::NotImplemented>(
            blockStmt.errorToken(), "block statements");
    }

    VisitResult BytecodeCompiler::visitExpressionStmt(const ExpressionStatement &exprStmt) {
        return exprStmt.expr().accept(*this);
    }

    VisitResult BytecodeCompiler::visitBinaryExpr(const BinaryExpression &binExpr) {
        auto leftType = std::any_cast<RuntimeType>(binExpr.left().accept(*this));
        auto rightType = std::any_cast<RuntimeType>(binExpr.right().accept(*this));

        int line = binExpr.op().location.line;
        switch (binExpr.op().type) {
        case TokenType::Plus:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IAdd, line);
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FAdd, line);
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'+'", std::vector{leftType.name(), rightType.name()});
            }
            return leftType;
        case TokenType::Minus:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::ISubtract, line);
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FSubtract, line);
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'-'", std::vector{leftType.name(), rightType.name()});
            }
            return leftType;
        case TokenType::Asterisk:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IMultiply, line);
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FMultiply, line);
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'*'", std::vector{leftType.name(), rightType.name()});
            }
            return leftType;
        case TokenType::Slash:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IDivide, line);
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FDivide, line);
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'/'", std::vector{leftType.name(), rightType.name()});
            }
            return leftType;
        case TokenType::Tilde:
            throw makeError<CompileError::NotImplemented>(
                binExpr.errorToken(), "concatenation operator");
        case TokenType::Percent:
            if (leftType == RuntimeType::IntType) {
                emit(OpCode::IAdd, line);
            } else if (leftType == RuntimeType::RealType) {
                emit(OpCode::FAdd, line);
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'%'", std::vector{leftType.name(), rightType.name()});
            }
            return leftType;
        case TokenType::AndAnd:
            throw makeError<CompileError::NotImplemented>(
                binExpr.errorToken(), "logical and operator");
        case TokenType::OrOr:
            throw makeError<CompileError::NotImplemented>(
                binExpr.errorToken(), "logical or operator");
        default:
            throw CompileException(
                std::format("unknown operator '{}' ({})",
                    binExpr.op().lexeme,
                    binExpr.op().type));
        }
    }

    VisitResult BytecodeCompiler::visitComparisonExpr(const ComparisonExpression &cmpExpr) {
        throw makeError<CompileError::NotImplemented>(
            cmpExpr.errorToken(), "comparisons");
    }

    VisitResult BytecodeCompiler::visitUnaryExpr(const UnaryExpression &unaryExpr) {
        auto type = std::any_cast<RuntimeType>(unaryExpr.operand().accept(*this));

        TokenType opType = unaryExpr.op().type;
        switch (opType) {
        case TokenType::Plus:
            if (type == RuntimeType::IntType || type == RuntimeType::RealType) {
                // unary plus does literally nothing, so emit no bytecode.
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    unaryExpr.errorToken(), "'+'", std::vector{type.name()});
            }
            return type;
        case TokenType::Minus:
            if (type == RuntimeType::IntType) {
                emit(OpCode::INegate, unaryExpr.op().location.line);
            } else if (type == RuntimeType::RealType) {
                emit(OpCode::FNegate, unaryExpr.op().location.line);
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    unaryExpr.errorToken(), "'-'", std::vector{type.name()});
            }
            return type;
        case TokenType::Tilde:
            throw makeError<CompileError::NotImplemented>(
                unaryExpr.errorToken(), "concatenation operator");
        case TokenType::Bang:
            throw makeError<CompileError::NotImplemented>(
                unaryExpr.errorToken(), "logical not operator");
        case TokenType::PlusPlus:
            throw makeError<CompileError::NotImplemented>(
                unaryExpr.errorToken(), "increment operators");
        case TokenType::MinusMinus:
            throw makeError<CompileError::NotImplemented>(
                unaryExpr.errorToken(), "decrement operators");
        default:
            throw CompileException{
                std::format("unknown unary operator '{}' ({})",
                    unaryExpr.op().lexeme,
                    unaryExpr.op().type)};
        }
    }

    VisitResult BytecodeCompiler::visitCallExpr(const CallExpression &callExpr) {
        throw makeError<CompileError::NotImplemented>(
            callExpr.errorToken(), "function calls");
    }

    VisitResult BytecodeCompiler::visitVariableExpr(const VariableExpression &varExpr) {
        throw makeError<CompileError::NotImplemented>(
            varExpr.errorToken(), "variable expressions");
    }

    VisitResult BytecodeCompiler::visitNumberExpr(const NumberExpression &numExpr) {
        Value value = parseNumericLiteral(numExpr);
        emitConstant(value, numExpr.value().location.line);
        return value.runtimeType();
    }

    void BytecodeCompiler::emit(OpCode opCode, int line) {
        m_chunk.writeInstruction(opCode, line);
    }

    void BytecodeCompiler::emit(OpCode opCode, std::uint8_t arg, int line) {
        m_chunk.writeInstruction(opCode, arg, line);
    }

    void BytecodeCompiler::emitConstant(const Value &value, int line) {
        emit(OpCode::Constant, makeConstant(value), line);
    }

    std::uint8_t BytecodeCompiler::makeConstant(const Value &value) {
        std::uint8_t constant = m_chunk.addConstant(value);
        if (m_chunk.constantPool().size() > std::numeric_limits<std::uint8_t>::max()) {
            throw CompileException("Too many constants in one chunk.");
        }
        return constant;
    }

    Value BytecodeCompiler::parseNumericLiteral(const NumberExpression &numExpr) {
        std::string lexeme{numExpr.value().lexeme};
        std::erase(lexeme, '_');

        std::istringstream stream{lexeme};
        if (numExpr.isIntLiteral()) {
            std::int64_t result;
            if (stream >> result && stream.eof()) {
                return Value{result};
            }
        } else {
            double result;
            if (stream >> result && stream.eof()) {
                return Value{result};
            }
        }

        std::string literalType = numExpr.isIntLiteral() ? "integer" : "real";
        if (!stream.eof()) {
            throw CompileException(std::format("could not parse {} literal: unexpected suffix", literalType));
        } else {
            throw CompileException(std::format("could not parse {} literal", literalType));
        }
    }
}
