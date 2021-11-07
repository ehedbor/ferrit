#pragma once

#include "Token.h"

#include <ostream>
#include <format>
#include <variant>
#include <vector>

namespace ferrit {
    class DeclaredType;

    /**
     * Represents a simple type consisting of a single identifier.
     */
    class SimpleType final {
    public:
        /**
         * Constructs a new simple type.
         *
         * @note This constructor is an implementation detail of \c DeclaredType
         * and should not be directly called.
         *
         * @param name the type token
         */
        explicit SimpleType(Token name) noexcept;

        /**
         * Returns the token representing this type.
         */
        [[nodiscard]] const Token &name() const noexcept;

    private:
        Token m_name;
    };

    /**
     * Represents the type of a function, consisting of its parameters' types and its return type.
     */
    class FunctionType final {
    public:
        /**
         * Constructs a new function type.
         *
         * @note This constructor is an implementation detail of \c DeclaredType
         * and should not be directly constructed.
         *
         * @param errorToken a token to be used for error reporting
         * @param parameters the function's parameters
         * @param returnType the function's return type
         */
        FunctionType(
            Token errorToken,
            std::vector<DeclaredType> parameters,
            std::unique_ptr<DeclaredType> returnType) noexcept;

        ~FunctionType() noexcept = default;

        FunctionType(const FunctionType &other) noexcept;
        FunctionType &operator=(const FunctionType &other) noexcept;

        FunctionType(FunctionType &&other) noexcept = default;
        FunctionType &operator=(FunctionType &&other) noexcept = default;

    public:
        /**
         * Returns a token to be used for error reporting.
         */
        [[nodiscard]] const Token &errorToken() const noexcept;

        /**
         * Returns the function's parameter types.
         */
        [[nodiscard]] const std::vector<DeclaredType> &parameters() const noexcept;

        /**
         * Returns the function's return type.
         */
        [[nodiscard]] const DeclaredType &returnType() const noexcept;

    private:
        Token m_errorToken;
        std::vector<DeclaredType> m_parameters;
        std::unique_ptr<DeclaredType> m_returnType;
    };

    /**
     * Represents a type as defined by the user (as opposed to an <tt>llvm::Type</tt> for instance).
     */
    class DeclaredType final {
    public:
        /**
         * Construct a simple type.
         *
         * @param name the type name
         */
        explicit DeclaredType(Token name) noexcept;

        /**
         * Construct a function type.
         *
         * @param errorToken a token to be used for error reporting.
         *     For a function declaration, this should be the fun keyword.
         * @param parameters the type of parameters passed to the function
         * @param returnType the function's return type
         */
        DeclaredType(
            Token errorToken,
            std::vector<DeclaredType> parameters,
            DeclaredType returnType) noexcept;

        /**
         * Checks if the type refers to a simple unannotated type (e.g. Int, String, MyCustomClass).
         */
        [[nodiscard]] bool isSimple() const noexcept;

        /**
         * Checks if the type refers to a function.
         */
        [[nodiscard]] bool isFunction() const noexcept;

        /**
         * Returns the type as a SimpleType.
         */
        [[nodiscard]] const SimpleType &simple() const;

        /**
         * Returns the type as a FunctionType.
         */
        [[nodiscard]] const FunctionType &function() const;

        /**
         * Returns a token to be used for error reporting.
         */
        [[nodiscard]] const Token &errorToken() const noexcept;

        /**
         * Compare this type to another type.
         */
        [[nodiscard]] bool operator==(const DeclaredType &other) const noexcept;

    private:
        std::variant<SimpleType, FunctionType> m_data;
    };

    /**
     * Outputs a user-friendly representation of this DeclaredType.
     *
     * If the type of the declaredType is unknown, the ostream's fail bit will be set.
     */
    std::ostream &operator<<(std::ostream &output, const DeclaredType &declaredType);

    /**
     * Represents a parameter in a function prototype.
     *
     * Currently, parameters only have a \c name and a \c type,
     * though in the future default values may also be supported.
     */
    class Parameter final {
    public:
        /**
         * Construct a parameter.
         *
         * @param name the name of the parameter
         * @param type the parameter's type
         */
        Parameter(Token name, DeclaredType type) noexcept;

        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const DeclaredType &type() const noexcept;

        bool operator==(const Parameter &other) const noexcept;

    private:
        Token m_name;
        DeclaredType m_type;
    };
}

template <>
struct std::formatter<ferrit::DeclaredType> : std::formatter<std::string> {
    auto format(const ferrit::DeclaredType &declaredType, std::format_context &ctx) {
        if (declaredType.isSimple()) {
            return super::format(declaredType.simple().name().lexeme, ctx);
        } else if (declaredType.isFunction()) {
            const auto &func = declaredType.function();
            std::string result{};

            result += "(";
            for (int i = 0; i < func.parameters().size(); i++) {
                const auto &param = func.parameters()[i];
                result += std::format("{}", param);
                if (i < func.parameters().size() - 1) {
                    result += ", ";
                }
            }
            result += std::format(") -> {}", func.returnType());

            return super::format(result, ctx);
        } else {
            throw std::format_error("unknown DeclaredType variant");
        }
    }

private:
    using super = typename std::formatter<std::string>;
};
