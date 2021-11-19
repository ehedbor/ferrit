#include "NativeHandler.h"


namespace ferrit {
    NativeHandler::NativeHandler(std::ostream &output, std::ostream &errors, std::istream &input) noexcept :
        m_output{&output}, m_errors{&errors}, m_input{&input} {
    }

    void NativeHandler::println(const std::string &msg) {
        *m_output << msg << std::endl;
    }

    void NativeHandler::eprintln(const std::string &msg) {
        *m_errors << msg << std::endl;
    }

    std::string NativeHandler::readln() {
        std::string line;
        std::getline(*m_input, line);
        return line;
    }
}