#include <catch2/catch.hpp>
#include <iostream>
#include "Parser.h"

namespace es::tests {
    static bool checkResultErr(const ParseResult &result) {
        if (result.has_error()) {
            for (auto &err : result.error()) std::cerr << err << std::endl;
        }
        return result.has_value();
    }

    // TODO: generate tokens from statements by use of a Expr/Stmt visitor.

    static std::vector<Token> generateFunctionHeader(const FunctionStatement &func) {
        std::vector<Token> result;
        for (const auto &modifier : func.modifiers()) {
            result.push_back(modifier);
        }

        result.push_back(func.keyword());
        result.push_back(func.name());

        result.emplace_back(TokenType::LeftParen, "(", SourceLocation{});
        for (const auto &param : func.params()) {
            result.push_back(param.name());
            result.emplace_back(TokenType::Colon, ":", SourceLocation{});
            result.push_back(param.type().name());
            result.emplace_back(TokenType::Comma, ",", SourceLocation{});
        }
        result.emplace_back(TokenType::RightParen, ")", SourceLocation{});

        result.emplace_back(TokenType::Arrow, "->", SourceLocation{});
        result.push_back(func.returnType().name());

        return result;
    }

    TEST_CASE("function declarations can be parsed", "[parser]") {
        SECTION("parsing simple no-op function") {
            FunctionStatement expected(
                std::vector<Token>(),
                {TokenType::Fun, "fun", {}},
                {TokenType::Identifier, "my_function", {}},
                std::vector<Parameter>(),
                Type({TokenType::Int, "int", {}}),
                std::make_unique<ExpressionStatement>(
                    std::make_unique<NumberExpression>(
                        Token{TokenType::IntegerLiteral, "0", {}}, true)));
            auto tokens = generateFunctionHeader(expected);
            tokens.emplace_back(TokenType::Assign, "=", SourceLocation{});
            // TODO: generate bodies cause this is horrible.
            tokens.push_back(
                reinterpret_cast<const NumberExpression &>(
                    reinterpret_cast<const ExpressionStatement &>(
                        expected.body()
                    ).expr()
                ).value());
            tokens.emplace_back(TokenType::EndOfFile, "", SourceLocation{});

            Parser parser(std::move(tokens));
            ParseResult result = parser.parse();
            REQUIRE(checkResultErr(result));
            REQUIRE(result->size() == 1);

            Statement *decl = result.value()[0].get();
            auto *funcDecl = dynamic_cast<FunctionStatement *>(decl);
            REQUIRE(funcDecl != nullptr);
            REQUIRE(*funcDecl == expected);
        }

        SECTION("parsing simple function with 2 parameters") {
            FunctionStatement expected{
                {
                    Token{TokenType::Native, "native", {}}
                    },
                Token{TokenType::Fun, "fun", {}},
                Token{TokenType::Identifier, "my_func", {}},
                {
                    Parameter{
                        Token{TokenType::Identifier, "a", {}},
                        Type{Token{TokenType::Double, "double", {}}}},
                    Parameter{
                        Token{TokenType::Identifier, "b", {}},
                        Type{Token{TokenType::Int, "int", {}}}}
                        },
                Type(Token{TokenType::Double, "double", {}}),
                std::make_unique<ExpressionStatement>(
                    std::make_unique<VariableExpression>(
                        Token{TokenType::Identifier, "a", {}}))
            };
            auto tokens = generateFunctionHeader(expected);
            tokens.emplace_back(TokenType::Assign, "=", SourceLocation{});
            tokens.push_back(
                reinterpret_cast<const VariableExpression &>(
                    reinterpret_cast<const ExpressionStatement &>(
                        expected.body()
                    ).expr()
                ).name());
            tokens.emplace_back(TokenType::EndOfFile, "", SourceLocation{});

            Parser parser(std::move(tokens));
            ParseResult result = parser.parse();
            REQUIRE(checkResultErr(result));
            REQUIRE(result->size() == 1);

            Statement *decl = result.value()[0].get();
            auto *funcDecl = dynamic_cast<FunctionStatement *>(decl);
            REQUIRE(funcDecl != nullptr);
            REQUIRE(*funcDecl == expected);
        }
    }
}