#pragma once

#include "Error.h"

#include <memory>
#include <ostream>


namespace ferrit {
    /**
     * Utility class to output compiler errors to an <tt>std::ostream</tt>.
     */
    class ErrorReporter {
    public:
        /**
         * Construct an ErrorReporter.
         *
         * @param output the ostream to output to.
         * @param plainOutput true to not use colors.
         */
        ErrorReporter(std::ostream &output, bool plainOutput);

        /**
         * Reports an error to the ostream.
         */
        void logError(const Error &err) const;

        /**
         * Reports a warning to the ostream.
         */
         void logWarning(const Error &warning) const;

    private:
        std::ostream &m_output;
        bool m_plainOutput;
    };
}
