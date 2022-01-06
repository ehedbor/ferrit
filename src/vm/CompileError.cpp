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

    CompileError::IncompatibleTypes::IncompatibleTypes(
        Token cause, const std::string &operation, const std::vector<std::string> &types) :
        CompileError{std::move(cause), std::format("incompatible type(s) for {}: {}", operation, formatTypes(types))} { }

    std::string CompileError::IncompatibleTypes::formatTypes(const std::vector<std::string> &types) noexcept {
        std::string result{};
        bool needsSeparator = false;
        for (const auto& type : types) {
            if (needsSeparator) {
                result.append(", ");
            }
            result.append("'").append(type).append("'");
            needsSeparator = true;
        }
        return result;
    }
}