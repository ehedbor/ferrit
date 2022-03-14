#include "BytecodeInterpreter.h"
#include "Disassembler.h"

namespace ferrit {
    BytecodeInterpreter::BytecodeInterpreter() noexcept :
        Interpreter() {
    }

    BytecodeInterpreter::BytecodeInterpreter(InterpretOptions options) noexcept :
        Interpreter(options) {
    }

    BytecodeInterpreter::BytecodeInterpreter(InterpretOptions options,
        std::ostream &output, std::ostream &errors, std::istream &input) noexcept :
        Interpreter(options, output, errors, input) {
    }

    InterpretResult BytecodeInterpreter::run(const std::string &code) {
        auto ast = parse(code);
        if (!ast.has_value()) {
            return InterpretResult::ParseError;
        }

        auto chunk = m_compiler.compile(ast.value());
        if (!chunk.has_value()) {
            return InterpretResult::CompileError;
        }

        //TODO: add a compiler flag for disassembly only
        if (m_options.traceVm) {
            Disassembler debug{*m_output};
            debug.disassembleChunk(*chunk, "<main>");
            *m_output << "\n";
        }

        // Even though this function throws exceptions,
        // none of them are caught since it indicates a bug
        // in the compiler. Therefore, the program should crash
        // with an "internal compiler error".
        m_vm.interpret(chunk.value());

        // TODO: add a way for ferrit programs to return a value and check for runtime errors
        return InterpretResult::Ok;
    }
}