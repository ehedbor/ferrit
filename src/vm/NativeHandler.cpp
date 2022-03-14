#include "NativeHandler.h"


namespace ferrit {
    NativeHandler::NativeHandler(std::ostream &output, std::ostream &errors, std::istream &input) noexcept :
        m_output{&output}, m_errors{&errors}, m_input{&input} {
    }

    void NativeHandler::panic(const ExecutionContext &ctx, const std::string &msg) {
        eprintln(ctx, msg);
        throw PanicError(msg);
    }

    void NativeHandler::println(const ExecutionContext &ctx, const std::string &msg) {
        *m_output << msg << std::endl;
        if (!(*m_errors)) {
            panic(ctx, "could not write to standard output");
        }
    }

    void NativeHandler::eprintln(const ExecutionContext &ctx, const std::string &msg) {
        *m_errors << msg << std::endl;
        if (!(*m_errors)) {
            panic(ctx, "could not write to standard error");
        }
    }

    std::string NativeHandler::readln(const ExecutionContext &ctx) {
        std::string line;
        if (!std::getline(*m_input, line)) {
            panic(ctx, "could not read from standard input");
        }
        return line;
    }
}