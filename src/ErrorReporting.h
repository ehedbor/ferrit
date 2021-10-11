#pragma once

#include "Error.h"

namespace ferrit {
    /**
     * Reports an error to the console.
     */
    void logError(const Error &error);

    /**
     * Reports a warning to the console.
     */
    void logWarning(const Error &warning);
}
