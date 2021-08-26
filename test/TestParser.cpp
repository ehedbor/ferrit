#include <catch2/catch.hpp>
#include <iostream>
#include "Parser.h"

namespace es::tests {
    class TokenGenerator : public ExpressionVisitor, public StatementVisitor {
    public:
        TokenGenerator() = default;

        std::vector<Token> takeTokens() noexcept {
            auto ret = std::move(m_tokens);
            m_tokens.clear();
            return ret;
        }

    public:
        VisitResult visitNumber(const NumberExpression &expr) noexcept override {
            m_tokens.push_back(expr.value());
            return {};
        }

        VisitResult visitVariable(const VariableExpression &expr) noexcept override {
            m_tokens.push_back(expr.name());
            return {};
        }

        VisitResult visitBinary(const SimpleBinaryExpression &expr) noexcept override {
            expr.left().accept(*this);
            m_tokens.push_back(expr.op());
            expr.right().accept(*this);
            return {};
        }

        VisitResult visitBinary(const BitwiseBinaryExpression &expr) noexcept override {
            expr.left().accept(*this);
            m_tokens.push_back(expr.op());
            expr.right().accept(*this);
            return {};
        }

        VisitResult visitBinary(const CompareBinaryExpression &expr) noexcept override {
            expr.left().accept(*this);
            m_tokens.push_back(expr.op());
            expr.right().accept(*this);
            return {};
        }

        VisitResult visitUnary(const UnaryExpression &expr) noexcept override {
            m_tokens.push_back(expr.op());
            expr.operand().accept(*this);
            return {};
        }

        VisitResult visitExprStmt(const ExpressionStatement &stmt) noexcept override {
            stmt.expr().accept(*this);
            return {};
        }

        VisitResult visitBlock(const Block &stmt) noexcept override {
            m_tokens.emplace_back(TokenType::LeftBrace, "{", SourceLocation());
            for (const auto &statement : stmt.body()) {
                statement->accept(*this);
                m_tokens.emplace_back(TokenType::Semicolon, ";", SourceLocation());
            }
            m_tokens.emplace_back(TokenType::RightBrace, "}", SourceLocation());

            return {};
        }

        VisitResult visitFunDeclaration(const FunctionDeclaration &stmt) noexcept override {
            for (const auto &modifier : stmt.modifiers()) {
                m_tokens.push_back(modifier);
            }

            m_tokens.push_back(stmt.keyword());
            m_tokens.push_back(stmt.name());

            m_tokens.emplace_back(TokenType::LeftParen, "(", SourceLocation());
            for (const auto &param : stmt.params()) {
                m_tokens.push_back(param.name());
                m_tokens.emplace_back(TokenType::Colon, ":", SourceLocation());
                m_tokens.push_back(param.type().name());
                m_tokens.emplace_back(TokenType::Comma, ",", SourceLocation());
            }
            m_tokens.emplace_back(TokenType::RightParen, ")", SourceLocation());

            m_tokens.emplace_back(TokenType::Arrow, "->", SourceLocation());
            m_tokens.push_back(stmt.returnType().name());

            return {};
        }

        VisitResult visitFunDefinition(const FunctionDefinition &def) noexcept override {
            def.declaration().accept(*this);

            if (dynamic_cast<const ExpressionStatement *>(&def.body())) {
                m_tokens.emplace_back(TokenType::Assign, "=", SourceLocation());
            }
            def.body().accept(*this);

            return {};
        }

    private:
        std::vector<Token> m_tokens{};
    };

    static bool checkResultErr(const ParseResult &result) {
        if (result.has_error()) {
            for (auto &err : result.error()) std::cerr << err << std::endl;
        }
        return result.has_value();
    }

    static void addEof(std::vector<Token> &tokens) {
        tokens.emplace_back(TokenType::EndOfFile, "", SourceLocation());
    }

    TEST_CASE("function declarations can be parsed", "[parser]") {
        TokenGenerator tokenGen;

        SECTION("parsing simple no-op function") {
            FunctionDefinition expected(
                std::make_unique<FunctionDeclaration>(
                    std::vector<Token>(),
                    Token(TokenType::Fun, "fun", {}),
                    Token({TokenType::Identifier, "my_function", {}}),
                    std::vector<Parameter>(),
                    Type(Token(TokenType::Int, "int", {}))),
                std::make_unique<ExpressionStatement>(
                    std::make_unique<NumberExpression>(
                        Token(TokenType::IntegerLiteral, "0", {}), true)));

            tokenGen.visitFunDefinition(expected);
            auto tokens = tokenGen.takeTokens();
            addEof(tokens);

            Parser parser(std::move(tokens));
            ParseResult result = parser.parse();
            REQUIRE(checkResultErr(result));
            REQUIRE(result->size() == 1);

            Statement *decl = result.value()[0].get();
            auto *funcDef = dynamic_cast<FunctionDefinition *>(decl);
            REQUIRE(funcDef != nullptr);
            REQUIRE(*funcDef == expected);
        }

        SECTION("parsing simple function with 2 parameters") {
            FunctionDefinition expected{
                std::make_unique<FunctionDeclaration>(
                    std::vector<Token>{
                        Token{TokenType::Native, "native", {}}
                    },
                    Token{TokenType::Fun, "fun", {}},
                    Token{TokenType::Identifier, "my_func", {}},
                    std::vector<Parameter>{
                    Parameter{
                        Token{TokenType::Identifier, "a", {}},
                        Type{Token{TokenType::Double, "double", {}}}},
                        Parameter{
                            Token{TokenType::Identifier, "b", {}},
                            Type{Token{TokenType::Int, "int", {}}}}
                    },
                    Type(Token{TokenType::Double, "double", {}})),
                std::make_unique<ExpressionStatement>(
                    std::make_unique<VariableExpression>(
                        Token{TokenType::Identifier, "a", {}}))
            };
            tokenGen.visitFunDefinition(expected);
            auto tokens = tokenGen.takeTokens();
            addEof(tokens);

            Parser parser(std::move(tokens));
            ParseResult result = parser.parse();
            REQUIRE(checkResultErr(result));
            REQUIRE(result->size() == 1);

            Statement *decl = result.value()[0].get();
            auto *funcDecl = dynamic_cast<FunctionDefinition *>(decl);
            REQUIRE(funcDecl != nullptr);
            REQUIRE(*funcDecl == expected);
        }
    }
}