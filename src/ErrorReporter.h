#pragma once

#include <memory>
#include <ostream>
#include "CompileOptions.h"
#include "Error.h"

namespace ferrit {
    /**
     * Utility class to output compiler errors to an <tt>std::ostream</tt>.
     */
    class ErrorReporter {
    public:
        ErrorReporter(std::shared_ptr<const CompileOptions> options, std::ostream &output);

        /**
         * Reports an error to the console.
         */
        void logError(const Error &err) const;

        /**
         * Reports a warning to the console.
         */
         void logWarning(const Error &warning) const;

    private:
        std::shared_ptr<const CompileOptions> m_options;
        std::ostream &m_output;
    };
}
