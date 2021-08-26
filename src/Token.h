#pragma once

#include <ostream>
#include <string>


namespace es {
    enum class TokenType {
        LeftParen,          ///< '('
        RightParen,         ///< ')'
        LeftBrace,          ///< '{'
        RightBrace,         ///< '}'
        LeftBracket,        ///< '['
        RightBracket,       ///< ']'

        //Nullable,           ///< '?'
        //NullOrElse,         ///< '?:'
        Comma,              ///< ','
        Period,             ///< '.'
        //SafeAccess,         ///< '?.'
        //RangeTo,            ///< '..'
        Arrow,              ///< '->'
        Colon,              ///< ':'
        Semicolon,          ///< ';'
        Newline,            ///< '\\n'. Defines a possible statement terminator.

        Plus,               ///< '+'
        Minus,              ///< '-'
        Times,              ///< '*'
        Divide,             ///< '/'
        Modulo,             ///< '%'

        LogicalAnd,         ///< '&&'
        LogicalOr,          ///< '||'
        LogicalNot,         ///< '!'

        BitwiseAnd,         ///< '&'
        BitwiseOr,          ///< '|'
        BitwiseXor,         ///< '^'
        BitwiseNot,         ///< '~'
        BitwiseLeftShift,           ///< '<<'
        BitwiseRightShift,          ///< '>>'
        BitwiseUnsignedRightShift,  ///< '>>>'

        Assign,             ///< '='
        //PlusAssign,         ///< '+='
        //MinusAssign,        ///< '-='
        //TimesAssign,        ///< '*='
        //DivideAssign,       ///< '/='
        //ModuloAssign,       ///< '%='
        //LogicalAndAssign,   ///< '&&='
        //LogicalOrAssign,    ///< '||='
        //BitwiseAndAssign,   ///< '&='
        //BitwiseOrAssign,    ///< '|='
        //BitwiseXorAssign,   ///< '^='
        //BitwiseLeftShiftAssign, ///< '<<='
        //BitwiseRightShiftAssign, ///< '>>='
        //BitwiseUnsignedRightShiftAssign, ///< now this is getting a bit ridiculous... '>>>='

        Equal,              ///< '=='
        NotEqual,           ///< '!='
        Greater,            ///< '>'
        GreaterEqual,       ///< '>='
        Less,               ///< '<'
        LessEqual,          ///< '<='

        //As,
        //Is,
        //In,
        //Module,
        //Using,
        Native,
        Var,
        //Val,
        //Class,
        Fun,
        //Init,
        //This,
        //Super,
        //If,
        //Else,
        //For,
        //While,
        //Do,
        Return,
        //Continue,
        //Break,
        True,
        False,
        //Null,

        //SByte,
        //Byte,
        //Short,
        //UShort,
        Int,
        //UInt,
        //Long,
        //ULong,
        //Float,
        Double,
        //Char,
        //String,
        //Bool,
        Void,

        Identifier,
        StringLiteral,
        CharLiteral,
        IntegerLiteral,
        FloatLiteral,

        EndOfFile,
    };

    std::ostream &operator<<(std::ostream &out, TokenType type);

    struct SourceLocation {
    public:
        SourceLocation() noexcept = default;
        SourceLocation(std::size_t line, std::size_t column) noexcept;

        bool operator==(const SourceLocation &other) const noexcept;
        bool operator!=(const SourceLocation &other) const noexcept;

        friend std::ostream &operator<<(std::ostream &out, const SourceLocation &loc);

    public:
        std::size_t line{1};
        std::size_t column{1};
    };

    struct Token {
    public:
        Token() noexcept = default;
        Token(TokenType type, std::string lexeme, SourceLocation location) noexcept;

        bool operator==(const Token &other) const noexcept;
        bool operator!=(const Token &other) const noexcept;

        friend std::ostream &operator<<(std::ostream &out, const Token &token);

    public:
        TokenType type{};
        std::string lexeme{};
        SourceLocation location{};
    };
}