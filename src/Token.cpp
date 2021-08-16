#include "Token.h"

SourceLocation::SourceLocation(int line, int column) :
    line(line), column(column) {
}

Token::Token(TokenType type, std::string lexeme, SourceLocation location) :
    type(type), lexeme(std::move(lexeme)), location(location) {
}
