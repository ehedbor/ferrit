#pragma once

#include "Token.h"


namespace es {
    class Type {
    public:
        explicit Type(Token typeName) noexcept;

        [[nodiscard]] const Token &name() const noexcept;

    private:
        Token m_name;
    };

    class Parameter {
    public:
        Parameter(Token name, Type type) noexcept;

        [[nodiscard]] const Token &name() const noexcept;
        [[nodiscard]] const Type &type() const noexcept;

    private:
        Token m_name;
        Type m_type;
    };
}
