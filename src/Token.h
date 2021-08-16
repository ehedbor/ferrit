#pragma once

#include <string>

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
    //Arrow,              ///< '<-'
    //Colon,              ///< ':'
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
    //DebugPrint,

    //SByte,
    //Byte,
    //Short,
    //UShort,
    //Int,
    //UInt,
    //Long,
    //ULong,
    //Float,
    //Double,
    //Char,
    //String,
    //Bool,

    Identifier,
    StringLiteral,
    CharLiteral,
    IntegerLiteral,
    FloatLiteral,

    EndOfFile,
};

struct SourceLocation {
public:
    SourceLocation() = default;
    SourceLocation(int line, int column);

public:
    int line{0};
    int column{0};
};

struct Token {
public:
    Token() = default;
    Token(TokenType type, std::string lexeme, SourceLocation location);

public:
    TokenType type{};
    std::string lexeme{};
    SourceLocation location{};
};

