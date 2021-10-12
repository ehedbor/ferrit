#include <catch2/catch.hpp>
#include "Lexer.h"

namespace ferrit::tests {
    TEST_CASE("lex numeric literals", "[lexer]") {
        CompileOptions options;
        Lexer lexer(options);

        SECTION("basic integer literals") {
            auto tokens = lexer.lex("10 20 386 -1382");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 6);
            REQUIRE(tokens.value()[0] == Token{TokenType::IntegerLiteral, "10", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::IntegerLiteral, "20", {1, 4}});
            REQUIRE(tokens.value()[2] == Token{TokenType::IntegerLiteral, "386", {1, 7}});
            REQUIRE(tokens.value()[3] == Token{TokenType::Minus, "-", {1, 11}});
            REQUIRE(tokens.value()[4] == Token{TokenType::IntegerLiteral, "1382", {1, 12}});
            REQUIRE(tokens.value()[5] == Token{TokenType::EndOfFile, "", {1, 16}});
        }

        SECTION("basic float literals") {
            auto tokens = lexer.lex("1.0 6.9 386.14");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 4);
            REQUIRE(tokens.value()[0] == Token{TokenType::FloatLiteral, "1.0", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::FloatLiteral, "6.9", {1, 5}});
            REQUIRE(tokens.value()[2] == Token{TokenType::FloatLiteral, "386.14", {1, 9}});
            REQUIRE(tokens.value()[3] == Token{TokenType::EndOfFile, "", {1, 15}});
        }

        SECTION("not float literals") {
            auto tokens = lexer.lex("100. .14");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 5);
            REQUIRE(tokens.value()[0] == Token{TokenType::IntegerLiteral, "100", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::Dot, ".", {1, 4}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Dot, ".", {1, 6}});
            REQUIRE(tokens.value()[3] == Token{TokenType::IntegerLiteral, "14", {1, 7}});
            REQUIRE(tokens.value()[4] == Token{TokenType::EndOfFile, "", {1, 9}});
        }

        SECTION("int literals with suffix") {
            auto tokens = lexer.lex("10i");
            REQUIRE(!tokens.has_value());
            tokens = lexer.lex("10L");
            REQUIRE(!tokens.has_value());
            tokens = lexer.lex("10s");
            REQUIRE(!tokens.has_value());
            tokens = lexer.lex("10b");
            REQUIRE(!tokens.has_value());
        }

        SECTION("int literals with base") {
            auto tokens = lexer.lex("0b1101_0010");
            REQUIRE(!tokens.has_value());
            tokens = lexer.lex("0xB30C");
            REQUIRE(!tokens.has_value());
        }

        SECTION("float literals with exponent") {
            auto tokens = lexer.lex("6.022e23");
            REQUIRE(!tokens.has_value());

            tokens = lexer.lex("1.0e-10");
            REQUIRE(!tokens.has_value());
        }
    }

    TEST_CASE("lex string literals", "[lexer]") {
        Lexer lexer{CompileOptions()};

        SECTION("valid string and char literals") {
            auto tokens = lexer.lex(R"str("My String" "\0\t\n\r\'\"\\" 'a' '$')str");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 5);
            REQUIRE(tokens.value()[0] == Token{TokenType::StringLiteral, "\"My String\"", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::StringLiteral, R"("\0\t\n\r\'\"\\")", {1, 13}});
            REQUIRE(tokens.value()[2] == Token{TokenType::CharLiteral, "'a'", {1, 30}});
            REQUIRE(tokens.value()[3] == Token{TokenType::CharLiteral, "'$'", {1, 34}});
            REQUIRE(tokens.value()[4] == Token{TokenType::EndOfFile, "", {1, 37}});
        }

        SECTION("invalid string and char literals") {
            std::string literals[] = {
                "\"whuh oh",
                "\"Is that a \n newline???\"",
                R"("It's an \? illegal escape character!")",
                "'hello there'",
                "''",
                "'\n'",
                "'\\"
            };
            for (const auto &literal : literals) {
                REQUIRE(!lexer.lex(literal).has_value());
            }
        }
    }

    TEST_CASE("lex operators", "[lexer]") {
        Lexer lexer{CompileOptions{}};

        SECTION("grouping operators") {
            auto tokens = lexer.lex("(){}[]");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 7);
            REQUIRE(tokens.value()[0] == Token{TokenType::LeftParen, "(", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::RightParen, ")", {1, 2}});
            REQUIRE(tokens.value()[2] == Token{TokenType::LeftBrace, "{", {1, 3}});
            REQUIRE(tokens.value()[3] == Token{TokenType::RightBrace, "}", {1, 4}});
            REQUIRE(tokens.value()[4] == Token{TokenType::LeftBracket, "[", {1, 5}});
            REQUIRE(tokens.value()[5] == Token{TokenType::RightBracket, "]", {1, 6}});
            REQUIRE(tokens.value()[6] == Token{TokenType::EndOfFile, "", {1, 7}});
        }
        SECTION("navigation operators") {
            auto tokens = lexer.lex(". :: ? ?? !! ?. ?:");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 8);
            REQUIRE(tokens.value()[0] == Token{TokenType::Dot, ".", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::ColonColon, "::", {1, 3}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Question, "?", {1, 6}});
            REQUIRE(tokens.value()[3] == Token{TokenType::QuestionQuestion, "??", {1, 8}});
            REQUIRE(tokens.value()[4] == Token{TokenType::BangBang, "!!", {1, 11}});
            REQUIRE(tokens.value()[5] == Token{TokenType::QuestionDot, "?.", {1, 14}});
            REQUIRE(tokens.value()[6] == Token{TokenType::QuestionColon, "?:", {1, 17}});
            REQUIRE(tokens.value()[7] == Token{TokenType::EndOfFile, "", {1, 19}});
        }
        SECTION("arithmetic operators") {
            auto tokens = lexer.lex("+ ++ - -- * / % ~");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 9);
            REQUIRE(tokens.value()[0] == Token{TokenType::Plus, "+", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::PlusPlus, "++", {1, 3}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Minus, "-", {1, 6}});
            REQUIRE(tokens.value()[3] == Token{TokenType::MinusMinus, "--", {1, 8}});
            REQUIRE(tokens.value()[4] == Token{TokenType::Asterisk, "*", {1, 11}});
            REQUIRE(tokens.value()[5] == Token{TokenType::Slash, "/", {1, 13}});
            REQUIRE(tokens.value()[6] == Token{TokenType::Percent, "%", {1, 15}});
            REQUIRE(tokens.value()[7] == Token{TokenType::Tilde, "~", {1, 17}});
            REQUIRE(tokens.value()[8] == Token{TokenType::EndOfFile, "", {1, 18}});
        }
        SECTION("comparison and boolean operators") {
            auto tokens = lexer.lex("> < >= <= == != && || !");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 10);
            REQUIRE(tokens.value()[0] == Token{TokenType::Greater, ">", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::Less, "<", {1, 3}});
            REQUIRE(tokens.value()[2] == Token{TokenType::GreaterEqual, ">=", {1, 5}});
            REQUIRE(tokens.value()[3] == Token{TokenType::LessEqual, "<=", {1, 8}});
            REQUIRE(tokens.value()[4] == Token{TokenType::EqualEqual, "==", {1, 11}});
            REQUIRE(tokens.value()[5] == Token{TokenType::BangEqual, "!=", {1, 14}});
            REQUIRE(tokens.value()[6] == Token{TokenType::AndAnd, "&&", {1, 17}});
            REQUIRE(tokens.value()[7] == Token{TokenType::OrOr, "||", {1, 20}});
            REQUIRE(tokens.value()[8] == Token{TokenType::Bang, "!", {1, 23}});
            REQUIRE(tokens.value()[9] == Token{TokenType::EndOfFile, "", {1, 24}});
        }
        SECTION("assignment operators") {
            auto tokens = lexer.lex("=  += -= *= /= %= ~= &&= ||=");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 10);
            REQUIRE(tokens.value()[0] == Token{TokenType::Equal, "=", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::PlusEqual, "+=", {1, 4}});
            REQUIRE(tokens.value()[2] == Token{TokenType::MinusEqual, "-=", {1, 7}});
            REQUIRE(tokens.value()[3] == Token{TokenType::AsteriskEqual, "*=", {1, 10}});
            REQUIRE(tokens.value()[4] == Token{TokenType::SlashEqual, "/=", {1, 13}});
            REQUIRE(tokens.value()[5] == Token{TokenType::PercentEqual, "%=", {1, 16}});
            REQUIRE(tokens.value()[6] == Token{TokenType::TildeEqual, "~=", {1, 19}});
            REQUIRE(tokens.value()[7] == Token{TokenType::AndAndEqual, "&&=", {1, 22}});
            REQUIRE(tokens.value()[8] == Token{TokenType::OrOrEqual, "||=", {1, 26}});
            REQUIRE(tokens.value()[9] == Token{TokenType::EndOfFile, "", {1, 29}});
        }
        SECTION("keyword operators") {
            auto tokens = lexer.lex("as as? is !is in !in");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 7);
            REQUIRE(tokens.value()[0] == Token{TokenType::As, "as", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::AsQuestion, "as?", {1, 4}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Is, "is", {1, 8}});
            REQUIRE(tokens.value()[3] == Token{TokenType::BangIs, "!is", {1, 11}});
            REQUIRE(tokens.value()[4] == Token{TokenType::In, "in", {1, 15}});
            REQUIRE(tokens.value()[5] == Token{TokenType::BangIn, "!in", {1, 18}});
            REQUIRE(tokens.value()[6] == Token{TokenType::EndOfFile, "", {1, 21}});
        }
        SECTION("other operators") {
            auto tokens = lexer.lex(", .. ... -> : ; \r\n");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 8);
            REQUIRE(tokens.value()[0] == Token{TokenType::Comma, ",", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::DotDot, "..", {1, 3}});
            REQUIRE(tokens.value()[2] == Token{TokenType::DotDotDot, "...", {1, 6}});
            REQUIRE(tokens.value()[3] == Token{TokenType::Arrow, "->", {1, 10}});
            REQUIRE(tokens.value()[4] == Token{TokenType::Colon, ":", {1, 13}});
            REQUIRE(tokens.value()[5] == Token{TokenType::Semicolon, ";", {1, 15}});
            REQUIRE(tokens.value()[6] == Token{TokenType::Newline, "\r\n", {1, 17}});
            REQUIRE(tokens.value()[7] == Token{TokenType::EndOfFile, "", {2, 1}});
        }
    }

    TEST_CASE("lex identifiers and keywords", "[lexer]") {
        Lexer lexer{CompileOptions()};

        SECTION("modifiers") {
            std::pair<std::string, TokenType> modifiers[] = {
                {"using", TokenType::Using},
                {"module", TokenType::Module},
                {"public", TokenType::Public},
                {"protected", TokenType::Protected},
                {"private", TokenType::Private},
                {"companion", TokenType::Companion},
                {"friend", TokenType::Friend},
                {"open", TokenType::Open},
                {"closed", TokenType::Closed},
                {"abstract", TokenType::Abstract},
                {"override", TokenType::Override},
                {"operator", TokenType::Operator},
                {"native", TokenType::Native},
            };

            for (const auto &[mod, type] : modifiers) {
                auto tokens = lexer.lex(mod);
                REQUIRE(tokens.has_value());
                REQUIRE(tokens.value().size() == 2);
                REQUIRE(tokens.value()[0] == Token{type, mod, {1, 1}});
                REQUIRE(tokens.value()[1] == Token{TokenType::EndOfFile, "", {1, static_cast<int>(mod.size()) + 1}});
            }
        }

        SECTION("class, function and variable keywords") {
            std::pair<std::string, TokenType> keywords[] = {
                {"class", TokenType::Class},
                {"object", TokenType::Object},
                {"trait", TokenType::Trait},
                {"init", TokenType::Init},
                {"this", TokenType::This},
                {"super", TokenType::Super},
                {"fun", TokenType::Fun},
                {"var", TokenType::Var},
                {"val", TokenType::Val},
            };

            for (const auto &[mod, type] : keywords) {
                auto tokens = lexer.lex(mod);
                REQUIRE(tokens.has_value());
                REQUIRE(tokens.value().size() == 2);
                REQUIRE(tokens.value()[0] == Token{type, mod, {1, 1}});
                REQUIRE(tokens.value()[1] == Token{TokenType::EndOfFile, "", {1, static_cast<int>(mod.size()) + 1}});
            }
        }

        SECTION("control flow keywords") {
            std::pair<std::string, TokenType> keywords[] = {
                {"if", TokenType::If},
                {"else", TokenType::Else},
                {"for", TokenType::For},
                {"while", TokenType::While},
                {"do", TokenType::Do},
                {"return", TokenType::Return},
                {"continue", TokenType::Continue},
                {"break", TokenType::Break},
            };

            for (const auto &[mod, type] : keywords) {
                auto tokens = lexer.lex(mod);
                REQUIRE(tokens.has_value());
                REQUIRE(tokens.value().size() == 2);
                REQUIRE(tokens.value()[0] == Token{type, mod, {1, 1}});
                REQUIRE(tokens.value()[1] == Token{TokenType::EndOfFile, "", {1, static_cast<int>(mod.size()) + 1}});
            }
        }

        SECTION("literal keywords") {
            auto tokens = lexer.lex("true false null");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 4);
            REQUIRE(tokens.value()[0] == Token{TokenType::True, "true", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::False, "false", {1, 6}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Null, "null", {1, 12}});
            REQUIRE(tokens.value()[3] == Token{TokenType::EndOfFile, "", {1, 16}});
        }

        SECTION("some identifiers") {
            auto tokens = lexer.lex("ANormalIdent Return _int _1_000_000");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 5);
            REQUIRE(tokens.value()[0] == Token{TokenType::Identifier, "ANormalIdent", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::Identifier, "Return", {1, 14}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Identifier, "_int", {1, 21}});
            REQUIRE(tokens.value()[3] == Token{TokenType::Identifier, "_1_000_000", {1, 26}});
            REQUIRE(tokens.value()[4] == Token{TokenType::EndOfFile, "", {1, 36}});
        }
    }

    TEST_CASE("lex comments and whitespace", "[lexer]") {
        Lexer lexer(CompileOptions{});

        SECTION("a line comment") {
            auto tokens = lexer.lex("a//b\r\nc");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 4);
            REQUIRE(tokens.value()[0] == Token{TokenType::Identifier, "a", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::Newline, "\r\n", {1, 5}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Identifier, "c", {2, 1}});
            REQUIRE(tokens.value()[3] == Token{TokenType::EndOfFile, "", {2, 2}});
        }
        SECTION("a block comment") {
            auto tokens = lexer.lex("a/*Hello\r\nthere!*/b");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 3);
            REQUIRE(tokens.value()[0] == Token{TokenType::Identifier, "a", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::Identifier, "b", {2, 9}});
            REQUIRE(tokens.value()[2] == Token{TokenType::EndOfFile, "", {2, 10}});
        }
        SECTION("some newlines") {
            auto tokens = lexer.lex("a\r\nb\nc");
            REQUIRE(tokens.has_value());
            REQUIRE(tokens.value().size() == 6);
            REQUIRE(tokens.value()[0] == Token{TokenType::Identifier, "a", {1, 1}});
            REQUIRE(tokens.value()[1] == Token{TokenType::Newline, "\r\n", {1, 2}});
            REQUIRE(tokens.value()[2] == Token{TokenType::Identifier, "b", {2, 1}});
            REQUIRE(tokens.value()[3] == Token{TokenType::Newline, "\n", {2, 2}});
            REQUIRE(tokens.value()[4] == Token{TokenType::Identifier, "c", {3, 1}});
            REQUIRE(tokens.value()[5] == Token{TokenType::EndOfFile, "", {3, 2}});
        }
    }
}