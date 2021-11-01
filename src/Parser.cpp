#include "Parser.h"
#include <iostream>
#include "AstPrinter.h"

namespace ferrit {
    Parser::Parser(
        std::shared_ptr<const CompileOptions> options,
        std::shared_ptr<ErrorReporter> errorReporter) noexcept :
        m_options(std::move(options)), m_errorReporter(std::move(errorReporter)) {
    }

    void Parser::init(const std::vector<Token> &tokens) noexcept {
        m_tokens = tokens;
        m_current = 0;
        m_stackTrace.clear();
    }

    std::optional<std::vector<StatementPtr>> Parser::parse(const std::vector<Token> &tokens) {
        init(tokens);

        std::vector<StatementPtr> program;
        bool hadError = false;

        skipTerminators(true);
        while (!isAtEnd()) {
            try {
                StatementPtr nextDecl = parseDeclaration();
                program.push_back(std::move(nextDecl));
                skipTerminators(true);
            } catch (const ParseException &) {
                hadError = true;
                synchronize();
            }
        }

        if (hadError) {
            return {};
        } else {
            return program;
        }
    }

    StatementPtr Parser::parseDeclaration() {
        auto mods = parseModifiers();
        if (match(TokenType::Fun)) {
            return parseFunctionDeclaration(mods);
        }
        if (mods.empty()) {
            return parseStatement();
        }
        throw makeError("expected declaration");
    }

    StatementPtr Parser::parseFunctionDeclaration(const std::vector<Token> &modifiers) {
        // remember the keyword
        const Token &keyword = previous();
        const Token &name = consume(TokenType::Identifier, "expected function name");
        m_stackTrace.push_back(name);

        consume(TokenType::LeftParen, "expected '(' after function name");
        auto params = parseParameters();

        consume(TokenType::Arrow, "expected return type after function parameters");
        DeclaredType returnType = parseType();

        // A function has no body if:
        //   A. it ends with a semicolon, or
        //   B. it ends with eof, or
        //   C. it ends with a newline and is not followed by a '=' or '{'
        //
        // These are declarations:
        // fun x() -> void;
        // fun x() -> void<EOF>
        // fun x() -> void
        // class ...
        //
        // These are definitions:
        // fun x() -> int
        //     = 10
        // fun x() -> void
        // { }
        auto foundTerms = skipTerminators(true);
        bool hasNoBody = foundTerms.semicolon || foundTerms.eof ||
            (foundTerms.newline && (check(TokenType::Equal) || check(TokenType::LeftBrace)));

        std::unique_ptr<Statement> body;
        if (!hasNoBody) {
            if (match(TokenType::Equal)) {
                auto expr = parseExpression();
                body = std::make_unique<ExpressionStatement>(std::move(expr));
            } else if (match(TokenType::LeftBrace)) {
                auto block = parseBlock();
                body = std::move(block);
            } else {
                throw makeError("expected function body");
            }
        }

        m_stackTrace.pop_back();
        return std::make_unique<FunctionDeclaration>(
            modifiers, keyword, name, std::move(params), returnType, std::move(body));
    }

    std::vector<Token> Parser::parseModifiers() {
        std::vector<Token> result;
        while (true) {
            if (match(TokenType::Native)) {
                result.push_back(previous());
            } else {
                break;
            }
        }
        return result;
    }

    std::vector<Parameter> Parser::parseParameters() {
        std::vector<Parameter> result;
        if (!check(TokenType::RightParen)) {
            // parameter list is not empty.
            // accept the first parameter and then check for additional parameters.
            consume(TokenType::Identifier, "expected parameter name");
            Parameter firstParam = parseParameter();
            result.push_back(std::move(firstParam));

            while (match(TokenType::Comma)) {
                if (check(TokenType::RightParen)) {
                    // trailing comma, just break from the loop
                    break;
                } else {
                    // actual parameter
                    consume(TokenType::Identifier, "expected parameter name");
                    auto param = parseParameter();
                    result.push_back(std::move(param));
                }
            }
        }

        consume(TokenType::RightParen, "expected ')' after parameters");

        return result;
    }

