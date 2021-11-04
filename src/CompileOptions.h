#pragma once

#include <unordered_map>
#include "Error.h"

namespace ferrit {
    /**
     * Container for compiler flags.
     */
    class CompileOptions {
    public:
        CompileOptions() noexcept = default;

    public:
        [[nodiscard]] bool printAst() const noexcept;
        CompileOptions &setPrintAst(bool printAst) noexcept;

        [[nodiscard]] bool silentErrors() const noexcept;
        CompileOptions &setSilentErrors(bool silentErrors) noexcept;

        [[nodiscard]] bool plainOutput() const noexcept;
        CompileOptions &setPlainOutput(bool plainOutput) noexcept;

        [[nodiscard]] bool vmTraceExecution() const noexcept;
        CompileOptions &setVmTraceExecution(bool trace) noexcept;

        [[nodiscard]] WarningLevel warningLevel(const std::string &errorName) const;
        CompileOptions &setWarningLevel(const std::string &errorName, WarningLevel warningLevel);

    private:
        bool m_printAst{false};
        bool m_silentErrors{false};
        bool m_plainOutput{false};
        bool m_vmTraceExecution{false};
        std::unordered_map<std::string, WarningLevel> m_warningLevels{};
    };
}
