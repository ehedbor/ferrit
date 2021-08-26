#pragma once

#include "Token.h"


namespace es {
    /**
     * Represents a type that a value can have, such as @c int or <tt>Comparable\<double\></tt>.
     */
    class Type {
    public:
        explicit Type(Token typeName) noexcept;

        [[nodiscard]] const Token &name() const noexcept;

        bool operator==(const Type &other) const noexcept;
        bool operator!=(const Type &other) const noexcept;

    private:
        Token m_name;
    };

    /**
     * Represents a parameter in a function prototype.
     * Currently, parameters only have a \c name and a \c type,
     * though in the future default values may also be supported.
     */
    class Parameter {
    public:
        Parameter(Token name, Type type) noexcept;

        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const Type &type() const noexcept;

        bool operator==(const Parameter &other) const noexcept;
        bool operator!=(const Parameter &other) const noexcept;

    private:
        Token m_name;
        Type m_type;
    };
}
