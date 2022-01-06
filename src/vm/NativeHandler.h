#pragma once

#include <iostream>
#include <stdexcept>
#include <string>


namespace ferrit {
    struct ExecutionContext final {
        int line{};
    };

    class PanicError final : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    class NativeHandler final {
    public:
        explicit NativeHandler(std::ostream &output, std::ostream &errors, std::istream &input) noexcept;

        void panic(const ExecutionContext &ctx, const std::string &msg);

        void println(const ExecutionContext &ctx, const std::string &msg);
        void eprintln(const ExecutionContext &ctx, const std::string &msg);
        std::string readln(const ExecutionContext &ctx);

    private:
        std::ostream *m_output{};
        std::ostream *m_errors{};
        std::istream *m_input{};
    };
}
