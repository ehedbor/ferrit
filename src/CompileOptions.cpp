#include "CompileOptions.h"
#include <format>

namespace ferrit {
    bool CompileOptions::showLexerOutput() const noexcept {
        return _showLexerOutput;
    }

    void CompileOptions::setShowLexerOutput(bool showLexerOutput) noexcept {
        _showLexerOutput = showLexerOutput;
    }

    bool CompileOptions::showParserOutput() const noexcept {
        return _showParserOutput;
    }

    void CompileOptions::setShowParserOutput(bool showParserOutput) noexcept {
        _showParserOutput = showParserOutput;
    }

    WarningLevel CompileOptions::warningLevel(const std::string &errorName) const {
        if (const auto it = _warningLevels.find(errorName); it != _warningLevels.cend()) {
            return it->second;
        }
        throw std::out_of_range(std::format("Cannot configure warning level for '{}'", errorName));
    }

    void CompileOptions::setWarningLevel(const std::string &errorName, WarningLevel warningLevel) {
        if (auto it = _warningLevels.find(errorName); it != _warningLevels.end()) {
            it->second = warningLevel;
        }
        throw std::out_of_range(std::format("Cannot configure warning level for '{}'", errorName));
    }
}