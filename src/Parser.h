#pragma once

#include "ErrorReporter.h"
#include "Expression.h"
#include "ParseError.h"
#include "Statement.h"
#include "Token.h"

#include <memory>
#include <optional>
#include <vector>


namespace ferrit {
    /**
     * Converts a stream of tokens into an abstract syntax tree.
     */
    class Parser final {
    public:
        Parser() noexcept = default;

        /**
         * Constructs a \c Parser with the given error reporter.
         *
         * @param errorReporter logger for compile errors
         */
        explicit Parser(std::shared_ptr<const ErrorReporter> errorReporter) noexcept;

    private:
        /**
         * Initializes the parser with the given tokens.
         */
        void init(const std::vector<Token> &tokens) noexcept;

    public:

        /**
         * Parses an entire stream of tokens, representing an entire file.
         */
        [[nodiscard]] std::optional<std::vector<StatementPtr>> parse(const std::vector<Token> &tokens);

    private:
        // Declarations
        [[nodiscard]] StatementPtr parseDeclaration();
        [[nodiscard]] StatementPtr parseFunctionDeclaration(const std::vector<Token>& modifiers);

        // Supporting AST elements
        [[nodiscard]] std::vector<Token> parseModifiers();
        [[nodiscard]] std::vector<Parameter> parseParameters();
        [[nodiscard]] Parameter parseParameter();
        [[nodiscard]] DeclaredType parseType();

        // Other statements
        [[nodiscard]] StatementPtr parseStatement();
        [[nodiscard]] StatementPtr parseBlock();

        // Operators
        [[nodiscard]] ExpressionPtr parseExpression();
        [[nodiscard]] ExpressionPtr parseDisjunction();
        [[nodiscard]] ExpressionPtr parseConjunction();
        [[nodiscard]] ExpressionPtr parseEquality();
        [[nodiscard]] ExpressionPtr parseComparison();
        [[nodiscard]] ExpressionPtr parseAdditive();
        [[nodiscard]] ExpressionPtr parseMultiplicative();
        [[nodiscard]] ExpressionPtr parseUnaryPrefix();
        [[nodiscard]] ExpressionPtr parseUnaryPostfix();
        [[nodiscard]] std::vector<ExpressionPtr> parseArguments();

        // Simple expressions
        [[nodiscard]] ExpressionPtr parsePrimary();
        [[nodiscard]] ExpressionPtr parseParenthesizedExpr();
        [[nodiscard]] ExpressionPtr parseVariable();
        [[nodiscard]] ExpressionPtr parseNumber();

        /**
         * Attempts to recover from an error by skipping tokens until finding
         * one that is likely to start a new line. This is to allow for finding
         * multiple parse errors at once.
         */
        void synchronize() noexcept;

        struct FoundTerminators {
            bool newline = false;
            bool semicolon = false;
            bool eof = false;

            [[nodiscard]] bool any() const noexcept { return newline || semicolon || eof; }
        };
        /**
         * Skips all line terminators encountered until the next
         * non-line terminator token is found.
         *
         * @param allowSemicolons if semicolons are permitted
         * @return which kinds of terminators were found
         */
        FoundTerminators skipTerminators(bool allowSemicolons) noexcept;

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current token matches the given <tt>TokenType</tt>. If it does,
         * the parser is advanced and the next token is returned.
         *
         * @param expected token type to check for
         * @param errMsg error message if the expected token type is not present
         * @return the token, if it matches the expected type
         * @throws ParseException if the current token does not match the expected type
         */
        const Token &consume(TokenType expected, const std::string &errMsg);

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current token matches the given <tt>TokenType</tt>. If it does,
         * the parser is advanced.
         *
         * @param expected the token type to check for
         * @return true if the current token matches \c expected
         */
        [[nodiscard]] bool match(TokenType expected) noexcept;

        /**
         * Skips all non-semicolon line terminators, then checks to see
         * if the current token's type matches the given <tt>TokenType</tt>.
         */
        [[nodiscard]] bool check(TokenType expected) noexcept;

        /**
         * Advances the parser (unless EOF is reached) and then returns
         * the current token.
         */
        const Token &advance() noexcept;

        /**
         * Returns the current token.
         */
        [[nodiscard]] const Token &current() const noexcept;

        /**
         * Returns the last token.
         */
        [[nodiscard]] const Token &previous() const noexcept;

        /**
         * Returns true if there are no more tokens.
         */
        [[nodiscard]] bool isAtEnd() const noexcept;

        /**
         * Constructs a \c ParseException with the given logError message, logs
         * the error, and returns.
         *
         * Note that the exception is not thrown; it is the user's decision to
         * throw it.
         *
         * @param expected error message
         * @return the exception
         */
        [[nodiscard]] ParseError makeError(const std::string &expected) const;

    private:
        std::shared_ptr<const ErrorReporter> m_errorReporter{nullptr};
        std::vector<Token> m_tokens{};
        int m_current{0};
    };
}
