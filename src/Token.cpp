#include "Token.h"


namespace ferrit {
#define TOKEN_CASE(tokenName)                                                  \
    case TokenType::tokenName:                                                 \
        out << #tokenName;                                                     \
        return out

    std::ostream &operator<<(std::ostream &out, TokenType type) {
        switch (type) {
        TOKEN_CASE(Unknown);
        TOKEN_CASE(EndOfFile);
        TOKEN_CASE(LeftParen);
        TOKEN_CASE(RightParen);
        TOKEN_CASE(LeftBrace);
        TOKEN_CASE(RightBrace);
        TOKEN_CASE(LeftBracket);
        TOKEN_CASE(RightBracket);
        TOKEN_CASE(Comma);
        TOKEN_CASE(Dot);
        TOKEN_CASE(DotDot);
        TOKEN_CASE(DotDotDot);
        TOKEN_CASE(Colon);
        TOKEN_CASE(ColonColon);
        TOKEN_CASE(Question);
        TOKEN_CASE(QuestionQuestion);
        TOKEN_CASE(QuestionDot);
        TOKEN_CASE(QuestionColon);
        TOKEN_CASE(Arrow);
        TOKEN_CASE(Semicolon);
        TOKEN_CASE(Newline);
        TOKEN_CASE(Plus);
        TOKEN_CASE(PlusPlus);
        TOKEN_CASE(Minus);
        TOKEN_CASE(MinusMinus);
        TOKEN_CASE(Asterisk);
        TOKEN_CASE(Slash);
        TOKEN_CASE(Percent);
        TOKEN_CASE(Tilde);
        TOKEN_CASE(AndAnd);
        TOKEN_CASE(OrOr);
        TOKEN_CASE(Bang);
        TOKEN_CASE(BangBang);
        TOKEN_CASE(Equal);
        TOKEN_CASE(PlusEqual);
        TOKEN_CASE(MinusEqual);
        TOKEN_CASE(AsteriskEqual);
        TOKEN_CASE(SlashEqual);
        TOKEN_CASE(PercentEqual);
        TOKEN_CASE(TildeEqual);
        TOKEN_CASE(AndAndEqual);
        TOKEN_CASE(OrOrEqual);
        TOKEN_CASE(EqualEqual);
        TOKEN_CASE(BangEqual);
        TOKEN_CASE(Greater);
        TOKEN_CASE(GreaterEqual);
        TOKEN_CASE(Less);
        TOKEN_CASE(LessEqual);
        TOKEN_CASE(As);
        TOKEN_CASE(AsQuestion);
        TOKEN_CASE(Is);
        TOKEN_CASE(BangIs);
        TOKEN_CASE(In);
        TOKEN_CASE(BangIn);
        TOKEN_CASE(Using);
        TOKEN_CASE(Module);
        TOKEN_CASE(Public);
        TOKEN_CASE(Protected);
        TOKEN_CASE(Private);
        TOKEN_CASE(Companion);
        TOKEN_CASE(Friend);
        TOKEN_CASE(Open);
        TOKEN_CASE(Closed);
        TOKEN_CASE(Abstract);
        TOKEN_CASE(Override);
        TOKEN_CASE(Operator);
        TOKEN_CASE(Native);
        TOKEN_CASE(Class);
        TOKEN_CASE(Object);
        TOKEN_CASE(Trait);
        TOKEN_CASE(Init);
        TOKEN_CASE(This);
        TOKEN_CASE(Super);
        TOKEN_CASE(Fun);
        TOKEN_CASE(Var);
        TOKEN_CASE(Val);
        TOKEN_CASE(If);
        TOKEN_CASE(Else);
        TOKEN_CASE(For);
        TOKEN_CASE(While);
        TOKEN_CASE(Do);
        TOKEN_CASE(Return);
        TOKEN_CASE(Continue);
        TOKEN_CASE(Break);
        TOKEN_CASE(True);
        TOKEN_CASE(False);
        TOKEN_CASE(Null);
        TOKEN_CASE(Identifier);
        TOKEN_CASE(StringLiteral);
        TOKEN_CASE(CharLiteral);
        TOKEN_CASE(IntegerLiteral);
        TOKEN_CASE(FloatLiteral);
        default:
            throw std::logic_error("Unknown enum type for Token");
        }
    }

#undef TOKEN_CASE

    std::ostream &operator<<(std::ostream &out, const SourceLocation &location) {
        out << location.line << ":" << location.column;
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const Token &token) {
        out << "Token{" << token.type << " \"" << token.lexeme << "\" at " << token.location << "}";
        return out;
    }
}
