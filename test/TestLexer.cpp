#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Lexer.h"

namespace es::tests {
    SCENARIO("literals can be lexed", "[lexer]") {
        GIVEN("basic number literals") {
            Lexer lexer{"1 6.9 42"};
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::IntegerLiteral, "1", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::FloatLiteral, "6.9", {1, 3}});
                    REQUIRE(lexer.lex() == Token{TokenType::IntegerLiteral, "42", {1, 7}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {1, 9}});
                }
            }
        }
        GIVEN("invalid float literals") {
            Lexer lexer{"100. .14 0xF8"};
            WHEN("the tokens are lexed") {
                THEN("they will not be float literals") {
                    REQUIRE(lexer.lex() == Token{TokenType::IntegerLiteral, "100", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Period, ".", {1, 4}});
                    REQUIRE(lexer.lex() == Token{TokenType::Period, ".", {1, 6}});
                    REQUIRE(lexer.lex() == Token{TokenType::IntegerLiteral, "14", {1, 7}});
                    REQUIRE(lexer.lex().has_error());
                }
            }
        }
        GIVEN("some valid string and char literals") {
            Lexer lexer(R"str("My String" "\0\t\n\r\'\"\\" 'a' '$')str");
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::StringLiteral, "\"My String\"", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::StringLiteral, R"("\0\t\n\r\'\"\\")", {1, 13}});
                    REQUIRE(lexer.lex() == Token{TokenType::CharLiteral, "'a'", {1, 30}});
                    REQUIRE(lexer.lex() == Token{TokenType::CharLiteral, "'$'", {1, 34}});
                }
            }
        }
        GIVEN("invalid string and char literals") {
            Lexer lexers[] = {
                Lexer("\"whuh oh"),
                Lexer("\"Is that a \n newline???\""),
                Lexer(R"("It's an \? illegal escape character!")"),
                Lexer("'hello there'"),
                Lexer("''"),
                Lexer("'\n'"),
                Lexer("'\\"),
            };
            WHEN("the tokens are lexed") {
                THEN("they will all be errors") {
                    for (auto &lexer : lexers) {
                        REQUIRE(lexer.lex().has_error());
                    }
                }
            }
        }
    }

    SCENARIO("basic tokens can be lexed", "[lexer]") {
        GIVEN("grouping operators") {
            Lexer lexer("(){}[]");
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::LeftParen, "(", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::RightParen, ")", {1, 2}});
                    REQUIRE(lexer.lex() == Token{TokenType::LeftBrace, "{", {1, 3}});
                    REQUIRE(lexer.lex() == Token{TokenType::RightBrace, "}", {1, 4}});
                    REQUIRE(lexer.lex() == Token{TokenType::LeftBracket, "[", {1, 5}});
                    REQUIRE(lexer.lex() == Token{TokenType::RightBracket, "]", {1, 6}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {1, 7}});
                }
            }
        }
        GIVEN("single-character operators") {
            Lexer lexer(", . : ; + - * / % & | ^ ~ ! = > <");
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::Comma, ",", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Period, ".", {1, 3}});
                    REQUIRE(lexer.lex() == Token{TokenType::Colon, ":", {1, 5}});
                    REQUIRE(lexer.lex() == Token{TokenType::Semicolon, ";", {1, 7}});
                    REQUIRE(lexer.lex() == Token{TokenType::Plus, "+", {1, 9}});
                    REQUIRE(lexer.lex() == Token{TokenType::Minus, "-", {1, 11}});
                    REQUIRE(lexer.lex() == Token{TokenType::Times, "*", {1, 13}});
                    REQUIRE(lexer.lex() == Token{TokenType::Divide, "/", {1, 15}});
                    REQUIRE(lexer.lex() == Token{TokenType::Modulo, "%", {1, 17}});
                    REQUIRE(lexer.lex() == Token{TokenType::BitwiseAnd, "&", {1, 19}});
                    REQUIRE(lexer.lex() == Token{TokenType::BitwiseOr, "|", {1, 21}});
                    REQUIRE(lexer.lex() == Token{TokenType::BitwiseXor, "^", {1, 23}});
                    REQUIRE(lexer.lex() == Token{TokenType::BitwiseNot, "~", {1, 25}});
                    REQUIRE(lexer.lex() == Token{TokenType::LogicalNot, "!", {1, 27}});
                    REQUIRE(lexer.lex() == Token{TokenType::Assign, "=", {1, 29}});
                    REQUIRE(lexer.lex() == Token{TokenType::Greater, ">", {1, 31}});
                    REQUIRE(lexer.lex() == Token{TokenType::Less, "<", {1, 33}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {1, 34}});
                }
            }
        }
        GIVEN("two-character operators") {
            Lexer lexer("-> && || != == >= <= << >>");
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::Arrow, "->", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::LogicalAnd, "&&", {1, 4}});
                    REQUIRE(lexer.lex() == Token{TokenType::LogicalOr, "||", {1, 7}});
                    REQUIRE(lexer.lex() == Token{TokenType::NotEqual, "!=", {1, 10}});
                    REQUIRE(lexer.lex() == Token{TokenType::Equal, "==", {1, 13}});
                    REQUIRE(lexer.lex() == Token{TokenType::GreaterEqual, ">=", {1, 16}});
                    REQUIRE(lexer.lex() == Token{TokenType::LessEqual, "<=", {1, 19}});
                    REQUIRE(lexer.lex() == Token{TokenType::BitwiseLeftShift, "<<", {1, 22}});
                    REQUIRE(lexer.lex() == Token{TokenType::BitwiseRightShift, ">>", {1, 25}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {1, 27}});
                }
            }
        }
    }

    SCENARIO("identifiers and keywords can be lexed") {
        GIVEN("simple keywords") {
            Lexer lexer("native var fun return true false int double");
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::Native, "native", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Var, "var", {1, 8}});
                    REQUIRE(lexer.lex() == Token{TokenType::Fun, "fun", {1, 12}});
                    REQUIRE(lexer.lex() == Token{TokenType::Return, "return", {1, 16}});
                    REQUIRE(lexer.lex() == Token{TokenType::True, "true", {1, 23}});
                    REQUIRE(lexer.lex() == Token{TokenType::False, "false", {1, 28}});
                    REQUIRE(lexer.lex() == Token{TokenType::Int, "int", {1, 34}});
                    REQUIRE(lexer.lex() == Token{TokenType::Double, "double", {1, 38}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {1, 44}});
                }
            }
        }
        GIVEN("some identifiers") {
            Lexer lexer("ANormalIdent Return _int _1_000_000");
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "ANormalIdent", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "Return", {1, 14}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "_int", {1, 21}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "_1_000_000", {1, 26}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {1, 36}});
                }
            }
        }
    }

    SCENARIO("comments and newlines can be lexed") {
        GIVEN("a line comment") {
            Lexer lexer("a//b\r\nc");
            WHEN("the tokens are lexed") {
                THEN("the comment will be ignored until the end of the line") {
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "a", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Newline, "\r\n", {1, 5}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "c", {2, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {2, 2}});
                }
            }
        }
        GIVEN("a block comment") {
            Lexer lexer("a/*Hello\r\nthere!*/b");
            WHEN("the tokens are lexed") {
                THEN("the comment will be ignored until the closing characters are found") {
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "a", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "b", {2, 9}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {2, 10}});
                }
            }
        }
        GIVEN("some newlines") {
            Lexer lexer("a\r\nb\nc");
            WHEN("the tokens are lexed") {
                THEN("appropriate newline tokens are generated") {
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "a", {1, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Newline, "\r\n", {1, 2}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "b", {2, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::Newline, "\n", {2, 2}});
                    REQUIRE(lexer.lex() == Token{TokenType::Identifier, "c", {3, 1}});
                    REQUIRE(lexer.lex() == Token{TokenType::EndOfFile, "", {3, 2}});
                }
            }
        }
    }
}