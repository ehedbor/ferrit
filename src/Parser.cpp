#include "Parser.h"
#include "ResultTry.h"


namespace ferrit {
    Parser::Parser(std::vector<Token> tokens) noexcept : m_tokens(std::move(tokens)) {
    }

    ParseResult Parser::parse() {
        std::vector<StatementPtr> program;
        std::vector<Error> errors;

        skipTerminators(true);
        while (!isAtEnd()) {
            auto nextDecl = parseDeclaration();
            if (nextDecl) {
                program.push_back(std::move(nextDecl.value()));
                skipTerminators(true);
            } else {
                errors.push_back(std::move(nextDecl.error()));
                synchronize();
            }
        }
        if (!errors.empty()) {
            return cpp::fail(errors);
        } else {
            return program;
        }
    }

    StmtResult Parser::parseDeclaration() {
        auto mods = parseModifiers();
        if (match(TokenType::Fun)) {
            return parseFunctionDeclaration(mods);
        }
        return cpp::fail(makeError("expected declaration"));
    }

    StmtResult Parser::parseFunctionDeclaration(const std::vector<Token> &modifiers) {
        // remember the keyword
        Token keyword = previous();

        TRY(name, consume(TokenType::Identifier, "expected function name"));

        EXPECT(consume(TokenType::LeftParen, "expected '(' after function name"));
        TRY(params, parseParameters());

        EXPECT(consume(TokenType::Arrow, "expected return type after function parameters"));
        TRY(returnType, parseType());

        auto decl = std::make_unique<FunctionDeclaration>(modifiers, keyword, name, std::move(params), returnType);

        std::unique_ptr<Statement> body;

        // A function is a declaration if:
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
        bool isDecl = foundTerms.semicolon || foundTerms.eof ||
            (foundTerms.newline && (check(TokenType::Equal) || check(TokenType::LeftBrace)));
        if (isDecl) {
            return decl;
        } else {
            if (match(TokenType::Equal)) {
                TRY(expr, parseExpression());
                body = std::make_unique<ExpressionStatement>(std::move(expr));
            } else if (match(TokenType::LeftBrace)) {
                TRY(block, parseBlock());
                body = std::move(block);
            } else {
                return cpp::fail(makeError("expected function body"));
            }
            return std::make_unique<FunctionDefinition>(std::move(decl), std::move(body));
        }
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

    cpp::result<std::vector<Parameter>, Error> Parser::parseParameters() {
        std::vector<Parameter> result;
        if (!check(TokenType::RightParen)) {
            // parameter list is not empty.
            // accept the first parameter and then check for additional parameters.
            EXPECT(consume(TokenType::Identifier, "expected parameter name"));
            TRY(firstParam, parseParameter());
            result.push_back(std::move(firstParam));

            while (match(TokenType::Comma)) {
                if (check(TokenType::RightParen)) {
                    // trailing comma, just break from the loop
                    break;
                } else {
                    // actual parameter
                    EXPECT(consume(TokenType::Identifier, "expected parameter name"));
                    TRY(param, parseParameter());
                    result.push_back(std::move(param));
                }
            }
        }

        EXPECT(consume(TokenType::RightParen, "expected ')' after parameters"));

        return result;
    }

    cpp::result<Parameter, Error> Parser::parseParameter() {
        auto name = previous();
        EXPECT(consume(TokenType::Colon, "expected ':' after parameter name"));
        TRY(type, parseType());
        if (type.name().type == TokenType::Identifier && type.name().lexeme == "Void") {
            return cpp::fail(makeError("cannot declare parameter of type void"));
        }

        return Parameter(name, type);
    }

    cpp::result<Type, Error> Parser::parseType() {
        if (match(TokenType::Identifier)) {
            const std::string &lexeme = previous().lexeme;
            if (lexeme == "Int" || lexeme == "Double" || lexeme == "Void") {
                return Type(previous());
            } else {
                return cpp::fail(makeError("expected type name"));
            }
        }
        return cpp::fail(makeError("expected type name"));
    }

    StmtResult Parser::parseStatement() {
        TRY(expr, parseExpression());
        return std::make_unique<ExpressionStatement>(std::move(expr));
    }

    StmtResult Parser::parseBlock() {
        std::vector<StatementPtr> body;
        while (!check(TokenType::RightBrace) && !isAtEnd()) {
            TRY(statement, parseStatement());
            body.push_back(std::move(statement));

            auto foundTerms = skipTerminators(true);
            if (!foundTerms.any()) break;
        }
        EXPECT(consume(TokenType::RightBrace, "expected '}' after block"));

        return std::make_unique<Block>(std::move(body));
    }

    ExprResult Parser::parseExpression() {
        return parseDisjunction();
    }

    ExprResult Parser::parseDisjunction() {
        TRY(left, parseConjunction());
        while (match(TokenType::OrOr)) {
            Token op = previous();
            TRY(right, parseConjunction());
            left = std::make_unique<SimpleBinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExprResult Parser::parseConjunction() {
        TRY(left, parseEquality());
        while (match(TokenType::AndAnd)) {
            Token op = previous();
            TRY(right, parseEquality());
            left = std::make_unique<SimpleBinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExprResult Parser::parseEquality() {
        TRY(left, parseComparison());
        while (match(TokenType::EqualEqual) || match(TokenType::BangEqual)) {
            Token op = previous();
            TRY(right, parseComparison());
            left = std::make_unique<ComparisonExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExprResult Parser::parseComparison() {
        TRY(left, parseAdditive());
        while (
            match(TokenType::Greater) || match(TokenType::GreaterEqual) ||
            match(TokenType::Less) || match(TokenType::LessEqual))
        {
            Token op = previous();
            TRY(right, parseAdditive());
            left = std::make_unique<ComparisonExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExprResult Parser::parseAdditive() {
        TRY(left, parseMultiplicative());
        while (match(TokenType::Plus) || match(TokenType::Minus)) {
            Token op = previous();
            TRY(right, parseMultiplicative());
            left = std::make_unique<SimpleBinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExprResult Parser::parseMultiplicative() {
        TRY(left, parseUnary());
        while (match(TokenType::Asterisk) || match(TokenType::Slash) || match(TokenType::Percent)) {
            Token op = previous();
            TRY(right, parseUnary());
            left = std::make_unique<SimpleBinaryExpression>(op, std::move(left), std::move(right));
        }
        return left;
    }

    ExprResult Parser::parseUnary() {
        std::vector<Token> operators;
        while (match(TokenType::Plus) || match(TokenType::Minus) || match(TokenType::Bang)) {
            operators.push_back(previous());
        }

        TRY(operand, parsePrimary());
        // apply unary operators in REVERSE order. closer to the expression => higher precedence
        for (auto iter = operators.rbegin(); iter != operators.rend(); iter++) {
            operand = std::make_unique<UnaryExpression>(std::move(*iter), std::move(operand));
        }
        return operand;
    }

    ExprResult Parser::parsePrimary() {
        if (match(TokenType::Identifier)) {
            return parseVariable();
        } else if (match(TokenType::FloatLiteral) || match(TokenType::IntegerLiteral)) {
            return parseNumber();
        } else {
            return cpp::fail(makeError("expected primary expression"));
        }
    }

    ExprResult Parser::parseVariable() {
        return std::make_unique<VariableExpression>(previous());
    }

    ExprResult Parser::parseNumber() {
        Token number = previous();
        bool isInteger = (number.type == TokenType::IntegerLiteral);
        return std::make_unique<NumberExpression>(std::move(number), isInteger);
    }

    void Parser::synchronize() {
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

    Parser::FoundTerminators Parser::skipTerminators(bool allowSemicolons) {
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

    TokenResult Parser::consume(TokenType expected, const std::string &errMsg) {
        if (check(expected)) {
            return advance();
        } else {
            return cpp::fail(makeError(errMsg));
        }
    }

    bool Parser::match(TokenType expected) {
        if (check(expected)) {
            advance();
            return true;
        } else {
            return false;
        }
    }

    bool Parser::check(TokenType expected) {
        skipTerminators(false);
        return (current().type == expected);
    }

    const Token &Parser::advance() {
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
}
