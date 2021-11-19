#pragma once

#include <iostream>
#include <string>


namespace ferrit {
    class NativeHandler {
    public:
        NativeHandler(std::ostream &output, std::ostream &errors, std::istream &input) noexcept;

        void println(const std::string &msg);
        void eprintln(const std::string &msg);
        std::string readln();

    private:
        std::ostream *m_output{};
        std::ostream *m_errors{};
        std::istream *m_input{};
    };
}
