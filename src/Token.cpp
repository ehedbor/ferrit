#include "Token.h"

namespace ferrit {
    std::ostream &operator<<(std::ostream &output, TokenType tokenType) {
        output << std::format("{}", tokenType);
        return output;
    }
}
