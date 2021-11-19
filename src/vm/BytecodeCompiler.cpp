#include "BytecodeCompiler.h"


namespace ferrit {
    // TODO: use Errors for compiler errors and exceptions for compiler bugs

    BytecodeCompiler::BytecodeCompiler(std::shared_ptr<const ErrorReporter> errorReporter) :
        m_errorReporter{std::move(errorReporter)} {
    }

    std::optional<Chunk> BytecodeCompiler::compile(const std::vector<StatementPtr> &ast) {
        m_chunk = Chunk{};

        // TODO: support statements

        const Statement &stmt = *ast.front();
        if (!dynamic_cast<const ExpressionStatement *>(&stmt)) {
            throw std::invalid_argument("can only compile expressions");
        }
        stmt.accept(*this);

        // TODO: get last line somehow
        emit(OpCode::Return, -1);

        return m_chunk;
    }

    VisitResult BytecodeCompiler::visitFunctionDecl([[maybe_unused]] const FunctionDeclaration &funDecl) {
        throw std::logic_error("functions not implemented");
    }

    VisitResult BytecodeCompiler::visitBlockStmt([[maybe_unused]] const BlockStatement &blockStmt) {
        throw std::logic_error("block statements not implemented");
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
            throw std::logic_error("concatenation operator not implemented");
        case TokenType::Percent:
            throw std::logic_error("modulo operator not implemented");
        case TokenType::AndAnd:
            throw std::logic_error("logical and operator not implemented");
        case TokenType::OrOr:
            throw std::logic_error("logical or operator not implemented");
        default:
            throw std::logic_error(std::format("unknown operator '{}'", binExpr.op().type));
        }

        return {};
    }

    VisitResult BytecodeCompiler::visitComparisonExpr([[maybe_unused]] const ComparisonExpression &cmpExpr) {
        throw std::logic_error("comparisons not implemented");
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
            throw std::logic_error("concatenation operator not implemented");
        case TokenType::Bang:
            throw std::logic_error("boolean negation operator not implemented");
        case TokenType::PlusPlus:
            throw std::logic_error("increment operator not implemented");
        case TokenType::MinusMinus:
            throw std::logic_error("decrement operator not implemented");
        default:
            throw std::logic_error(std::format("unknown unary operator '{}'", opType));
        }
        return {};
    }

    VisitResult BytecodeCompiler::visitCallExpr([[maybe_unused]] const CallExpression &callExpr) {
        throw std::logic_error("function calls not implemented");
    }

    VisitResult BytecodeCompiler::visitVariableExpr([[maybe_unused]] const VariableExpression &varExpr) {
        throw std::logic_error("variables not implemented");
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
                throw std::logic_error("could not parse real literal: unexpected suffix");
            }
        } catch (const std::out_of_range&) {
            // TODO: this should be a compile error, not a program error
            throw std::logic_error("could not parse real literal: out of range");
        } catch (const std::invalid_argument&) {
            throw std::logic_error("could not parse real literal");
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
            throw std::logic_error("Too many constants in one chunk.");
        }
        return constant;
    }
}
