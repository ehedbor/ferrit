#include "AstPrinter.h"


namespace es {
    AstPrinter::AstPrinter(std::ostream &out, std::vector<StatementPtr> &ast) noexcept :
        m_out(out), m_ast(ast) {
    }

    void AstPrinter::print() {
        for (auto &declaration : m_ast) {
            declaration->accept(*this);
        }
    }

    VisitResult AstPrinter::visitNumber(const NumberExpression &expr) {
        printIndent();
        if (expr.isIntLiteral()) {
            m_out << "IntegerLiteral: ";
        } else {
            m_out << "FloatLiteral: ";
        }
        m_out << expr.value() << "\n";

        return {};
    }

    VisitResult AstPrinter::visitVariable(const VariableExpression &expr) {
        printIndent();
        m_out << "Variable: " << expr.name() << "\n";

        return {};
    }

    VisitResult AstPrinter::visitBinary(const SimpleBinaryExpression &expr) {
        handleBinary("SimpleBinary", expr.op(), expr.left(), expr.right());
        return {};
    }

    VisitResult AstPrinter::visitBinary(const BitwiseBinaryExpression &expr) {
        handleBinary("BitwiseBinary", expr.op(), expr.left(), expr.right());
        return {};
    }

    VisitResult AstPrinter::visitBinary(const CompareBinaryExpression &expr) {
        handleBinary("CompareBinary", expr.op(), expr.left(), expr.right());
        return {};
    }

    VisitResult AstPrinter::visitUnary(const UnaryExpression &expr) {
        printIndent();
        m_out << "Unary: " << expr.op() << "\n";
        m_depth++;
        expr.operand().accept(*this);
        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitExprStmt(const ExpressionStatement &stmt) {
        printIndent();
        m_out << "ExpressionStatement:\n";
        m_depth++;
        stmt.expr().accept(*this);
        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitBlock(const BlockStatement &stmt) {
        printIndent();
        m_out << "Block:\n";

        m_depth++;
        for (auto &line : stmt.body()) {
            line->accept(*this);
        }
        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitFunction(const FunctionStatement &stmt) {
        printIndent();
        m_out << "Function:\n";

        m_depth++;

        printIndent();
        m_out << "-Name=" << stmt.name() << "\n";

        printIndent();
        m_out << "-Modifiers:\n";
        m_depth++;
        for (auto &modifier: stmt.modifiers()) {
            printIndent();
            m_out << modifier << "\n";
        }
        m_depth--;

        printIndent();
        m_out << "-Params:\n";
        m_depth++;
        for (auto &param : stmt.params()) {
            printIndent();
            m_out << "Parameter{Name=" << param.name() << ", Type=" << param.type().name() << "}\n";
        }
        m_depth--;

        printIndent();
        m_out << "-Returns=" << stmt.returnType().name() << "\n";

        printIndent();

        m_out << "-Body:\n";
        if (stmt.body()) {
            m_depth++;
            stmt.body()->accept(*this);
            m_depth--;
        }

        m_depth--;

        return {};
    }

    void AstPrinter::handleBinary(const std::string &name, const Token &op, const Expression &left, const Expression &right) {
        printIndent();
        m_out << name << ":\n";
        m_depth++;

        printIndent();
        m_out << "-Op=" << op << "\n";

        printIndent();
        m_out << "-Left:\n";
        m_depth++;
        left.accept(*this);
        m_depth--;

        printIndent();
        m_out << "-Right:\n";
        m_depth++;
        right.accept(*this);
        m_depth--;

        m_depth--;
    }

    void AstPrinter::printIndent() noexcept {
        for (unsigned int i = 0; i < m_depth; i++) {
            m_out << "  ";
        }
    }
}
