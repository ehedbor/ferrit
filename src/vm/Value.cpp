#include "Value.h"

namespace ferrit {
    Value::Value(double real) noexcept :
        m_real(real) {
    }

    bool Value::isReal() const noexcept {
        return true;
    }

    double Value::asReal() const {
        return m_real;
    }

    bool operator==(Value left, Value right) {
        if (left.isReal() && right.isReal()) {
            return left.asReal() == right.asReal();
        } else {
            return false;
        }
    }

    std::ostream &operator<<(std::ostream &output, Value value) {
        try {
            output << std::format("{}", value);
        } catch (std::format_error &) {
            output.setstate(std::ios::failbit);
        }
        return output;
    }
}