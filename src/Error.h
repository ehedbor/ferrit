#pragma once

#include "Token.h"

#include <concepts>
#include <string>
#include <stdexcept>
#include <vector>


namespace ferrit {

    /**
     * Class to represent compile-time errors in the user's program.
     * This includes syntax errors, parsing errors, and compile errors.
     *
     * Runtime errors are not represented with this class.
     */
    class Error : public std::runtime_error {
    protected:
        /**
         * Construct a new Error.
         *
         * @param cause the location of the error
         * @param what a formatted message containing information about the error
         */
        Error(Token cause, const std::string &what) noexcept;

    public:
        /**
         * Returns the token at which this error occurred.
         */
        [[nodiscard]] const Token &cause() const noexcept;

        /**
         * Returns a message describing this error.
         *
         * This includes the content of the short message plus additional information.
         */
        [[nodiscard]] std::string longMessage() const;

    private:
        static std::string formatToken(const Token &token);

    private:
        Token m_cause;
    };

/**
 * Generates a static function to return a user-friendly name for this error.
 */
#define FERRIT_ERROR_PRETTY_NAME(name)                                         \
    [[nodiscard]] static constexpr const char *prettyName() noexcept {         \
        return name;                                                           \
    } static_assert(true)
}
