#include "AstPrinter.h"

#include <format>


namespace ferrit {
    AstPrinter::AstPrinter(std::ostream &out) noexcept : m_out(out) {
    }

    void AstPrinter::print(const std::vector<StatementPtr> &program) {
        for (const auto &declaration : program) {
            declaration->accept(*this);
        }
    }

    VisitResult AstPrinter::visitFunctionDecl(const FunctionDeclaration &funDecl) {
        printLine("FunctionDeclaration:");
        indent([&] {
            printLine("-Modifiers:");
            indent([&] {
                for (auto &modifier: funDecl.modifiers()) {
                    printLine(modifier.lexeme);
                }
            });
            printLine(std::format("-Keyword={}", funDecl.keyword().lexeme));
            printLine(std::format("-Name={}", funDecl.name().lexeme));

            printLine("-Params:");
            indent([&] {
                for (const auto &param : funDecl.params()) {
                    printLine(std::format("Parameter(Name={}, Type={})",
                        param.name().lexeme, param.type().errorToken().lexeme));
                }
            });
            printLine(std::format("-Returns={}", funDecl.returnType().errorToken().lexeme));

            if (funDecl.body()) {
                printLine("-Body:");
                indent([&] {
                    funDecl.body()->accept(*this);
                });
            }
        });
        return {};
    }

    VisitResult AstPrinter::visitBlockStmt(const BlockStatement &blockStmt) {
        printLine("BlockStatement:");
        indent([&] {
            for (const auto &line : blockStmt.body()) {
                line->accept(*this);
            }
        });
        return {};
    }

    VisitResult AstPrinter::visitExpressionStmt(const ExpressionStatement &exprStmt) {
        printLine("ExpressionStatement:");
        indent([&] {
            exprStmt.expr().accept(*this);
        });
        return {};
    }

    VisitResult AstPrinter::visitBinaryExpr(const BinaryExpression &binExpr) {
        printLine("BinaryExpression:");
        indent([&] {
            printLine(std::format("-Op={}", binExpr.op().lexeme));
            printLine("-Left:");
            indent([&] {
                binExpr.left().accept(*this);
            });
            printLine("-Right:");
            indent([&] {
                binExpr.right().accept(*this);
            });
        });
        return {};
    }

    VisitResult AstPrinter::visitComparisonExpr(const ComparisonExpression &cmpExpr) {
        printLine("ComparisonExpression:");
        indent([&] {
            printLine(std::format("-Op={}", cmpExpr.op().lexeme));
            printLine("-Left:");
            indent([&] {
                cmpExpr.left().accept(*this);
            });
            printLine("-Right:");
            indent([&] {
                cmpExpr.right().accept(*this);
            });
        });
        return {};
    }

    VisitResult AstPrinter::visitUnaryExpr(const UnaryExpression &unaryExpr) {
        printLine(std::format("UnaryExpression: {}", unaryExpr.op().lexeme));
        indent([&] {
            unaryExpr.operand().accept(*this);
        });

        return {};
    }

    VisitResult AstPrinter::visitCallExpr(const CallExpression &callExpr) {
        printLine("CallExpression:");
        indent([&] {
            printLine("-Callee:");
            callExpr.callee().accept(*this);

            printLine("-Arguments:");
            indent([&] {
                for (const auto &arg: callExpr.arguments()) {
                    arg->accept(*this);
                }
            });
        });

        return {};
    }

    VisitResult AstPrinter::visitVariableExpr(const VariableExpression &varExpr) {
        printLine(std::format("VariableExpression: {}", varExpr.name().lexeme));
        return {};
    }

    VisitResult AstPrinter::visitNumberExpr(const NumberExpression &numExpr) {
        printLine(std::format("NumberExpression: {} {}",
            numExpr.isIntLiteral() ? "Int" : "Double",
            numExpr.value().lexeme));
        return {};
    }

    void AstPrinter::printLine(const std::string &line) {
        m_out << std::format("{:{}} {}\n", ' ', m_depth * INDENTATION_LEVEL, line);
    }
}
