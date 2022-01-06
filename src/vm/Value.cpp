#include "Value.h"

#include <stdexcept>

namespace ferrit {
    Value::Value() noexcept : m_data{nullptr} {
    }

    Value::Value(bool boolean) noexcept : m_data{boolean} {
    }

    Value::Value(std::int64_t integer) noexcept : m_data{integer} {
    }

    Value::Value(double real) noexcept : m_data{real} {
    }

    bool Value::isNull() const noexcept {
        return std::holds_alternative<std::nullptr_t>(m_data);
    }

    bool Value::isBoolean() const noexcept {
        return std::holds_alternative<bool>(m_data);
    }

    bool Value::isInteger() const noexcept {
        return std::holds_alternative<std::int64_t>(m_data);
    }

    bool Value::isReal() const noexcept {
        return std::holds_alternative<double>(m_data);
    }

    bool Value::asBoolean() const {
        try {
            return std::get<bool>(m_data);
        } catch (const std::bad_variant_access &e) {
            throw std::logic_error(std::format("value not a boolean: {}", e.what()));
        }
    }

    std::int64_t Value::asInteger() const {
        try {
            return std::get<std::int64_t>(m_data);
        } catch (const std::bad_variant_access &e) {
            throw std::logic_error(std::format("value not an integer: {}", e.what()));
        }
    }

    double Value::asReal() const {
        try {
            return std::get<double>(m_data);
        } catch (const std::bad_variant_access &e) {
            throw std::logic_error(std::format("value not a real: {}", e.what()));
        }
    }

    bool operator==(Value left, Value right) {
        if (left.isNull() && right.isNull()) {
            return true;
        } else if (left.isBoolean() && right.isBoolean()) {
            return left.asBoolean() == right.asBoolean();
        } else if (left.isInteger() && right.isInteger()) {
            return left.asInteger() == right.asInteger();
        } else if (left.isReal() && right.isReal()) {
            return left.asReal() == right.asReal();
        } else {
            return false;
        }
    }

    RuntimeType Value::runtimeType() const {
        if (isNull()) {
            return RuntimeType::NullType;
        } else if (isBoolean()) {
            return RuntimeType::BoolType;
        } else if (isInteger()) {
            return RuntimeType::IntType;
        } else if (isReal()) {
            return RuntimeType::RealType;
        } else {
            throw std::logic_error("unknown value variant");
        }
    }

    std::ostream &operator<<(std::ostream &output, const Value &value) {
        try {
            output << std::format("{}", value);
        } catch (const std::format_error &) {
            output.setstate(std::ios::failbit);
        }
        return output;
    }
}