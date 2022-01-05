#include "BytecodeCompiler.h"


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

    VisitResult BytecodeCompiler::visitFunctionDecl(
        [[maybe_unused]] const FunctionDeclaration &funDecl) {
        throw makeError<CompileError::NotImplemented>(
            funDecl.errorToken(), "functions");
    }

    VisitResult BytecodeCompiler::visitBlockStmt(
        [[maybe_unused]] const BlockStatement &blockStmt) {
        throw makeError<CompileError::NotImplemented>(
            blockStmt.errorToken(), "block statements");
    }

    VisitResult BytecodeCompiler::visitExpressionStmt(const ExpressionStatement &exprStmt) {
        return exprStmt.expr().accept(*this);
    }

    VisitResult BytecodeCompiler::visitBinaryExpr(const BinaryExpression &binExpr) {
        binExpr.left().accept(*this);
        binExpr.right().accept(*this);

        int line = binExpr.op().location.line;
        switch (binExpr.op().type) {
        case TokenType::Plus:
            emit(OpCode::Add, line);
            break;
        case TokenType::Minus:
            emit(OpCode::Subtract, line);
            break;
        case TokenType::Asterisk:
            emit(OpCode::Multiply, line);
            break;
        case TokenType::Slash:
            emit(OpCode::Divide, line);
            break;
        case TokenType::Tilde:
            throw makeError<CompileError::NotImplemented>(
                binExpr.errorToken(), "concatenation operator");
        case TokenType::Percent:
            emit(OpCode::Modulus, line);
            break;
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

        return {};
    }

    VisitResult BytecodeCompiler::visitComparisonExpr(
        [[maybe_unused]] const ComparisonExpression &cmpExpr) {
        throw makeError<CompileError::NotImplemented>(
            cmpExpr.errorToken(), "comparisons");
    }

    VisitResult BytecodeCompiler::visitUnaryExpr(const UnaryExpression &unaryExpr) {
        unaryExpr.operand().accept(*this);

        TokenType opType = unaryExpr.op().type;
        switch (opType) {
        case TokenType::Plus:
            // unary plus does literally nothing, so emit no bytecode.
            break;
        case TokenType::Minus:
            emit(OpCode::Negate, unaryExpr.op().location.line);
            break;
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
        return {};
    }

    VisitResult BytecodeCompiler::visitCallExpr(
        [[maybe_unused]] const CallExpression &callExpr) {
        throw makeError<CompileError::NotImplemented>(
            callExpr.errorToken(), "function calls");
    }

    VisitResult BytecodeCompiler::visitVariableExpr(
        [[maybe_unused]] const VariableExpression &varExpr) {
        throw makeError<CompileError::NotImplemented>(
            varExpr.errorToken(), "variable expressions");
    }

    VisitResult BytecodeCompiler::visitNumberExpr(const NumberExpression &numExpr) {
        if (numExpr.isIntLiteral()) {
            // TODO: handle integer literals separately
            //throw std::logic_error("Integers not yet supported");
        }

        std::string lexeme = numExpr.value().lexeme;
        lexeme.erase(std::remove(lexeme.begin(), lexeme.end(), '_'), lexeme.end());

        // TODO: make function to parse integers
        double realValue;
        try {
            std::size_t numConverted;
            realValue = std::stod(lexeme, &numConverted);
            if (numConverted != lexeme.size()) {
                throw CompileException("could not parse real literal: unexpected suffix");
            }
        } catch (const std::out_of_range&) {
            throw makeError<CompileError::LiteralOutOfRange>(
                numExpr.errorToken(), "real literal");
        } catch (const std::invalid_argument&) {
            throw CompileException("could not parse real literal: invalid argument");
        }

        Value value{realValue};
        emitConstant(value, numExpr.value().location.line);

        return {};
    }

    void BytecodeCompiler::emit(OpCode opCode, int line) {
        m_chunk.writeInstruction(opCode, line);
    }

    void BytecodeCompiler::emit(OpCode opCode, std::uint8_t arg, int line) {
        m_chunk.writeInstruction(opCode, arg, line);
    }

    void BytecodeCompiler::emitConstant(Value value, int line) {
        emit(OpCode::Constant, makeConstant(value), line);
    }

    std::uint8_t BytecodeCompiler::makeConstant(Value value) {
        std::uint8_t constant = m_chunk.addConstant(value);
        if (m_chunk.constantPool().size() > std::numeric_limits<std::uint8_t>::max()) {
            throw CompileException("Too many constants in one chunk.");
        }
        return constant;
    }
}
