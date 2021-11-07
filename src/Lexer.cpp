#include "Lexer.h"

#include <unordered_map>


namespace ferrit {
    Lexer::Lexer(std::shared_ptr<const ErrorReporter> errorReporter) noexcept :
        m_errorReporter(std::move(errorReporter)) {
    }

    void Lexer::init(const std::string &code) noexcept {
        m_code = code;
        m_start = 0;
        m_current = 0;
        m_location = {1, 1};
    }

    std::optional<std::vector<Token>> Lexer::lex(const std::string &code) {
        init(code);

        try {
            std::vector<Token> result;
            while (true) {
                result.emplace_back(lexNext());
                const Token &token = result.back();
                if (token.type == TokenType::EndOfFile) {
                    return result;
                }
            }
        }  catch (const Error &) {
            return {};
        }
    }

    Token Lexer::lexNext() {
        if (auto whitespace = skipWhitespace()) {
            return *whitespace;
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
        case '.':
            if (match('.')) {
                if (match('.')) {
                    return makeToken(TokenType::DotDotDot);
                }
                return makeToken(TokenType::DotDot);
            }
            return makeToken(TokenType::Dot);
        case '?':
            if (match('?')) return makeToken(TokenType::QuestionQuestion);
            if (match(':')) return makeToken(TokenType::QuestionColon);
            if (match('.')) return makeToken(TokenType::QuestionDot);
            return makeToken(TokenType::Question);
        case ':':
            if (match(':')) return makeToken(TokenType::ColonColon);
            return makeToken(TokenType::Colon);
        case ';':
            return makeToken(TokenType::Semicolon);
        case '+':
            if (match('+')) return makeToken(TokenType::PlusPlus);
            if (match('=')) return makeToken(TokenType::PlusEqual);
            return makeToken(TokenType::Plus);
        case '-':
            if (match('-')) return makeToken(TokenType::MinusMinus);
            if (match('=')) return makeToken(TokenType::MinusEqual);
            if (match('>')) return makeToken(TokenType::Arrow);
            return makeToken(TokenType::Minus);
        case '*':
            if (match('=')) return makeToken(TokenType::AsteriskEqual);
            return makeToken(TokenType::Asterisk);
        case '/':
            if (match('=')) return makeToken(TokenType::SlashEqual);
            return makeToken(TokenType::Slash);
        case '%':
            if (match('=')) return makeToken(TokenType::PercentEqual);
            return makeToken(TokenType::Percent);
        case '~':
            if (match('=')) return makeToken(TokenType::TildeEqual);
            return makeToken(TokenType::Tilde);
        case '&':
            if (match('&')) {
                if (match('=')) {
                    return makeToken(TokenType::AndAndEqual);
                }
                return makeToken(TokenType::AndAnd);
            }
            throw makeError<Error::UnexpectedChar>('&');
        case '|':
            if (match('|')) {
                if (match('=')) {
                    return makeToken(TokenType::OrOrEqual);
                }
                return makeToken(TokenType::OrOr);
            }
            throw makeError<Error::UnexpectedChar>('|');
        case '!':
            if (match('!')) {
                return makeToken(TokenType::BangBang);
            } else if (match('=')) {
                return makeToken(TokenType::BangEqual);
            } else if (peek() && peekNext()) {
                bool secondNextIsIdent = isIdentifier(peekN(2).value_or('\0'));
                if (*peek() == 'i' && *peekNext() == 's' && !secondNextIsIdent) {
                    advance();
                    advance();
                    return makeToken(TokenType::BangIs);
                } else if (*peek() == 'i' && *peekNext() == 'n' && !secondNextIsIdent) {
                    advance();
                    advance();
                    return makeToken(TokenType::BangIn);
                }
            }
            return makeToken(TokenType::Bang);
        case '=':
            if (match('=')) return makeToken(TokenType::EqualEqual);
            return makeToken(TokenType::Equal);
        case '>':
            if (match('=')) return makeToken(TokenType::GreaterEqual);
            return makeToken(TokenType::Greater);
        case '<':
            if (match('=')) return makeToken(TokenType::LessEqual);
            return makeToken(TokenType::Less);
        default:
            throw makeError<Error::UnexpectedChar>(*ch);
        }
    }

    std::optional<Token> Lexer::skipWhitespace() {
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
                    ignoreBlockComment();
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

        while (getCurrentNewlineType() <= 0) {
            advance();
        }
    }

    void Lexer::ignoreBlockComment() {
        // consume '/*'
        advance();
        advance();

        // parse comment until reaching the * and \. (side note: c++ hates typing that combination.)
        while (true) {
            auto next = advance();
            if (!next) {
                throw makeError<Error::UnterminatedElement>("block comment");
            } else if (*next == '*' && match('/')) {
                return;
            } else if (*next == '\n') {
                // since this newline is inside a block comment,
                // it does not contribute to possible statement terminators
                m_location.line += 1;
                m_location.column = 1;
            }
        }
    }

