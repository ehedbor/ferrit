#include <catch2/catch.hpp>
#include <iostream>
#include "Parser.h"

namespace ferrit::tests {
    TEST_CASE("function declarations can be parsed", "[parser]") {
        CompileOptions opts;
        Parser parser(opts);

        SECTION("parsing simple no-op function") {
            std::vector<Token> tokens {
                {TokenType::Fun, "fun", {}},
                {TokenType::Identifier, "my_function", {}},
                {TokenType::LeftParen, "(", {}},
                {TokenType::RightParen, ")", {}},
                {TokenType::Arrow, "->", {}},
                {TokenType::Identifier, "Int", {}},
                {TokenType::Equal, "=", {}},
                {TokenType::IntegerLiteral, "0", {}},
                {TokenType::EndOfFile, "", {}}
            };

            FunctionDeclaration expected(
                std::vector<Token>(),
                Token{TokenType::Fun, "fun", {}},
                Token{TokenType::Identifier, "my_function", {}},
                std::vector<Parameter>(),
                Type(Token(TokenType::Identifier, "Int", {})),
                std::make_unique<ExpressionStatement>(
                    std::make_unique<NumberExpression>(
                        Token(TokenType::IntegerLiteral, "0", {}), true)));

            auto result = parser.parse(tokens);
            REQUIRE(result.has_value());
            REQUIRE(result.value().size() == 1);

            const Statement *decl = result.value()[0].get();
            const auto *funcDef = dynamic_cast<const FunctionDeclaration *>(decl);
            REQUIRE(funcDef != nullptr);
            REQUIRE(*funcDef == expected);
        }

        SECTION("parsing simple function with 2 parameters") {
            // private friend fun doUnholyMagic(
            //     taxes: Double,
            //     age: Int,
            // ) -> Double = taxes;
            std::vector<Token> tokens {
                //{TokenType::Private, "private", {1, 1}},
                //{TokenType::Friend, "friend", {1, 9}},
                {TokenType::Native, "native", {1, 1}},
                {TokenType::Fun, "fun", {1, 16}},
                {TokenType::Identifier, "doUnholyMagic", {1, 20}},
                {TokenType::LeftParen, "(", {1, 33}},
                {TokenType::Newline, "\r\n", {1, 34}},

                {TokenType::Identifier, "taxes", {2, 5}},
                {TokenType::Colon, ":", {2, 10}},
                {TokenType::Identifier, "Double", {2, 12}},
                {TokenType::Comma, ",", {2, 18}},
                {TokenType::Newline, "\r\n", {2, 19}},

                {TokenType::Identifier, "age", {3, 5}},
                {TokenType::Colon, ":", {3, 8}},
                {TokenType::Identifier, "Int", {3, 10}},
                {TokenType::Comma, ",", {3, 13}},
                {TokenType::Newline, "\r\n", {3, 14}},

                {TokenType::RightParen, ")", {4, 1}},
                {TokenType::Arrow, "->", {4, 3}},
                {TokenType::Identifier, "Double", {4, 6}},
                {TokenType::Equal, "=", {4, 13}},
                {TokenType::Identifier, "taxes", {4, 15}},
                {TokenType::Semicolon, ";", {4, 20}},
                {TokenType::EndOfFile, "", {4, 21}},
            };

            FunctionDeclaration expected{
                {
                    //Token(TokenType::Private, "private", {1, 1}),
                    //Token(TokenType::Friend, "friend", {1, 9}),
                    Token(TokenType::Native, "native", {1, 1}),
                },
                Token(TokenType::Fun, "fun", {1, 16}),
                Token(TokenType::Identifier, "doUnholyMagic", {1, 20}),
                {
                    Parameter(
                        Token(TokenType::Identifier, "taxes", {2, 5}),
                        Type(Token(TokenType::Identifier, "Double", {2, 12}))),
                    Parameter(
                        Token(TokenType::Identifier, "age", {3, 5}),
                        Type(Token(TokenType::Identifier, "Int", {3, 10})))
                },
                Type(Token(TokenType::Identifier, "Double", {4, 6})),
                std::make_unique<ExpressionStatement>(
                    std::make_unique<VariableExpression>(
                        Token(TokenType::Identifier, "taxes", {4, 15})))
            };

            auto result = parser.parse(tokens);
            REQUIRE(result.has_value());
            REQUIRE(result.value().size() == 1);

            Statement *decl = result.value()[0].get();
            auto *funcDecl = dynamic_cast<FunctionDeclaration *>(decl);
            REQUIRE(funcDecl != nullptr);
            REQUIRE(*funcDecl == expected);
        }
    }
}