#include "ErrorReporter.h"

#include <termcolor/termcolor.hpp>


namespace ferrit {
    ErrorReporter::ErrorReporter(std::ostream &output, bool plainOutput) :
        m_output(output), m_plainOutput(plainOutput) {
    }

    void ErrorReporter::logError(const Error &err) const {
        if (!m_plainOutput) {
            m_output << termcolor::colorize << termcolor::bright_red;
        }
        m_output << "error: " << err.longMessage() << std::endl;

        if (!m_plainOutput) {
            m_output << termcolor::reset;
        }
    }

    void ErrorReporter::logWarning(const Error &warning) const {
        if (!m_plainOutput) {
            m_output << termcolor::colorize << termcolor::bright_yellow;
        }
        m_output << "warning: " << warning.longMessage() << std::endl;
        if (!m_plainOutput) {
            m_output << termcolor::reset;
        }
    }
}