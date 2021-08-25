#include "AstElements.h"


namespace es {
    Type::Type(Token name) noexcept :
        m_name(std::move(name)) {
    }

    const Token &Type::name() const noexcept {
        return m_name;
    }

    bool Type::operator==(const Type &other) const noexcept {
        if (this == &other) return true;
        return name() == other.name();
    }

    bool Type::operator!=(const Type &other) const noexcept {
        return !(*this == other);
    }


    Parameter::Parameter(Token name, Type type) noexcept :
        m_name(std::move(name)), m_type(std::move(type)) {
    }

    const Token &Parameter::name() const noexcept {
        return m_name;
    }

    const Type &Parameter::type() const noexcept {
        return m_type;
    }

    bool Parameter::operator==(const Parameter &other) const noexcept {
        if (this == &other) return true;
        return name() == other.name() && type() == other.type();
    }

    bool Parameter::operator!=(const Parameter &other) const noexcept {
        return !(*this == other);
    }
}
