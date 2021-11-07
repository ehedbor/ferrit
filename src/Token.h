#pragma once

#include <format>
#include <ostream>
#include <string>


namespace ferrit {
    /**
     * Represents every possible type of \c Token.
     */
    enum class TokenType {
        Error,
        EndOfFile,

        LeftParen,          ///< '('
        RightParen,         ///< ')'
        LeftBrace,          ///< '{'
        RightBrace,         ///< '}'
        LeftBracket,        ///< '['
        RightBracket,       ///< ']'

        Comma,              ///< ','
        Dot,                ///< '.'
        DotDot,             ///< '..'
        DotDotDot,          ///< '...'
        Colon,              ///< ':'
        ColonColon,         ///< '::'
        Question,           ///< '?'
        QuestionQuestion,   ///< '??'
        QuestionDot,        ///< '?.'
        QuestionColon,      ///< '?:'
        Arrow,              ///< '->'
        Semicolon,          ///< ';'
        Newline,            ///< '\\n'. Defines a possible statement terminator.

        Plus,               ///< '+'
        PlusPlus,           ///< '++'
        Minus,              ///< '-'
        MinusMinus,         ///< '--'
        Asterisk,           ///< '*'
        Slash,              ///< '/'
        Percent,            ///< '%'
        Tilde,              ///< '~'
        AndAnd,             ///< '&&'
        OrOr,               ///< '||'
        Bang,               ///< '!'
        BangBang,           ///< '!!'

        Equal,              ///< '='
        PlusEqual,          ///< '+='
        MinusEqual,         ///< '-='
        AsteriskEqual,      ///< '*='
        SlashEqual,         ///< '/='
        PercentEqual,       ///< '%='
        TildeEqual,         ///< '~='
        AndAndEqual,        ///< '&&='
        OrOrEqual,          ///< '||='

        EqualEqual,         ///< '=='
        BangEqual,          ///< '!='
        Greater,            ///< '>'
        GreaterEqual,       ///< '>='
        Less,               ///< '<'
        LessEqual,          ///< '<='

        As,                 ///< 'as'
        AsQuestion,         ///< 'as?'
        Is,                 ///< 'is'
        BangIs,             ///< '!is'
        In,                 ///< 'in'
        BangIn,             ///< '!in'

        Using,              ///< 'using'
        Module,             ///< 'module'
        Public,             ///< 'public'
        Protected,          ///< 'protected'
        Private,            ///< 'private'
        Companion,          ///< 'companion'
        Friend,             ///< 'friend'
        Open,               ///< 'open'
        Closed,             ///< 'closed'
        Abstract,           ///< 'abstract'
        Override,           ///< 'override'
        Operator,           ///< 'operator'
        Native,             ///< 'native'

        Class,              ///< 'class'
        Object,             ///< 'object'
        Trait,              ///< 'trait'
        Init,               ///< 'init'
        This,               ///< 'this'
        Super,              ///< 'super'
        Fun,                ///< 'fun'
        Var,                ///< 'var'
        Val,                ///< 'val'

        If,                 ///< 'if'
        Else,               ///< 'else'
        For,                ///< 'for'
        While,              ///< 'while'
        Do,                 ///< 'do'
        Return,             ///< 'return'
        Continue,           ///< 'continue'
        Break,              ///< 'break'

        True,               ///< 'true'
        False,              ///< 'false'
        Null,               ///< 'null'

        /**
         * \brief Identifiers represent the names of functions, types and variables.
         *
         * Identifiers begin with an ASCII letter or underscore and
         * contain ASCII letters, numbers and underscores.
         *
         * Identifiers may not be single underscores.
         */
        Identifier,

        /**
         * \brief String literals are a convenient way to create String objects.
         *
         * Strings begin and end with double quotes and contain 0 or more
         * ASCII characters (excluding newlines) or escape sequences.
         *
         * Escape sequences consist of a backslash and an ASCII letter.
         * Common examples are '\\n' for newline and '\\t' for tab.
         */
        StringLiteral,

        /**
         * \brief Char literals are a convenient way to create Char objects.
         *
         * Char literals begin and end with single quotes and contain exactly
         * one ASCII character (excluding newlines) or escape sequence.
         */
        CharLiteral,

        /**
         * \brief Integer literals are the literal representations of the types
         * Byte, Short, Int and Long.
         *
         * Integer literals begin with a digit, contain a mix of digits and
         * underscores, and may end with a suffix representing the literal's
         * type ('b' for Byte, 's' for Short, 'i' for Int, or 'L' for Long).
         *
         * For a decimal integer, a digit is one of the ten ASCII digits.
         * Literals beginning with '0x' are hexadecimal and can additionally
         * include A-F as digits (capital letters only), and those starting with
         * '0b' are binary and can only include '0' and '1' as digits.
         */
        IntegerLiteral,

        /**
         * \brief Float literals are the literal representations of the Float
         * and Double types.
         *
         * Float literals begin with a digit, followed by a mix of digits and
         * underscores. They are then followed by a '.' and another set of
         * digits and underscores.
         *
         * Next, an optional exponent specifier may occur. This consists of an
         * 'e' (lowercase) and a sequence of digits and underscores.
         *
         * Finally, an optional suffix may be present: 'f' for Float and 'd' for Double.
         */
        FloatLiteral,
    };

    /**
     * Outputs the given token type to the ostream. If a formatting error occurs the failbit is set.
     */
    std::ostream &operator<<(std::ostream &output, TokenType tokenType);

    /**
     * Represents a line:column location in a source code file.
     */
    struct SourceLocation final {
    public:
        /**
         * Initialize the SourceLocation to line 1, column 1.
         */
        SourceLocation() noexcept = default;

        /**
         * Construct a SourceLocation with the given line/column numbers.
         */
        SourceLocation(int line, int column) noexcept;

        bool operator==(const SourceLocation &other) const noexcept = default;

    public:
        int line{1};
        int column{1};
    };

    /**
     * Represents a well-formed token in a source code file.
     */
    struct Token final {
    public:
        /**
         * Construct an invalid token.
         */
        Token() noexcept = default;

        /**
         * Construct a token with the given options.
         */
        Token(TokenType tokenType, std::string lexeme, SourceLocation location) noexcept;

        bool operator==(const Token &other) const noexcept = default;

    public:
        TokenType type{TokenType::Error};
        std::string lexeme{"<unknown token>"};
        SourceLocation location{-1, -1};
    };
}

#define TOKEN_CASE(tokenName)                                                  \
    case ::ferrit::TokenType::tokenName:                                       \
        return std::formatter<string>::format(#tokenName, ctx)

template<>
struct std::formatter<ferrit::TokenType> : std::formatter<std::string> {
    auto format(const ferrit::TokenType tokenType, std::format_context &ctx) {
        switch (tokenType) {
        default:
            throw std::format_error("Unknown enum type for Token");
        TOKEN_CASE(Error);
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
        }
    }
};

#undef TOKEN_CASE
