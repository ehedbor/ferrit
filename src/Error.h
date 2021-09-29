#pragma once

#include <string>
#include <format>
#include <ostream>
#include <vector>

#include "Token.h"

namespace ferrit {
    enum class ErrorCode {
        Unknown,
        SyntaxUnexpectedChar,
        SyntaxUnterminatedElement,
        SyntaxEmptyElement,
        SyntaxCharLiteralTooBig,
        SyntaxUnexpectedNewlineInElement,
        SyntaxIllegalEscapeSequence,
        SyntaxUnknownLiteralSuffix,
        SyntaxParseError,
    };

    /**
     * Represents any error-type in this program.
     */
    class Error final {
    public:
        Error() noexcept = default;

        template<typename... Args>
        Error(const Token &cause, ErrorCode errorCode, const Args &... args);

        [[nodiscard]] ErrorCode errorCode() const noexcept;

        [[nodiscard]] std::string msg() const noexcept;

        [[nodiscard]] const std::vector<Token> &stackTrace() const noexcept;

        void addStackTrace(const Token &location) noexcept;

        friend std::ostream &operator<<(std::ostream &out, const Error &error);

    private:
        [[nodiscard]] std::string formatString(int argc) const;

    private:
        ErrorCode m_errorCode{ErrorCode::Unknown};
        std::string m_msg{"unknown"};
        std::vector<Token> m_stackTrace{};
    };


    template<typename... Args>
    Error::Error(const Token &cause, ErrorCode errorCode, const Args &... args) :
        m_errorCode(errorCode),
        m_msg(std::format(formatString(sizeof...(args)), args...)) {
        addStackTrace(cause);
    }
}