#include "Error.h"
#include <iomanip>

namespace ferrit {
    const std::vector<Token> &Error::stackTrace() const noexcept {
        return m_stackTrace;
    }

    ErrorCode Error::errorCode() const noexcept {
        return m_errorCode;
    }

    std::string Error::msg() const noexcept {
        return m_msg;
    }

    void Error::addStackTrace(const Token &location) noexcept {
        m_stackTrace.push_back(location);
    }

    std::string Error::formatString(int argc) const {
        std::string fmtString;
        switch (m_errorCode) {
        case ErrorCode::Unknown:
            return "error error: unknown error";
        case ErrorCode::SyntaxUnexpectedChar:
            return "syntax error: unexpected symbol '{}'";
        case ErrorCode::SyntaxUnterminatedElement:
            return "syntax error: unterminated {}";
        case ErrorCode::SyntaxEmptyElement:
            return "syntax error: empty {}";
        case ErrorCode::SyntaxCharLiteralTooBig:
            return "syntax error: too many characters in char literal";
        case ErrorCode::SyntaxUnexpectedNewlineInElement:
            return "syntax error: unexpected newline in {}";
        case ErrorCode::SyntaxIllegalEscapeSequence:
            return "syntax error: illegal escape sequence '\\{}' in {}";
        case ErrorCode::SyntaxUnknownLiteralSuffix:
            return "syntax error: unknown literal suffix '{}'";
        case ErrorCode::SyntaxParseError:
            if (argc <= 1)
                return "syntax error: {}";
            else
                return "syntax error: {} after {}";
        }
        throw std::invalid_argument(std::format("Unknown enum case '{}'", static_cast<int>(m_errorCode)));
    }

    std::ostream &operator<<(std::ostream &out, const Error &error) {
        out << "Error E"
            << std::setfill('0') << std::setw(3) << static_cast<int>(error.errorCode())
            << ": " << error.msg() << "\n";

        for (const auto &entry : error.stackTrace()) {
            out << "    at " << entry.location << ": ";
            if (entry.type != TokenType::Unknown) {
                out << entry.type << " ";
            }
            out << "\"" << entry.lexeme << "\"" << "\n";
        }
        return out;
    }
}