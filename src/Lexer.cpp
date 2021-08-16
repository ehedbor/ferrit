#include "Lexer.h"

#include <sstream>
#include <unordered_map>


static const std::unordered_map<std::string, TokenType> IDENTIFIER_TYPES = { // NOLINT(cert-err58-cpp)
    {"native", TokenType::Native},
    {"var", TokenType::Var},
    {"fun", TokenType::Fun},
    {"return", TokenType::Return},
    {"true", TokenType::True},
    {"false", TokenType::False}
};

LexError::LexError(std::string msg, SourceLocation location) :
    msg(std::move(msg)), location(location) {
}

Lexer::Lexer(const std::string &code) : m_code(code) {
}

LexResult Lexer::lex() noexcept {
    auto maybeWhitespaceToken = skipWhitespace();
    if (maybeWhitespaceToken) {
        if (auto optToken = *maybeWhitespaceToken) {
            return *optToken;
        }
    } else {
        return cpp::fail(maybeWhitespaceToken.error());
    }

    m_start = m_current;

    std::optional<char> ch = advance();
    if (!ch) {
        return makeToken(TokenType::EndOfFile);
    }

    if (isDigit(*ch)) {
        return lexNumber();
    } else if (isIdentifierStart(*ch)) {
        return lexIdentifier();
    }

    switch (*ch) {
    case '"': return lexString();
    case '\'': return lexChar();
    case '(': return makeToken(TokenType::LeftParen);
    case ')': return makeToken(TokenType::RightParen);
    case '{': return makeToken(TokenType::LeftBrace);
    case '}': return makeToken(TokenType::RightBrace);
    case '[': return makeToken(TokenType::LeftBracket);
    case ']': return makeToken(TokenType::RightBracket);
    case ',': return makeToken(TokenType::Comma);
    case '.': return makeToken(TokenType::Period);
    case ';': return makeToken(TokenType::Semicolon);
    case '+': return makeToken(TokenType::Plus);
    case '-': return makeToken(TokenType::Minus);
    case '*': return makeToken(TokenType::Times);
    case '/': return makeToken(TokenType::Divide);
    case '%': return makeToken(TokenType::Modulo);
    case '&': return makeToken(expect('&') ? TokenType::LogicalAnd : TokenType::BitwiseAnd);
    case '|': return makeToken(expect('|') ? TokenType::LogicalOr : TokenType::BitwiseOr);
    case '^': return makeToken(TokenType::BitwiseXor);
    case '~': return makeToken(TokenType::BitwiseNot);
    case '!': return makeToken(expect('=') ? TokenType::NotEqual : TokenType::LogicalNot);
    case '=': return makeToken(expect('=') ? TokenType::Equal : TokenType::Assign);
    case '>': return makeToken(expect('>') ? TokenType::GreaterEqual : TokenType::Greater);
    case '<': return makeToken(expect('<') ? TokenType::LessEqual : TokenType::Less);
    default: {
        std::stringstream msg;
        msg << "unexpected character " << *ch;
        return cpp::fail(makeError(msg.str()));
    }
    }
}

cpp::result<std::optional<Token>, LexError> Lexer::skipWhitespace() noexcept {
    // ignore whitespace until the next token is found
    // if all whitespace is consumed, return {}
    while (true) {
        auto currentChar = peek();
        if (!currentChar) return {};

        bool isWindowsNewline = (*currentChar == '\r' && peekNext() && *peekNext() == '\n');
        if (isWindowsNewline || *currentChar == '\n') {
            m_start = m_current;
            // consume the newline characters
            advance();
            if (isWindowsNewline) {
                advance();
            }
            auto newlineToken = makeToken(TokenType::Newline);

            m_location.line++;
            m_location.column = 1;

            return newlineToken;
        } else if (std::isspace(*currentChar)) {
            advance();
        } else if (*currentChar == '/') {
            auto nextChar = peekNext();
            if (!nextChar) return {};

            if (*nextChar == '/') {
                ignoreLineComment();
            } else if (*nextChar == '*') {
                auto maybeError = ignoreBlockComment();
                if (maybeError.has_error()) {
                    return cpp::fail(maybeError.error());
                }
            } else {
                return {};
            }
        } else {
            return {};
        }
    }
}

void Lexer::ignoreLineComment() noexcept {
    // consume the '//'
    advance();
    advance();

    // parse comment until end of line
    while (peek() && *peek() != '\n') {
        advance();
    }
}

cpp::result<void, LexError> Lexer::ignoreBlockComment() noexcept {
    // consume '/*'
    advance();
    advance();

    // parse comment until reaching the * and \. (side note: c++ hates typing that combination.)
    while (true) {
        auto next = advance();
        if (!next) {
            return cpp::fail(makeError("unterminated block comment"));
        } else if (*next == '*' && expect('/')) {
            return {};
        } else if (*next == '\n') {
            // since this newline is inside a block comment,
            // it does not contribute to possible statement terminators
            m_location.line += 1;
            m_location.column = 1;
        }
    }
}

