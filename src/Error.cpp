#include "Error.h"

#include <format>
#include <typeinfo>
#include <unordered_map>

namespace ferrit {
    Error::Error(Token cause, const std::string &what) noexcept :
        std::runtime_error(what), m_cause(std::move(cause)) {
    }

    const Token &Error::cause() const noexcept {
        return m_cause;
    }

    std::string Error::longMessage() const {
        std::string result{what()};
        result += "\n";
        result += std::format("    at {}", formatToken(cause()));
        return result;
    }

    std::string Error::formatToken(const Token &token) {
        return std::format("{}:{}: {} \"{}\"",
            token.location.line,
            token.location.column,
            token.type,
            token.lexeme);
    }

}