    Token Lexer::lexString() {
        while (peek() && *peek() != '"') {
            advanceStringChar("string literal");
        }

        // consume closing quote
        if (match('"')) {
            return makeToken(TokenType::StringLiteral);
        } else {
            throw makeError<Error::UnterminatedElement>("string literal");
        }
    }

    Token Lexer::lexChar() {
        if (peek() && *peek() == '\'') {
            throw makeError<Error::EmptyElement>("char literal");
        }
        advanceStringChar("char literal");

        if (match('\'')) {
            return makeToken(TokenType::CharLiteral);
        } else if (!peek()) {
            throw makeError<Error::UnterminatedElement>("string literal");
        } else {
            throw makeError<Error::CharLiteralTooBig>();
        }
    }

    void Lexer::advanceStringChar(const std::string &literalType) {
        auto nextChar = peek();
        if (!nextChar) {
            throw makeError<Error::UnterminatedElement>(literalType);
        }

        switch (*nextChar) {
        case '\n':
            throw makeError<Error::UnexpectedNewline>(literalType);
        case '\\': {
            // consume the backslash, then the escape sequence.
            advance();
            auto escapeSeq = peek();
            if (!escapeSeq) {
                throw makeError<Error::UnterminatedElement>(literalType);
            } else if (*escapeSeq == '0' || *escapeSeq == 't' || *escapeSeq == 'n' || *escapeSeq == 'r' ||
                *escapeSeq == '\'' || *escapeSeq == '"' || *escapeSeq == '\\') {
                advance();
                return;
            } else {
                throw makeError<Error::IllegalEscapeSequence>(*escapeSeq, literalType);
            }
        }
        default:
            advance();
            return;
        }
    }

    Token Lexer::lexNumber() {
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
        if (peek() && isIdentifier(*peek())) {
            int start = m_current;
            advance();
            while (peek().has_value() && isIdentifier(*peek())) {
                advance();
            }

            int count = m_current - start;
            std::string lexeme = m_code.substr(start, count);

            auto literalType = (numberType == TokenType::IntegerLiteral)
                ? "integer literal" : "float literal";

            throw makeError<Error::UnknownLiteralSuffix>(literalType, lexeme);
        }

        return makeToken(numberType);
    }

    Token Lexer::lexIdentifier() noexcept {
        while (true) {
            auto next = peek();
            if (next && isIdentifier(*next)) {
                advance();
            } else {
                break;
            }
        }
        return makeToken(getCurrentKeywordType());
    }

    TokenType Lexer::getCurrentKeywordType() noexcept {
        static const std::unordered_map<std::string, TokenType> IDENTIFIER_TYPES = {
            {"as",          TokenType::As},
            {"is",          TokenType::Is},
            {"in",          TokenType::In},
            {"using",       TokenType::Using},
            {"module",      TokenType::Module},
            {"public",      TokenType::Public},
            {"protected",   TokenType::Protected},
            {"private",     TokenType::Private},
            {"companion",   TokenType::Companion},
            {"friend",      TokenType::Friend},
            {"open",        TokenType::Open},
            {"closed",      TokenType::Closed},
            {"abstract",    TokenType::Abstract},
            {"override",    TokenType::Override},
            {"operator",    TokenType::Operator},
            {"native",      TokenType::Native},
            {"class",       TokenType::Class},
            {"object",      TokenType::Object},
            {"trait",       TokenType::Trait},
            {"init",        TokenType::Init},
            {"this",        TokenType::This},
            {"super",       TokenType::Super},
            {"fun",         TokenType::Fun},
            {"var",         TokenType::Var},
            {"val",         TokenType::Val},
            {"if",          TokenType::If},
            {"else",        TokenType::Else},
            {"for",         TokenType::For},
            {"while",       TokenType::While},
            {"do",          TokenType::Do},
            {"return",      TokenType::Return},
            {"continue",    TokenType::Continue},
            {"break",       TokenType::Break},
            {"true",        TokenType::True},
            {"false",       TokenType::False},
            {"null",        TokenType::Null},
        };

        int count = m_current - m_start;
        std::string lexeme = m_code.substr(m_start, count);

        auto iter = IDENTIFIER_TYPES.find(lexeme);
        if (iter != IDENTIFIER_TYPES.cend()) {
            TokenType identType = iter->second;
            if (identType == TokenType::As && match('?')) {
                return TokenType::AsQuestion;
            } else {
                return identType;
            }
        } else {
            return TokenType::Identifier;
        }
    }

    Token Lexer::makeToken(TokenType type) const noexcept {
        int count = m_current - m_start;
        int startColumn = m_location.column - count;
        std::string lexeme = m_code.substr(m_start, count);
        return {type, std::move(lexeme), {m_location.line, startColumn}};
    }

    std::optional<char> Lexer::peek() const noexcept {
        return peekN(0);
    }

    std::optional<char> Lexer::peekNext() const noexcept {
        return peekN(1);
    }

    std::optional<char> Lexer::peekN(int n) const noexcept {
        if (m_current + n >= m_code.length()) {
            return {};
        } else {
            return m_code[m_current + n];
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
        if (peek() == expected) {
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
        if (peek() == '\n') {
            return 1;
        } else if (peek() == '\r' && peekNext() == '\n') {
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
