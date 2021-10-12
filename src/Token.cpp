#include "Token.h"

namespace ferrit {
    std::ostream &operator<<(std::ostream &output, TokenType tokenType) {
        output << std::format("{}", tokenType);
        return output;
    }

    SourceLocation::SourceLocation(int line, int column) noexcept :
        line(line), column(column) {
    }

    Token::Token(TokenType tokenType, std::string lexeme, SourceLocation location) noexcept :
        type(tokenType), lexeme(std::move(lexeme)), location(location) {
    }
}
