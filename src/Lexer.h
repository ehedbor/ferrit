#pragma once

#include <ostream>
#include <optional>
#include <string>
#include <vector>

#include <result.hpp>

#include "Token.h"


struct LexError {
public:
    LexError() = default;
    LexError(std::string msg, SourceLocation location);

    friend std::ostream &operator<<(std::ostream &out, const LexError &err);

public:
    std::string msg{};
    SourceLocation location{};
};

using LexResult = cpp::result<Token, LexError>;

class Lexer {
public:
    explicit Lexer(const std::string &code);

    [[nodiscard]] LexResult lex() noexcept;

private:
    [[nodiscard]] cpp::result<std::optional<Token>, LexError> skipWhitespace() noexcept;
    void ignoreLineComment() noexcept;
    [[nodiscard]] cpp::result<void, LexError> ignoreBlockComment() noexcept;

    [[nodiscard]] LexResult lexString() noexcept;
    [[nodiscard]] LexResult lexChar() noexcept;
    [[nodiscard]] cpp::result<void, LexError> advanceStringChar(const std::string &literalType) noexcept;

    [[nodiscard]] LexResult lexNumber() noexcept;
    [[nodiscard]] LexResult lexIdentifier() noexcept;
    [[nodiscard]] static TokenType getTokenTypeForIdent(const std::string &lexeme) noexcept;

    [[nodiscard]] Token makeToken(TokenType type) const noexcept;
    [[nodiscard]] LexError makeError(const std::string &msg) const noexcept;

    [[nodiscard]] std::optional<char> peek() const noexcept;
    [[nodiscard]] std::optional<char> peekNext() const noexcept;

    std::optional<char> advance() noexcept;
    [[nodiscard]] bool expect(char expected) noexcept;

    [[nodiscard]] bool isAtEnd() const noexcept;

    [[nodiscard]] static bool isDigit(char ch) noexcept;
    [[nodiscard]] static bool isIdentifier(char ch) noexcept;
    [[nodiscard]] static bool isIdentifierStart(char ch) noexcept;

private:
    const std::string &m_code;
    std::size_t m_start{0};
    std::size_t m_current{0};
    SourceLocation m_location{1, 1};
};


