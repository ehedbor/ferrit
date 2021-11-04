#include <sstream>

#include <catch2/catch.hpp>

#include "vm/Chunk.h"
#include "vm/Disassembler.h"

namespace ferrit::tests {
    TEST_CASE("Load constants into a chunk", "[chunk]") {
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

    TEST_CASE("Instructions to compute quadratic", "[chunk]") {
        Chunk chunk;
        auto a = chunk.addConstant(Value{381.14});
        auto b = chunk.addConstant(Value{146.0});
        auto two = chunk.addConstant(Value{2.0});
        auto epsilon = chunk.addConstant(Value{0.000001});

        // calculate (a + b)^2 == a^2 + 2ab + b^2
        //          ((a + b) * (a + b)) == (((a * a) + ((2 * a) * b)) + (b * b))

        // (a + b) * (a + b)
        chunk.writeInstruction(OpCode::Constant, a, 1);
        chunk.writeInstruction(OpCode::Constant, b, 1);
        chunk.writeInstruction(OpCode::Add, 1);
        chunk.writeInstruction(OpCode::Constant, a, 1);
        chunk.writeInstruction(OpCode::Constant, b, 1);
        chunk.writeInstruction(OpCode::Add, 1);
        chunk.writeInstruction(OpCode::Multiply, 1);

        // (a * a)
        chunk.writeInstruction(OpCode::Constant, a, 2);
        chunk.writeInstruction(OpCode::Constant, a, 2);
        chunk.writeInstruction(OpCode::Multiply, 2);

        // ((2 * a) * b)
        chunk.writeInstruction(OpCode::Constant, two, 2);
        chunk.writeInstruction(OpCode::Constant, a, 2);
        chunk.writeInstruction(OpCode::Multiply, 2);
        chunk.writeInstruction(OpCode::Constant, b, 2);
        chunk.writeInstruction(OpCode::Multiply, 2);

        chunk.writeInstruction(OpCode::Add, 2);

        // (b * b)
        chunk.writeInstruction(OpCode::Constant, b, 2);
        chunk.writeInstruction(OpCode::Constant, b, 2);
        chunk.writeInstruction(OpCode::Multiply, 2);

        chunk.writeInstruction(OpCode::Add, 2);

        // TODO: write comparison instruction
        chunk.writeInstruction(OpCode::Subtract, 3);
        chunk.writeInstruction(OpCode::Constant, epsilon, 3);
        //chunk.writeInstruction(OpCode::LessThan, 2);

        std::stringstream stream;
        Disassembler disassembler{stream};
        disassembler.disassembleChunk(chunk, "Quadratic Equation");

        std::string expected =
            "=== Quadratic Equation ===\n"
            "$0000    1 loadconst    0    // Constant 381.14\n"
            "$0002    | loadconst    1    // Constant 146.0\n"
            "$0004    | add\n"
            "$0005    | loadconst    0    // Constant 381.14\n"
            "$0007    | loadconst    1    // Constant 146.0\n"
            "$0009    | add\n"
            "$000A    | multiply\n"
            "$000B    2 loadconst    0    // Constant 381.14\n"
            "$000D    | loadconst    0    // Constant 381.14\n"
            "$000F    | multiply\n"
            "$0010    | loadconst    2    // Constant 2.0\n"
            "$0012    | loadconst    0    // Constant 381.14\n"
            "$0014    | multiply\n"
            "$0015    | loadconst    1    // Constant 146.0\n"
            "$0017    | multiply\n"
            "$0018    | add\n"
            "$0019    | loadconst    1    // Constant 146.0\n"
            "$001B    | loadconst    1    // Constant 146.0\n"
            "$001D    | multiply\n"
            "$001E    | add\n"
            "$001F    3 subtract\n"
            "$0020    | loadconst    3    // Constant 1.0e-6\n"
            "$0022    | lessthan\n";
        REQUIRE(expected == stream.str());
    }
}