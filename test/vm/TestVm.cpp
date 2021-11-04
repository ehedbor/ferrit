#include <catch2/catch.hpp>

#include "vm/VirtualMachine.h"

namespace ferrit::tests {
    TEST_CASE("Can negate a constant", "[vm]") {
        Chunk chunk{};
        std::uint8_t constant = chunk.addConstant(Value{1.2});
        chunk.writeInstruction(OpCode::Constant, constant, 14);
        chunk.writeInstruction(OpCode::Negate, 14);
        chunk.writeInstruction(OpCode::Return, 14);

        auto opts = std::make_shared<CompileOptions>();
        opts->setVmTraceExecution(true);
        VirtualMachine vm{opts};
        CHECK_NOTHROW(vm.interpret(chunk));
    }

    TEST_CASE("Can compute a simple expression", "[vm]") {
        Chunk chunk{};

        // Compute -((1.2 + 3.4) / 5.6):
        std::uint8_t constant = chunk.addConstant(Value{1.2});
        chunk.writeInstruction(OpCode::Constant, constant, 123);

        constant = chunk.addConstant(Value{3.4});
        chunk.writeInstruction(OpCode::Constant, constant, 123);

        chunk.writeInstruction(OpCode::Add, 123);

        constant = chunk.addConstant(Value{5.6});
        chunk.writeInstruction(OpCode::Constant, constant, 123);

        chunk.writeInstruction(OpCode::Divide, 123);
        chunk.writeInstruction(OpCode::Negate, 123);

        chunk.writeInstruction(OpCode::Return, 123);

        auto opts = std::make_shared<CompileOptions>();
        opts->setVmTraceExecution(true);
        VirtualMachine vm{opts};
        CHECK_NOTHROW(vm.interpret(chunk));
    }
}