    Parameter Parser::parseParameter() {
        const Token &name = previous();
        consume(TokenType::Colon, "expected ':' after parameter name");
        DeclaredType type = parseType();
        if (type.isSimple() && type.simple().name().lexeme == "Void") {
            throw makeError("cannot declare parameter of type void");
        }

        return {name, type};
    }

    DeclaredType Parser::parseType() {
        if (match(TokenType::Identifier)) {
            const std::string &lexeme = previous().lexeme;
            if (lexeme == "Int" || lexeme == "Double" || lexeme == "Void") {
                return DeclaredType(previous());
            } else {
                throw makeError("expected type name");
            }
        }
        throw makeError("expected type name");
    }

    StatementPtr Parser::parseStatement() {
        auto expr = parseExpression();
        return std::make_unique<ExpressionStatement>(std::move(expr));
    }

    StatementPtr Parser::parseBlock() {
        std::vector<StatementPtr> body;
        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            auto statement = parseStatement();
            body.push_back(std::move(statement));

            auto foundTerms = skipTerminators(true);
            if (!foundTerms.any()) break;
        }
        consume(TokenType::RightBrace, "expected '}' after block");

        return std::make_unique<BlockStatement>(std::move(body));
    }

    ExpressionPtr Parser::parseExpression() {
        //return parseDisjunction();
        return parseAdditive();
    }

    ExpressionPtr Parser::parseDisjunction() {
        auto left = parseConjunction();
        while (match(TokenType::OrOr)) {
            Token op = previous();
            auto right = parseConjunction();
            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExpressionPtr Parser::parseConjunction() {
        auto left = parseEquality();
        while (match(TokenType::AndAnd)) {
            Token op = previous();
            auto right = parseEquality();
            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExpressionPtr Parser::parseEquality() {
        auto left = parseComparison();
        while (match(TokenType::EqualEqual) || match(TokenType::BangEqual)) {
            Token op = previous();
            auto right = parseComparison();
            left = std::make_unique<ComparisonExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExpressionPtr Parser::parseComparison() {
        auto left = parseAdditive();
        while (
            match(TokenType::Greater) || match(TokenType::GreaterEqual) ||
            match(TokenType::Less) || match(TokenType::LessEqual))
        {
            Token op = previous();
            auto right = parseAdditive();
            left = std::make_unique<ComparisonExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExpressionPtr Parser::parseAdditive() {
        auto left = parseMultiplicative();
        while (match(TokenType::Plus) || match(TokenType::Minus)) {
            Token op = previous();
            auto right = parseMultiplicative();
            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExpressionPtr Parser::parseMultiplicative() {
        auto left = parseUnaryPrefix();
        while (match(TokenType::Asterisk) || match(TokenType::Slash) || match(TokenType::Percent)) {
            Token op = previous();
            auto right = parseUnaryPrefix();
            left = std::make_unique<BinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExpressionPtr Parser::parseUnaryPrefix() {
        std::vector<Token> operators;
        while (match(TokenType::Plus) || match(TokenType::Minus)) {
            operators.push_back(previous());
        }

        auto operand = parseUnaryPostfix();
        // apply unary operators in REVERSE order. closer to the expression => higher precedence
        for (auto iter = operators.rbegin(); iter != operators.rend(); iter++) {
            operand = std::make_unique<UnaryExpression>(std::move(*iter), std::move(operand), true);
        }
        return operand;
    }

    ExpressionPtr Parser::parseUnaryPostfix() {
        ExpressionPtr operand = parsePrimary();

        while (true) {
            if (match(TokenType::LeftParen)) {
                const Token &paren = previous();
                auto args = parseArguments();
                operand = std::make_unique<CallExpression>(paren, std::move(operand), std::move(args));
            } else {
                break;
            }
        }
        return operand;
    }

    std::vector<ExpressionPtr> Parser::parseArguments() {
        std::vector<ExpressionPtr> result;
        if (!check(TokenType::RightParen)) {
            // argument list is not empty.
            result.push_back(parseExpression());
            while (match(TokenType::Comma)) {
                if (check(TokenType::RightParen)) {
                    // trailing comma
                    break;
                } else {
                    result.push_back(parseExpression());
                }
            }
        }
        consume(TokenType::RightParen, "expected ')' after function arguments");
        return result;
    }

    ExpressionPtr Parser::parsePrimary() {
        if (match(TokenType::LeftParen)) {
            return parseParenthesizedExpr();
        } else if (match(TokenType::Identifier)) {
            return parseVariable();
        } else if (match(TokenType::FloatLiteral) || match(TokenType::IntegerLiteral)) {
            return parseNumber();
        } else {
            throw makeError("expected primary expression");
        }
    }

    ExpressionPtr Parser::parseParenthesizedExpr() {
        auto expr = parseExpression();
        consume(TokenType::RightParen, "expected ')' after parenthesized expression");
        return expr;
    }

    ExpressionPtr Parser::parseVariable() {
        return std::make_unique<VariableExpression>(previous());
    }

    ExpressionPtr Parser::parseNumber() {
        Token number = previous();
        bool isInteger = (number.type == TokenType::IntegerLiteral);
        return std::make_unique<NumberExpression>(std::move(number), isInteger);
    }

    void Parser::synchronize() noexcept {
        m_stackTrace.clear();
        advance();

        while (!isAtEnd()) {
            if (previous().type == TokenType::Semicolon) return;

            switch (current().type) {
            // Return on tokens that are likely to start a new line
            case TokenType::Native:
            case TokenType::Var:
            case TokenType::Fun:
            case TokenType::Return:
                return;
            // no possible statement ending tokens were found
            default:
                advance();
                break;
            }
        }
    }

    Parser::FoundTerminators Parser::skipTerminators(bool allowSemicolons) noexcept {
        FoundTerminators result;

        while (true) {
            if (current().type == TokenType::Newline) {
                result.newline = true;
            } else if (allowSemicolons && current().type == TokenType::Semicolon) {
                result.semicolon = true;
            } else {
                if (current().type == TokenType::EndOfFile) {
                    result.eof = true;
                }
                break;
            }
            advance();
        }
        return result;
    }

    const Token &Parser::consume(TokenType expected, const std::string &errMsg) {
        if (check(expected)) {
            return advance();
        } else {
            throw makeError(errMsg);
        }
    }

    bool Parser::match(TokenType expected) noexcept {
        if (check(expected)) {
            advance();
            return true;
        } else {
            return false;
        }
    }

    bool Parser::check(TokenType expected) noexcept {
        skipTerminators(false);
        return (current().type == expected);
    }

    const Token &Parser::advance() noexcept {
        const Token &retVal = current();
        if (!isAtEnd()) m_current++;
        return retVal;
    }

    const Token &Parser::current() const noexcept {
        return m_tokens[m_current];
    }

    const Token &Parser::previous() const noexcept {
        return m_tokens[m_current - 1];
    }

    bool Parser::isAtEnd() const noexcept {
        return current().type == TokenType::EndOfFile;
    }

    ParseException Parser::makeError(const std::string &expected) const {
        Error::ParseError error(current(), m_stackTrace, expected);
        if (!m_options->silentErrors()) {
            m_errorReporter->logError(error);
        }

        return ParseException(error);
    }

    ParseException::ParseException(const Error &cause) noexcept :
        std::runtime_error(cause.shortMessage()), m_cause(cause) {
    }

    const Error &ParseException::cause() const {
        return m_cause;
    }
}

