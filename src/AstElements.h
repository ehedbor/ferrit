#pragma once

#include "Token.h"


class Type {
public:
    explicit Type(Token typeName);

    [[nodiscard]] const Token &name() const;

private:
    Token m_name;
};

class Parameter {
public:
    Parameter(Token name, Type type);

    [[nodiscard]] const Token &name() const;
    [[nodiscard]] const Type &type() const;

private:
    Token m_name;
    Type m_type;
};