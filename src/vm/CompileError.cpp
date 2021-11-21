#include "CompileError.h"

namespace ferrit {
    CompileError::NotImplemented::NotImplemented(
        Token cause, const std::string &feature) :
        CompileError{std::move(cause), std::format("{} not implemented", feature)} {
    }

    CompileError::LiteralOutOfRange::LiteralOutOfRange(
        Token cause, const std::string &literalType) :
        CompileError{std::move(cause), std::format("{} out of range", literalType)} {
    }
}