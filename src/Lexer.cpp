#include "Lexer.h"

#include <sstream>
#include <unordered_map>
#include "ResultTry.h"


namespace ferrit {
    Lexer::Lexer(std::string code) noexcept : m_code(std::move(code)) {
    }

    LexResult Lexer::lex() {
        TRY(optWhitespaceToken, skipWhitespace());
        if (optWhitespaceToken) {
            return *optWhitespaceToken;
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
        case ':': return makeToken(TokenType::Colon);
        case ';': return makeToken(TokenType::Semicolon);
        case '+': return makeToken(TokenType::Plus);
        case '-': return makeToken(match('>') ? TokenType::Arrow : TokenType::Minus);
        case '*': return makeToken(TokenType::Times);
        case '/': return makeToken(TokenType::Divide);
        case '%': return makeToken(TokenType::Modulo);
        case '&': return makeToken(match('&') ? TokenType::LogicalAnd : TokenType::BitwiseAnd);
        case '|': return makeToken(match('|') ? TokenType::LogicalOr : TokenType::BitwiseOr);
        case '^': return makeToken(TokenType::BitwiseXor);
        case '~': return makeToken(TokenType::BitwiseNot);
        case '!': return makeToken(match('=') ? TokenType::NotEqual : TokenType::LogicalNot);
        case '=': return makeToken(match('=') ? TokenType::Equal : TokenType::Assign);
        case '>':
            if (match('>')) return makeToken(TokenType::BitwiseRightShift);
            if (match('=')) return makeToken(TokenType::GreaterEqual);
            return makeToken(TokenType::Greater);
        case '<':
            if (match('<')) return makeToken(TokenType::BitwiseLeftShift);
            if (match('=')) return makeToken(TokenType::LessEqual);
            return makeToken(TokenType::Less);
        default:
            return cpp::fail(makeError(ErrorCode::SyntaxUnexpectedChar, *ch));
        }
    }

    cpp::result<std::optional<Token>, Error> Lexer::skipWhitespace() {
        // ignore whitespace until the next token is found
        // if all whitespace is consumed, return {}
        while (true) {
            auto currentChar = peek();
            if (!currentChar) return {};

            int newlineType = getCurrentNewlineType();
            if (newlineType > 0) {
                m_start = m_current;
                // consume the newline characters
                advance();
                if (newlineType == 2) {
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
                    EXPECT(ignoreBlockComment());
                } else {
                    return {};
                }
            } else {
                return {};
            }
        }
    }

    void Lexer::ignoreLineComment() {
        // consume the '//'
        advance();
        advance();

        // parse comment until end of line

        while (getCurrentNewlineType() <= 0) {
            advance();
        }
    }

    cpp::result<void, Error> Lexer::ignoreBlockComment() {
        // consume '/*'
        advance();
        advance();

        // parse comment until reaching the * and \. (side note: c++ hates typing that combination.)
        while (true) {
            auto next = advance();
            if (!next) {
                return cpp::fail(makeError(ErrorCode::SyntaxUnterminatedElement, "block comment"));
            } else if (*next == '*' && match('/')) {
                return {};
            } else if (*next == '\n') {
                // since this newline is inside a block comment,
                // it does not contribute to possible statement terminators
                m_location.line += 1;
                m_location.column = 1;
            }
        }
    }

    LexResult Lexer::lexString() {
        while (peek() && *peek() != '"') {
            EXPECT(advanceStringChar("string literal"));
        }

        // consume closing quote
        if (match('"')) {
            return makeToken(TokenType::StringLiteral);
        } else {
            return cpp::fail(makeError(ErrorCode::SyntaxUnterminatedElement, "string literal"));
        }
    }

    LexResult Lexer::lexChar() {
        if (peek() && *peek() == '\'') {
            return cpp::fail(makeError(ErrorCode::SyntaxEmptyElement, "char literal"));
        }
        EXPECT(advanceStringChar("char literal"));

        if (match('\'')) {
            return makeToken(TokenType::CharLiteral);
        } else if (!peek()) {
            return cpp::fail(makeError(ErrorCode::SyntaxUnterminatedElement, "string literal"));
        } else {
            return cpp::fail(makeError(ErrorCode::SyntaxCharLiteralTooBig));
        }
    }

    cpp::result<void, Error> Lexer::advanceStringChar(const std::string &literalType) {
        auto nextChar = peek();
        if (!nextChar) {
            return cpp::fail(makeError(ErrorCode::SyntaxUnterminatedElement, literalType));
        }

        switch (*nextChar) {
        case '\n': {
            return cpp::fail(makeError(ErrorCode::SyntaxUnexpectedNewlineInElement, literalType));
        }
        case '\\': {
            // consume the backslash, then the escape sequence.
            advance();
            auto escapeSeq = peek();
            if (!escapeSeq) {
                return cpp::fail(makeError(ErrorCode::SyntaxUnterminatedElement, literalType));
            } else if (*escapeSeq == '0' || *escapeSeq == 't' || *escapeSeq == 'n' || *escapeSeq == 'r' ||
                *escapeSeq == '\'' || *escapeSeq == '"' || *escapeSeq == '\\') {
                advance();
                return {};
            } else {
                return cpp::fail(makeError(ErrorCode::SyntaxIllegalEscapeSequence, *escapeSeq, literalType));
            }
        }
        default:
            advance();
            return {};
        }
    }

    LexResult Lexer::lexNumber() {
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
        // prevent numbers from being immediately followed by an identifier
        char nextChar = peek().value_or('\0');
        if (isIdentifierStart(nextChar)) {
            return cpp::fail(makeError(ErrorCode::SyntaxUnknownLiteralSuffix, nextChar));
        }

        return makeToken(numberType);
    }

    LexResult Lexer::lexIdentifier() {
        while (true) {
            auto next = peek();
            if (next && isIdentifier(*next)) {
                advance();
            } else {
                break;
            }
        }

        std::size_t count = m_current - m_start;
        std::string lexeme = m_code.substr(m_start, count);
        TokenType type = getTokenTypeForIdent(lexeme);
        return makeToken(type);
    }

    TokenType Lexer::getTokenTypeForIdent(const std::string &lexeme) noexcept {
        static const std::unordered_map<std::string, TokenType> IDENTIFIER_TYPES = {
            {"native",      TokenType::Native},
            {"var",         TokenType::Var},
            {"fun",         TokenType::Fun},
            {"return",      TokenType::Return},
            {"true",        TokenType::True},
            {"false",       TokenType::False},
            {"int",         TokenType::Int},
            {"double",      TokenType::Double},
            {"void",        TokenType::Void},
        };

        auto iter = IDENTIFIER_TYPES.find(lexeme);
        if (iter != IDENTIFIER_TYPES.cend()) {
            return iter->second;
        } else {
            return TokenType::Identifier;
        }
    }

    Token Lexer::makeToken(TokenType type) const noexcept {
        std::size_t count = m_current - m_start;
        std::size_t startColumn = m_location.column - count;
        std::string lexeme = m_code.substr(m_start, count);
        return {type, std::move(lexeme), {m_location.line, startColumn}};
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

    bool Lexer::match(char expected) noexcept {
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

    int Lexer::getCurrentNewlineType() const noexcept {
        if (peek() && *peek() == '\n') {
            return 1;
        } else if (peek() && *peek() == '\r' && peekNext() && *peekNext() == '\n') {
            return 2;
        }
        return 0;
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
}
