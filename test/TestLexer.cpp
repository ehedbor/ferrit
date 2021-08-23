#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Lexer.h"

namespace es::tests {
    void requireTokenEquals(const cpp::result<Token, LexError> &actual, const es::Token &expected) {
        REQUIRE(actual.has_value());
        REQUIRE(actual.value() == expected);
    }

    SCENARIO("numbers can be lexed", "[lexer]") {
        GIVEN("A lexer with some number literals") {
            es::Lexer lexer{"1 6.9 42"};
            WHEN("the tokens are lexed") {
                THEN("there will be no errors") {
                    requireTokenEquals(lexer.lex(), {TokenType::IntegerLiteral, "1", {1, 1}});
                    requireTokenEquals(lexer.lex(), {TokenType::FloatLiteral, "6.9", {1, 3}});
                    requireTokenEquals(lexer.lex(), {TokenType::IntegerLiteral, "42", {1, 7}});
                    requireTokenEquals(lexer.lex(), {TokenType::EndOfFile, "", {1, 9}});
                }
            }
        }
    }
}