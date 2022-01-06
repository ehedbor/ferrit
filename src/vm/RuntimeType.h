#pragma once

#include <string>

class RuntimeType final {
public:
    explicit RuntimeType(std::string name) noexcept;

    bool operator==(const RuntimeType& other) const = default;

    [[nodiscard]] const std::string &name() const noexcept;

public:
    static const RuntimeType NullType;
    static const RuntimeType BoolType;
    static const RuntimeType IntType;
    static const RuntimeType RealType;

private:
    std::string m_name;
};


