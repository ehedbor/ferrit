#pragma once

#include <string>
#include <ostream>
#include <vector>


namespace ferrit {
    struct Token;

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
        Error(const Token &cause, ErrorCode errorCode, std::vector<std::string> fmtArgs = {}) noexcept;

        [[nodiscard]] ErrorCode errorCode() const noexcept;
        [[nodiscard]] std::string msg() const;

        [[nodiscard]] const std::vector<Token> &stackTrace() const noexcept;
        void addStackTrace(const Token& location) noexcept;

        friend std::ostream &operator<<(std::ostream &out, const Error &error);

    private:
        ErrorCode m_errorCode{ErrorCode::Unknown};
        std::vector<std::string> m_fmtArgs{};
        std::vector<Token> m_stackTrace{};
    };

}