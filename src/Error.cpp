#include "Error.h"
#include <utility>


namespace ferrit {
    Error::Error(std::string msg) noexcept :
        m_msg(std::move(msg)) {
    }

    const std::string &Error::msg() const noexcept {
        return m_msg;
    }

    std::ostream &operator<<(std::ostream &out, const Error &error) {
        error.printTo(out);
        return out;
    }
}