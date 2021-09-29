#include "Error.h"
#include "Token.h"


namespace ferrit {
    Error::Error(const Token &cause, ErrorCode errorCode, std::vector<std::string> fmtArgs) noexcept :
        m_errorCode(errorCode), m_fmtArgs(std::move(fmtArgs)) {
        addStackTrace(cause);
    }

    const std::vector<Token> &Error::stackTrace() const noexcept {
        return m_stackTrace;
    }

    ErrorCode Error::errorCode() const noexcept {
        return m_errorCode;
    }

    std::string Error::msg() const {
        std::string fmtString;
        switch (m_errorCode) {
        case ErrorCode::SyntaxUnexpectedChar:
            fmtString = "syntax error: unexpected symbol '{}'";
            break;
        case ErrorCode::SyntaxUnterminatedElement:
            fmtString = "syntax error: unterminated {}";
            break;
        case ErrorCode::SyntaxEmptyElement:
            fmtString = "syntax error: empty {}";
            break;
        case ErrorCode::SyntaxCharLiteralTooBig:
            fmtString = "syntax error: too many characters in char literal";
            break;
        case ErrorCode::SyntaxUnexpectedNewlineInElement:
            fmtString = "syntax error: unexpected newline in {}";
            break;
        case ErrorCode::SyntaxIllegalEscapeSequence:
            fmtString = "syntax error: illegal escape sequence '\\{}' in {}";
            break;
        case ErrorCode::SyntaxUnknownLiteralSuffix:
            fmtString = "syntax error: unknown literal suffix '{}'";
            break;
        case ErrorCode::SyntaxParseError:
            if (m_fmtArgs.size() <= 1)
                fmtString = "syntax error: expected {}";
            else
                fmtString = "syntax error: expected {} after {}";
            break;
        }
        // TODO: use std::format
        return fmtString;
    }


    void Error::addStackTrace(const Token &location) noexcept {
        m_stackTrace.push_back(location);
    }

    std::ostream &operator<<(std::ostream &out, const Error &error) {
        out << "Error E" << static_cast<int>(error.errorCode()) << ": " << error.msg() << "\n";
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