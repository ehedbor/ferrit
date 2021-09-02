#include "Token.h"


namespace ferrit {
#define TOKEN_CASE(tokenName)                                                  \
    case TokenType::tokenName:                                                 \
        out << #tokenName;                                                     \
        return out

    std::ostream &operator<<(std::ostream &out, TokenType type) {
        switch (type) {
        TOKEN_CASE(LeftParen);
        TOKEN_CASE(RightParen);
        TOKEN_CASE(LeftBrace);
        TOKEN_CASE(RightBrace);
        TOKEN_CASE(LeftBracket);
        TOKEN_CASE(RightBracket);
        TOKEN_CASE(Comma);
        TOKEN_CASE(Period);
        TOKEN_CASE(Arrow);
        TOKEN_CASE(Colon);
        TOKEN_CASE(Semicolon);
        TOKEN_CASE(Newline);
        TOKEN_CASE(Plus);
        TOKEN_CASE(Minus);
        TOKEN_CASE(Times);
        TOKEN_CASE(Divide);
        TOKEN_CASE(Modulo);
        TOKEN_CASE(LogicalAnd);
        TOKEN_CASE(LogicalOr);
        TOKEN_CASE(LogicalNot);
        TOKEN_CASE(BitwiseAnd);
        TOKEN_CASE(BitwiseOr);
        TOKEN_CASE(BitwiseXor);
        TOKEN_CASE(BitwiseNot);
        TOKEN_CASE(BitwiseLeftShift);
        TOKEN_CASE(BitwiseRightShift);
        TOKEN_CASE(Assign);
        TOKEN_CASE(Equal);
        TOKEN_CASE(NotEqual);
        TOKEN_CASE(Greater);
        TOKEN_CASE(GreaterEqual);
        TOKEN_CASE(Less);
        TOKEN_CASE(LessEqual);
        TOKEN_CASE(Native);
        TOKEN_CASE(Var);
        TOKEN_CASE(Fun);
        TOKEN_CASE(Return);
        TOKEN_CASE(True);
        TOKEN_CASE(False);
        TOKEN_CASE(Int);
        TOKEN_CASE(Double);
        TOKEN_CASE(Void);
        TOKEN_CASE(Identifier);
        TOKEN_CASE(StringLiteral);
        TOKEN_CASE(CharLiteral);
        TOKEN_CASE(IntegerLiteral);
        TOKEN_CASE(FloatLiteral);
        TOKEN_CASE(EndOfFile);
        default:
            throw std::logic_error("Unknown enum type for Token");
        }
    }

#undef TOKEN_CASE

    SourceLocation::SourceLocation(std::size_t line, std::size_t column) noexcept :
        line(line), column(column) {
    }

    bool SourceLocation::operator==(const SourceLocation &other) const noexcept {
        if (this == &other) return true;
        return line == other.line && column == other.column;
    }

    bool SourceLocation::operator!=(const SourceLocation &other) const noexcept {
        return !(*this == other);
    }

    std::ostream &operator<<(std::ostream &out, const SourceLocation &loc) {
        out << loc.line << ":" << loc.column;
        return out;
    }

    Token::Token(TokenType type, std::string lexeme, SourceLocation location) noexcept :
        type(type), lexeme(std::move(lexeme)), location(location) {
    }

    bool Token::operator==(const Token &other) const noexcept {
        if (this == &other) return true;
        return type == other.type && lexeme == other.lexeme && location == other.location;
    }

    bool Token::operator!=(const Token &other) const noexcept {
        return !(*this == other);
    }

    std::ostream &operator<<(std::ostream &out, const Token &token) {
        out << "Token{" << token.type << " \"" << token.lexeme << "\" at " << token.location << "}";
        return out;
    }
}
