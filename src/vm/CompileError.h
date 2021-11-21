#pragma once

#include "../Error.h"


namespace ferrit {
    /**
     * Thrown when the compiler encounters a broken invariant that should never occur.
     *
     * @example When compiling a binary expression, the parser should ensure that
     * the operator is one of a select few token types. If an unknown token type
     * is encountered, it is due to a bug in the parser, not the user's code,
     * and should result in a fatal error.
     */
    class CompileException : public std::logic_error {
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

    protected:
        using Error::Error;
    };
    
    /**
     * Indicates that a given feature is not implemented.
     */
    class CompileError::NotImplemented : public CompileError {
    public:
        NotImplemented(Token cause, const std::string &feature);
        FERRIT_ERROR_PRETTY_NAME("not-implemented");
    };
    
    /**
     * Indicates that the given literal is not representable. 
     */
    class CompileError::LiteralOutOfRange : public CompileError {
    public:
        LiteralOutOfRange(Token cause, const std::string &literalType);
        FERRIT_ERROR_PRETTY_NAME("literal-out-of-range");
    };
}
