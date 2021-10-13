#include "ErrorReporter.h"
#include <termcolor/termcolor.hpp>

namespace ferrit {
    ErrorReporter::ErrorReporter(std::shared_ptr<const CompileOptions> options, std::ostream &output) :
        m_options(std::move(options)), m_output(output) {
    }

    void ErrorReporter::logError(const Error &err) const {
        if (!m_options->plainOutput()) {
            m_output << termcolor::colorize << termcolor::bright_red;
        }
        m_output << "error: " << err.message() << std::endl;

        if (!m_options->plainOutput()) {
            m_output << termcolor::reset;
        }
    }

    void ErrorReporter::logWarning(const Error &warning) const {
        if (!m_options->plainOutput()) {
            m_output << termcolor::colorize << termcolor::bright_yellow;
        }
        m_output << "warning: " << warning.message() << std::endl;
        if (!m_options->plainOutput()) {
            m_output << termcolor::reset;
        }
    }
}