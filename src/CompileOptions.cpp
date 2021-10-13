#include "CompileOptions.h"
#include <format>

namespace ferrit {
    bool CompileOptions::printAst() const noexcept {
        return m_printAst;
    }

    CompileOptions &CompileOptions::setPrintAst(bool printAst) noexcept {
        m_printAst = printAst;
        return *this;
    }

    bool CompileOptions::silentErrors() const noexcept {
        return m_silentErrors;
    }

    CompileOptions &CompileOptions::setSilentErrors(bool silentErrors) noexcept {
        m_silentErrors = silentErrors;
        return *this;
    }

    bool CompileOptions::plainOutput() const noexcept {
        return m_plainOutput;
    }

    CompileOptions &CompileOptions::setPlainOutput(bool plainOutput) noexcept {
        m_plainOutput = plainOutput;
        return *this;
    }

    WarningLevel CompileOptions::warningLevel(const std::string &errorName) const {
        if (const auto it = m_warningLevels.find(errorName); it != m_warningLevels.cend()) {
            return it->second;
        }
        throw std::out_of_range(std::format("Cannot configure warning level for '{}'", errorName));
    }

    CompileOptions &CompileOptions::setWarningLevel(const std::string &errorName, WarningLevel warningLevel) {
        if (auto it = m_warningLevels.find(errorName); it != m_warningLevels.end()) {
            it->second = warningLevel;
            return *this;
        }
        throw std::out_of_range(std::format("Cannot configure warning level for '{}'", errorName));
    }
}