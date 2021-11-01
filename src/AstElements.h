#pragma once

#include "Token.h"
#include <format>
#include <variant>
#include <vector>

namespace ferrit {
    class DeclaredType;

    class SimpleType final {
    public:
        explicit SimpleType(Token name) noexcept;

        [[nodiscard]] const Token &name() const noexcept;

    private:
        Token m_name;
    };

    class FunctionType final {
    public:
        FunctionType(
            Token paren,
            std::vector<DeclaredType> parameters,
            std::unique_ptr<DeclaredType> returnType) noexcept;
        ~FunctionType() noexcept = default;

        FunctionType(const FunctionType &other) noexcept;
        FunctionType &operator=(const FunctionType &other) noexcept;
        FunctionType(FunctionType &&other) noexcept = default;
        FunctionType &operator=(FunctionType &&other) noexcept = default;

    public:
        [[nodiscard]] const Token &paren() const noexcept;
        [[nodiscard]] const std::vector<DeclaredType> &parameters() const noexcept;
        [[nodiscard]] const DeclaredType &returnType() const noexcept;

    private:
        Token m_paren;
        std::vector<DeclaredType> m_parameters;
        std::unique_ptr<DeclaredType> m_returnType;
    };

    /**
     * Represents a type as defined by the user (as opposed to an <tt>llvm::Type</tt> for instance).
     */
    class DeclaredType final {
    public:
        DeclaredType() noexcept;

        explicit DeclaredType(Token name) noexcept;

        explicit DeclaredType(
            Token paren,
            std::vector<DeclaredType> parameters,
            DeclaredType returnType) noexcept;

        [[nodiscard]] bool isSimple() const noexcept;
        [[nodiscard]] bool isFunction() const noexcept;

        [[nodiscard]] const SimpleType &simple() const;
        [[nodiscard]] const FunctionType &function() const;

        /**
         * Returns a token to be used for error reporting.
         */
        [[nodiscard]] const Token &errorToken() const noexcept;

        [[nodiscard]] bool operator==(const DeclaredType &other) const noexcept;
        [[nodiscard]] std::string toString() const noexcept;

    private:
        std::variant<SimpleType, FunctionType> m_data;
    };

    /**
     * Represents a parameter in a function prototype.
     * Currently, parameters only have a \c name and a \c type,
     * though in the future default values may also be supported.
     */
    class Parameter final {
    public:
        Parameter(Token name, DeclaredType type) noexcept;

        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const DeclaredType &type() const noexcept;

        bool operator==(const Parameter &other) const noexcept;

    private:
        Token m_name;
        DeclaredType m_type;
    };
}


namespace std {
    template <>
    struct formatter<ferrit::DeclaredType> : formatter<string> {
        auto format(const ferrit::DeclaredType &declaredType, format_context &ctx) {
            return formatter<string>::format(declaredType.toString(), ctx);
        }
    };
}
