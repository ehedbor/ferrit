#include "Error.h"
#include <utility>


namespace es {
    Error::Error(std::string msg) noexcept :
        m_msg(std::move(msg)) {
    }

    Error::~Error() noexcept = default;

    const std::string &Error::msg() const noexcept {
        return m_msg;
    }

    std::ostream &operator<<(std::ostream &out, const Error &error) {
        error.printTo(out);
        return out;
    }
}