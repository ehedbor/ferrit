#pragma once

#include "../Interpreter.h"
#include "BytecodeCompiler.h"
#include "VirtualMachine.h"


namespace ferrit {
    class BytecodeInterpreter : public Interpreter {
    public:
        BytecodeInterpreter() noexcept;
        BytecodeInterpreter(InterpretOptions options) noexcept; // NOLINT(google-explicit-constructor)
        BytecodeInterpreter(InterpretOptions options,
            std::ostream &output, std::ostream &errors, std::istream &input) noexcept;

    public:
        InterpretResult run(const std::string &code) override;

    private:
        BytecodeCompiler m_compiler{m_errorReporter};
        VirtualMachine m_vm{
            NativeHandler{*m_output, *m_errors, *m_input},
            (m_options.traceVm ? m_output : nullptr)};
    };
}
