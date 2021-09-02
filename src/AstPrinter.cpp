#include "AstPrinter.h"


namespace ferrit {
    AstPrinter::AstPrinter(std::ostream &out) noexcept : m_out(out) {
    }

    void AstPrinter::print(const std::vector<StatementPtr> &ast) {
        for (const auto &declaration : ast) {
            declaration->accept(*this);
        }
    }


    VisitResult AstPrinter::visitFunDeclaration(const FunctionDeclaration &funDecl) {
        printIndent();
        m_out << "Function Declaration:\n";

        m_depth++;

        printIndent();
        m_out << "-Modifiers:\n";
        m_depth++;
        for (auto &modifier: funDecl.modifiers()) {
            printIndent();
            m_out << modifier << "\n";
        }
        m_depth--;

        printIndent();
        m_out << "-Keyword=" << funDecl.keyword() << "\n";

        printIndent();
        m_out << "-Name=" << funDecl.name() << "\n";

        printIndent();
        m_out << "-Params:\n";
        m_depth++;
        for (auto &param : funDecl.params()) {
            printIndent();
            m_out << "Parameter{Name=" << param.name() << ", Type=" << param.type().name() << "}\n";
        }
        m_depth--;

        printIndent();
        m_out << "-Returns=" << funDecl.returnType().name() << "\n";

        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitFunDefinition(const FunctionDefinition &funDef) {
        printIndent();
        m_out << "Function Definition:\n";

        m_depth++;

        printIndent();
        m_out << "-Declaration:\n";
        m_depth++;
        funDef.declaration().accept(*this);
        m_depth--;

        printIndent();
        m_out << "-Body:\n";
        m_depth++;
        funDef.body().accept(*this);
        m_depth--;

        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitBlock(const Block &block) {
        printIndent();
        m_out << "Block:\n";

        m_depth++;
        for (auto &line : block.body()) {
            line->accept(*this);
        }
        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitExprStmt(const ExpressionStatement &exprStmt) {
        printIndent();
        m_out << "Expression Statement:\n";
        m_depth++;
        exprStmt.expr().accept(*this);
        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitSimpleBinary(const SimpleBinaryExpression &binExpr) {
        handleBinary("Simple Binary", binExpr.op(), binExpr.left(), binExpr.right());
        return {};
    }

    VisitResult AstPrinter::visitBitwiseBinary(const BitwiseBinaryExpression &bitBinExpr) {
        handleBinary("Bitwise Binary", bitBinExpr.op(), bitBinExpr.left(), bitBinExpr.right());
        return {};
    }

    VisitResult AstPrinter::visitComparison(const ComparisonExpression &cmpExpr) {
        handleBinary("Comparison", cmpExpr.op(), cmpExpr.left(), cmpExpr.right());
        return {};
    }

    VisitResult AstPrinter::visitUnary(const UnaryExpression &unaryExpr) {
        printIndent();
        m_out << "Unary: " << unaryExpr.op() << "\n";
        m_depth++;
        unaryExpr.operand().accept(*this);
        m_depth--;

        return {};
    }

    VisitResult AstPrinter::visitVariable(const VariableExpression &varExpr) {
        printIndent();
        m_out << "Variable: " << varExpr.name() << "\n";

        return {};
    }

    VisitResult AstPrinter::visitNumber(const NumberExpression &numExpr) {
        printIndent();
        if (numExpr.isIntLiteral()) {
            m_out << "Integer Literal: ";
        } else {
            m_out << "Float Literal: ";
        }
        m_out << numExpr.value() << "\n";

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
