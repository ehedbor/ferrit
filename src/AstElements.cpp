#include "AstElements.h"

Type::Type(Token name) :
    m_name(std::move(name)) {

}

const Token &Type::name() const {
    return m_name;
}

Parameter::Parameter(Token name, Type type) :
    m_name(std::move(name)), m_type(std::move(type)) {

}

const Token &Parameter::name() const {
    return m_name;
}

const Type &Parameter::type() const {
    return m_type;
}
