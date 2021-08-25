#pragma once

#include <string>
#include <ostream>

namespace es {
    class Error {
    public:
        Error() noexcept = default;
        explicit Error(std::string msg) noexcept;
        virtual ~Error() noexcept = 0;

        [[nodiscard]] const std::string &msg() const noexcept;

        friend std::ostream &operator<<(std::ostream &out, const Error &error);

    protected:
        virtual void printTo(std::ostream &out) const = 0;

    private:
        std::string m_msg;
    };
}