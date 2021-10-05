#pragma once

#include <ostream>
#include <string>


namespace ferrit {
    /**
     * Represents every possible type of \c Token.
     */
    enum class TokenType {
        Unknown,
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

    std::ostream &operator<<(std::ostream &out, TokenType type);

    /**
     * Represents a line:column location in a source code file.
     */
    struct SourceLocation {
    public:
        bool operator==(const SourceLocation &other) const noexcept = default;

        friend std::ostream &operator<<(std::ostream &out, const SourceLocation &loc);

    public:
        int line{1};
        int column{1};
    };

    /**
     * Represents a well-formed token in a source code file.
     */
    struct Token {
    public:
        bool operator==(const Token &other) const noexcept = default;

        friend std::ostream &operator<<(std::ostream &out, const Token &token);

    public:
        TokenType type{};
        std::string lexeme{};
        SourceLocation location{};
    };
}