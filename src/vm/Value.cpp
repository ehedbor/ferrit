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

    std::ostream &operator<<(std::ostream &output, Value value) {
        output << std::format("{}", value);
        return output;
    }
}