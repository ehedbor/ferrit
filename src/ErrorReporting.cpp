#include "ErrorReporting.h"
#include <iostream>
#include <termcolor/termcolor.hpp>

namespace ferrit {
    void logError(const Error &error) {
        std::cout << termcolor::colorize << termcolor::bright_red
            << "error: " << error.message()
            << termcolor::reset << std::endl;
    }

    void logWarning(const Error &warning) {
        std::cout << termcolor::colorize << termcolor::bright_yellow
            << "warning: " << warning.message()
            << termcolor::reset << std::endl;
    }
}