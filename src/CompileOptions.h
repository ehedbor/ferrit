#pragma once

#include <unordered_map>
#include "Error.h"

namespace ferrit {
    class CompileOptions {
    public:
        CompileOptions() noexcept = default;

        [[nodiscard]] bool showLexerOutput() const noexcept;
        void setShowLexerOutput(bool showLexerOutput) noexcept;

        [[nodiscard]] bool showParserOutput() const noexcept;
        void setShowParserOutput(bool showParserOutput) noexcept;

        [[nodiscard]] WarningLevel warningLevel(const std::string &errorName) const;
        void setWarningLevel(const std::string &errorName, WarningLevel warningLevel);

    private:
        bool _showLexerOutput{false};
        bool _showParserOutput{false};
        std::unordered_map<std::string, WarningLevel> _warningLevels{};
    };
}
