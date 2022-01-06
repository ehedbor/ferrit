#include "RuntimeType.h"

RuntimeType::RuntimeType(std::string name) noexcept :
    m_name{std::move(name)} {
}

const std::string &RuntimeType::name() const noexcept {
    return m_name;
}

const RuntimeType RuntimeType::NullType{"ferrit.Null"};
const RuntimeType RuntimeType::BoolType{"ferrit.Bool"};
const RuntimeType RuntimeType::IntType{"ferrit.Int"};
const RuntimeType RuntimeType::RealType{"ferrit.Real"};