LexResult Lexer::lexString() noexcept {
    while (peek() && *peek() != '"') {
        auto maybeError = advanceStringChar("string literal");
        if (maybeError.has_error()) {
            return cpp::fail(maybeError.error());
        }
    }

    // consume closing quote
    advance();
    return makeToken(TokenType::StringLiteral);
}

LexResult Lexer::lexChar() noexcept {
    if (peek() && *peek() == '\'') {
        return cpp::fail(makeError("empty char literal"));
    }

    auto maybeError = advanceStringChar("char literal");
    if (maybeError.has_error()) {
        return cpp::fail(maybeError.error());
    }

    if (expect('\'')) {
        return makeToken(TokenType::CharLiteral);
    } else {
        return cpp::fail(makeError("unterminated char literal"));
    }
}

cpp::result<void, LexError> Lexer::advanceStringChar(const std::string &literalType) noexcept {
    auto nextChar = peek();
    if (!nextChar) {
        std::stringstream msg;
        msg << "unterminated " << literalType;
        return cpp::fail(makeError(msg.str()));
    }

    switch (*nextChar) {
    case '\n': {
        std::stringstream msg;
        msg << "unexpected newline in " << literalType;
        return cpp::fail(makeError(msg.str()));
    }
    case '\\': {
        // consume the backslash, then the escape sequence.
        advance();
        auto escapeSeq = peek();
        if (!escapeSeq) {
            std::stringstream msg;
            msg << "unterminated " << literalType;
            return cpp::fail(makeError(msg.str()));
        } else if (*escapeSeq == '0' || *escapeSeq == 't' || *escapeSeq == 'n' || *escapeSeq == 'r' ||
            *escapeSeq == '\'' || *escapeSeq == '"' || *escapeSeq == '\\') {
            advance();
        } else {
            std::stringstream msg;
            msg << "illegal escape sequence '\\" << *escapeSeq << "' in " << literalType;
            return cpp::fail(makeError(msg.str()));
        }
    }
    default:
        advance();
        return {};
    }
}

LexResult Lexer::lexNumber() noexcept {
    TokenType numberType = TokenType::IntegerLiteral;
    while (peek() && isDigit(*peek())) {
        advance();
    }

    bool currentIsPeriod = peek() && *peek() == '.';
    bool nextIsDigit = peekNext() && isDigit(*peekNext());
    if (currentIsPeriod && nextIsDigit) {
        numberType = TokenType::FloatLiteral;
        // consume the '.'
        advance();
        while (peek() && isDigit(*peek())) {
            advance();
        }
    }

    return makeToken(numberType);
}

LexResult Lexer::lexIdentifier() noexcept {
    while (true) {
        auto next = peek();
        if (next && isIdentifier(*next)) {
            advance();
        } else {
            break;
        }
    }

    std::string lexeme = m_code.substr(m_start, m_current);
    TokenType type = getTokenTypeForIdent(lexeme);
    return makeToken(type);
}

TokenType Lexer::getTokenTypeForIdent(const std::string &lexeme) noexcept {
    auto iter = IDENTIFIER_TYPES.find(lexeme);
    if (iter != IDENTIFIER_TYPES.cend()) {
        return iter->second;
    } else {
        return TokenType::Identifier;
    }
}

Token Lexer::makeToken(TokenType type) const noexcept {
    std::string lexeme = m_code.substr(m_start, m_current);
    return {type, lexeme, getStartLocation()};
}

LexError Lexer::makeError(const std::string &msg) const noexcept {
    return {msg, getStartLocation()};
}

SourceLocation Lexer::getStartLocation() const noexcept {
    int diff = static_cast<int>(m_current) - static_cast<int>(m_start);
    int actualColumn = m_location.column - diff;
    return {m_location.line, actualColumn};
}

std::optional<char> Lexer::peek() const noexcept {
    if (m_current >= m_code.length()) {
        return {};
    } else {
        return m_code[m_current];
    }
}

std::optional<char> Lexer::peekNext() const noexcept {
    if (m_current + 1 >= m_code.length()) {
        return {};
    } else {
        return m_code[m_current + 1];
    }
}

std::optional<char> Lexer::advance() noexcept {
    if (isAtEnd()) {
        return {};
    } else {
        m_current++;
        m_location.column++;
        return m_code[m_current - 1];
    }
}

bool Lexer::expect(char expected) noexcept {
    auto ch = peek();
    if (ch && *ch == expected) {
        advance();
        return true;
    } else {
        return false;
    }
}

bool Lexer::isAtEnd() const noexcept {
    return m_current >= m_code.length();
}

bool Lexer::isDigit(char ch) noexcept {
    return ch >= '0' && ch <= '9';
}

bool Lexer::isIdentifier(char ch) noexcept {
    return isDigit(ch) || isIdentifierStart(ch);
}

bool Lexer::isIdentifierStart(char ch) noexcept {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}
