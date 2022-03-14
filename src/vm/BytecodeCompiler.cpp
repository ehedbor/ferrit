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

        for (const auto &stmt : ast) {
            stmt->accept(*this);
        }

        const Statement &lastStmt = *ast.back();
        emit(OpCode::Return, lastStmt.errorToken().location.line);
        return m_chunk;
    }

    VisitResult BytecodeCompiler::visitFunctionDecl(const FunctionDeclaration &funDecl) {
        throw makeError<CompileError::NotImplemented>(
            funDecl.errorToken(), "functions");
    }

    VisitResult BytecodeCompiler::visitConditionalStmt(const ConditionalStatement &conditionalStmt) {
        auto conditionType = std::any_cast<RuntimeType>(conditionalStmt.condition().accept(*this));
        if (conditionType != RuntimeType::BoolType) {
            throw makeError<CompileError::IncompatibleTypes>(
                conditionalStmt.condition().errorToken(), "if statement", std::vector{conditionType.name()});
        }
        int conditionPos = emitJump(true, conditionalStmt.ifKeyword().location.line);

        conditionalStmt.ifBody().accept(*this);
        int elsePos = -1;
        if (conditionalStmt.elseBody()) {
            elsePos = emitJump(false, conditionalStmt.elseKeyword()->location.line);
        }

        patchJump(conditionPos);
        if (conditionalStmt.elseBody()) {
            conditionalStmt.elseBody()->accept(*this);
            patchJump(elsePos);
        }

        return RuntimeType::NothingType;
    }

    VisitResult BytecodeCompiler::visitBlockStmt(const BlockStatement &blockStmt) {
        for (const auto &stmt: blockStmt.body()) {
            stmt->accept(*this);
        }
        return RuntimeType::NothingType;
    }

    VisitResult BytecodeCompiler::visitExpressionStmt(const ExpressionStatement &exprStmt) {
        exprStmt.expr().accept(*this);
        emit(OpCode::Pop, exprStmt.errorToken().location.line);
        return RuntimeType::NothingType;
    }

    VisitResult BytecodeCompiler::visitBinaryExpr(const BinaryExpression &binExpr) {
        auto leftType = std::any_cast<RuntimeType>(binExpr.left().accept(*this));
        auto rightType = std::any_cast<RuntimeType>(binExpr.right().accept(*this));

        int line = binExpr.op().location.line;
        switch (binExpr.op().type) {
        case TokenType::Plus:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IAdd, line);
                return RuntimeType::IntType;
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FAdd, line);
                return RuntimeType::RealType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'+'", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::Minus:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::ISubtract, line);
                return RuntimeType::IntType;
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FSubtract, line);
                return RuntimeType::RealType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'-'", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::Asterisk:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IMultiply, line);
                return RuntimeType::IntType;
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FMultiply, line);
                return RuntimeType::RealType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'*'", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::Slash:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IDivide, line);
                return RuntimeType::IntType;
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FDivide, line);
                return RuntimeType::RealType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'/'", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::Tilde:
            throw makeError<CompileError::NotImplemented>(
                binExpr.errorToken(), "concatenation operator");
        case TokenType::Percent:
            if (leftType == RuntimeType::IntType && rightType == RuntimeType::IntType) {
                emit(OpCode::IAdd, line);
                return RuntimeType::IntType;
            } else if (leftType == RuntimeType::RealType && rightType == RuntimeType::RealType) {
                emit(OpCode::FAdd, line);
                return RuntimeType::RealType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'%'", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::AndAnd:
            if (leftType == RuntimeType::BoolType && rightType == RuntimeType::BoolType) {
                emit(OpCode::BAnd, line);
                return RuntimeType::BoolType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'&&'", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::OrOr:
            if (leftType == RuntimeType::BoolType && rightType == RuntimeType::BoolType) {
                emit(OpCode::BOr, line);
                return RuntimeType::BoolType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    binExpr.errorToken(), "'&&'", std::vector{leftType.name(), rightType.name()});
            }
        default:
            throw CompileException(
                std::format("unknown operator '{}' ({})",
                    binExpr.op().lexeme,
                    binExpr.op().type));
        }
    }

    VisitResult BytecodeCompiler::visitComparisonExpr(const ComparisonExpression &cmpExpr) {
        auto leftType = std::any_cast<RuntimeType>(cmpExpr.left().accept(*this));
        auto rightType = std::any_cast<RuntimeType>(cmpExpr.right().accept(*this));

        int line = cmpExpr.op().location.line;
        switch (cmpExpr.op().type) {
        case TokenType::EqualEqual:
            if (leftType == RuntimeType::BoolType && rightType == RuntimeType::BoolType) {
                emit(OpCode::BEqual, line);
                return RuntimeType::BoolType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    cmpExpr.errorToken(), "'=='", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::BangEqual:
            if (leftType == RuntimeType::BoolType && rightType == RuntimeType::BoolType) {
                emit(OpCode::BNotEqual, line);
                return RuntimeType::BoolType;
            } else {
                throw makeError<CompileError::IncompatibleTypes>(
                    cmpExpr.errorToken(), "'!='", std::vector{leftType.name(), rightType.name()});
            }
        case TokenType::GreaterEqual:
        case TokenType::Greater:
        case TokenType::LessEqual:
        case TokenType::Less:
            throw makeError<CompileError::NotImplemented>(
                cmpExpr.errorToken(), "comparisons");
        default:
            throw CompileException(
                std::format("unknown operator '{}' ({})",
                    cmpExpr.op().lexeme,
                    cmpExpr.op().type));
        }
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
            if (type != RuntimeType::BoolType) {
                throw makeError<CompileError::IncompatibleTypes>(
                    unaryExpr.errorToken(), "'!'", std::vector{type.name()});
            } else {
                emit(OpCode::BNot, unaryExpr.op().location.line);
                return type;
            }
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

    VisitResult BytecodeCompiler::visitBoolExpr(const BooleanExpression &boolExpr) {
        bool booleanValue;
        if (boolExpr.value().type == TokenType::True) {
            booleanValue = true;
        } else if (boolExpr.value().type == TokenType::False) {
            booleanValue = false;
        } else {
            throw CompileException("Unknown boolean literal kind.");
        }

        Value value{booleanValue};
        emitConstant(value, boolExpr.value().location.line);
        return value.runtimeType();
    }

    void BytecodeCompiler::emit(OpCode opCode, int line) {
        m_chunk.writeInstruction(opCode, line);
    }

    void BytecodeCompiler::emit(OpCode opCode, std::uint8_t arg, int line) {
        m_chunk.writeInstruction(opCode, arg, line);
    }

    int BytecodeCompiler::emitJump(bool isConditionalJump, int line) {
        OpCode opCode = isConditionalJump ? OpCode::JumpIfFalse : OpCode::Jump;
        m_chunk.writeInstruction(opCode, static_cast<std::uint16_t>(0xDEAD), line);

        return m_chunk.size() - 2;
    }

    void BytecodeCompiler::patchJump(int jumpOpOffset) {
        // subtract 2 to account for the instruction's parameter
        int offset = m_chunk.size() - jumpOpOffset - 2;
        if (offset < 0) {
            throw CompileException("Negative offset in jump instruction.");
        } else if (offset > std::numeric_limits<std::uint16_t>::max()) {
            throw CompileException(std::format("Jump of {} bytes too big.", offset));
        }

        m_chunk.patchShort(jumpOpOffset, static_cast<std::uint16_t>(offset));
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
