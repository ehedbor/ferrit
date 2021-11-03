#include <sstream>

#include <catch2/catch.hpp>

#include "vm/Chunk.h"
#include "vm/Disassembler.h"

namespace ferrit::tests {
    TEST_CASE("Load constants", "[chunk][disasembler]") {
        Value pi{3.14};
        Value zero{0.0};

        Chunk chunk;
        auto piConstIdx = chunk.addConstant(pi);
        auto zeroConstIdx = chunk.addConstant(zero);

        chunk.writeInstruction(OpCode::Constant, piConstIdx, 1);
        chunk.writeInstruction(OpCode::Constant, zeroConstIdx, 1);
        chunk.writeInstruction(OpCode::Return, 1);

        std::stringstream stream;
        Disassembler disassembler{stream};
        disassembler.disassembleChunk(chunk, "Test Chunk");

        std::string expected =
            "=== Test Chunk ===\n"
            "$0000    1 loadconst    0    // Constant 3.14\n"
            "$0002    | loadconst    1    // Constant 0.0\n"
            "$0004    | return\n";
        REQUIRE(expected == stream.str());
    }
}