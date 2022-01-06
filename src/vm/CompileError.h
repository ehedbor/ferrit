#pragma once

#include "../Error.h"

#include <vector>

namespace ferrit {
    /**
     * Thrown when the compiler encounters a broken invariant that should never occur.
     *
     * @example When compiling a binary expression, the parser should ensure that
     * the operator is one of a select few token types. If an unknown token type
     * is encountered, it is due to a bug in the parser, not the user's code,
     * and should result in a fatal error.
     */
    class CompileException final : public std::logic_error {
    public:
        using std::logic_error::logic_error;
    };

    /**
     * Thrown when a compile-time error is present in the user's program.
     */
    class CompileError : public Error {
    public:
        class NotImplemented;
        class LiteralOutOfRange;
        class IncompatibleTypes;

    protected:
        using Error::Error;
    };
    
    /**
     * Indicates that a given feature is not implemented.
     */
    class CompileError::NotImplemented final : public CompileError {
    public:
        explicit NotImplemented(Token cause, const std::string &feature);
        FERRIT_ERROR_PRETTY_NAME("not-implemented");
    };
    
    /**
     * Indicates that the given literal is not representable. 
     */
    class CompileError::LiteralOutOfRange final : public CompileError {
    public:
        explicit LiteralOutOfRange(Token cause, const std::string &literalType);
        FERRIT_ERROR_PRETTY_NAME("literal-out-of-range");
    };

    class CompileError::IncompatibleTypes final : public CompileError {
    public:
        explicit IncompatibleTypes(Token cause, const std::string &operation, const std::vector<std::string>& types);
        FERRIT_ERROR_PRETTY_NAME("incompatible-types");

    private:
        static std::string formatTypes(const std::vector<std::string>& types) noexcept;
    };
}
