#include "AstPrinter.h"


namespace es {
    AstPrinter::AstPrinter(std::ostream &out, std::vector<StatementPtr> &ast) noexcept :
        m_out(out), m_ast(ast) {
    }

    void AstPrinter::print() noexcept {
        for (auto &declaration : m_ast) {
            declaration->accept(*this);
        }
    }

    void AstPrinter::visitExpr(const ExpressionStatement &stmt) noexcept {
        printIndent();
        m_out << "ExpressionStatement:\n";
        m_depth++;
        stmt.expr().accept(*this);
        m_depth--;
    }

    void AstPrinter::visitBlock(const BlockStatement &stmt) noexcept {
        printIndent();
        m_out << "Block:\n";

        m_depth++;
        for (auto &line : stmt.body()) {
            line->accept(*this);
        }
        m_depth--;
    }

    void AstPrinter::visitFunction(const FunctionStatement &stmt) noexcept {
        printIndent();
        m_out << "Function:\n";

        m_depth++;

        printIndent();
        m_out << "Name: " << stmt.name() << "\n";

        printIndent();
        m_out << "Modifiers:\n";
        m_depth++;
        for (auto &modifier: stmt.modifiers()) {
            printIndent();
            m_out << modifier << "\n";
        }
        m_depth--;

        printIndent();
        m_out << "Params:\n";
        m_depth++;
        for (auto &param : stmt.params()) {
            printIndent();
            m_out << "" << param.name() << " : " << param.type().name() << "\n";
        }
        m_depth--;

        printIndent();
        m_out << "Returns: " << stmt.returnType().name() << "\n";

        printIndent();
        m_out << "Body:\n";
        m_depth++;
        stmt.body().accept(*this);
        m_depth--;

        m_depth--;
    }

    void AstPrinter::visitNumber(const NumberExpression &expr) noexcept {
        printIndent();
        if (expr.isIntLiteral()) {
            m_out << "IntegerLiteral: ";
        } else {
            m_out << "FloatLiteral: ";
        }
        m_out << expr.value() << "\n";
    }

    void AstPrinter::visitVariable(const VariableExpression &expr) noexcept {
        printIndent();
        m_out << "Variable: " << expr.name() << "\n";
    }

    void AstPrinter::printIndent() noexcept {
        for (unsigned int i = 0; i < m_depth; i++) {
            m_out << "  ";
        }
    }
}
