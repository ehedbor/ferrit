#include "vm/Chunk.h"
#include "vm/Disassembler.h"

#include <catch2/catch.hpp>

#include <format>
#include <limits>
#include <sstream>

namespace ferrit::tests {
    SCENARIO("Constant instructions", "[chunk]") {
        GIVEN("an empty chunk") {
            Chunk chunk{};
            REQUIRE(chunk.constantPool().empty());
            REQUIRE(chunk.bytecode().empty());

            WHEN("a value is added to the chunk") {
                Value value{2.0};
                std::uint8_t index = chunk.addConstant(value);

                THEN("it will be present in the chunk") {
                    REQUIRE(chunk.constantPool().size() == 1);
                    REQUIRE(chunk.constantPool()[index] == value);
                    REQUIRE(chunk.bytecode().empty());
                }
            }

            WHEN("a loadconst instruction is added to load a value") {
                Value value{3.14};
                std::uint8_t index = chunk.addConstant(value);
                chunk.writeInstruction(OpCode::Constant, index, 26);

                THEN("the instruction will be added to the bytecode") {
                    REQUIRE(chunk.constantPool().size() == 1);
                    REQUIRE(chunk.constantPool()[index] == value);

                    REQUIRE(chunk.bytecode().size() == 2);
                    REQUIRE(chunk.bytecode()[0] == static_cast<std::uint8_t>(OpCode::Constant));
                    REQUIRE(chunk.bytecode()[1] == index);

                    REQUIRE(chunk.getLineForOffset(0) == 26);
                    REQUIRE(chunk.getLineForOffset(1) == 26);
                }
            }

            WHEN("a loadconst instruction attempts to load a nonexistent value") {
                std::uint8_t badIndex = 100;
                chunk.writeInstruction(OpCode::Constant, badIndex, 38);

                THEN("the invalid instruction will still be added to the bytecode") {
                    REQUIRE(chunk.constantPool().empty());

                    REQUIRE(chunk.bytecode().size() == 2);
                    REQUIRE(chunk.bytecode()[0] == static_cast<std::uint8_t>(OpCode::Constant));
                    REQUIRE(chunk.bytecode()[1] == badIndex);

                    REQUIRE(chunk.getLineForOffset(0) == 38);
                    REQUIRE(chunk.getLineForOffset(1) == 38);
                }
            }

            WHEN("loading two constants and returning their sum") {
                Value pointOne{0.1};
                Value pointTwo{0.2};

                auto ptOneIndex = chunk.addConstant(pointOne);
                auto ptTwoIndex = chunk.addConstant(pointTwo);

                chunk.writeInstruction(OpCode::Constant, ptOneIndex, 1);
                chunk.writeInstruction(OpCode::Constant, ptTwoIndex, 2);
                chunk.writeInstruction(OpCode::Add, 3);
                chunk.writeInstruction(OpCode::Return, 4);

                THEN("the instructions will be added to the bytecode") {
                    REQUIRE(chunk.constantPool().size() == 2);
                    REQUIRE(chunk.constantPool()[ptOneIndex] == pointOne);
                    REQUIRE(chunk.constantPool()[ptTwoIndex] == pointTwo);

                    REQUIRE(chunk.bytecode().size() == 6);
                    REQUIRE(chunk.bytecode()[0] == static_cast<std::uint8_t>(OpCode::Constant));
                    REQUIRE(chunk.bytecode()[1] == ptOneIndex);
                    REQUIRE(chunk.bytecode()[2] == static_cast<std::uint8_t>(OpCode::Constant));
                    REQUIRE(chunk.bytecode()[3] == ptTwoIndex);
                    REQUIRE(chunk.bytecode()[4] == static_cast<std::uint8_t>(OpCode::Add));
                    REQUIRE(chunk.bytecode()[5] == static_cast<std::uint8_t>(OpCode::Return));

                    REQUIRE(chunk.getLineForOffset(0) == 1);
                    REQUIRE(chunk.getLineForOffset(1) == 1);
                    REQUIRE(chunk.getLineForOffset(2) == 2);
                    REQUIRE(chunk.getLineForOffset(3) == 2);
                    REQUIRE(chunk.getLineForOffset(4) == 3);
                    REQUIRE(chunk.getLineForOffset(5) == 4);
                }
            }
        }
    }

    SCENARIO("Disassembling a chunk", "[chunk]") {
        GIVEN("a chunk with code in it") {
            Chunk chunk{};

            // compute V = (((4.0 / 3.0) * pi) * ((r * r) * r))  with r=20.0

            Value four{4.0};
            Value three{3.0};
            Value pi{3.1415926535};
            Value radius{20.0};

            std::uint8_t fourIndex = chunk.addConstant(four);
            std::uint8_t threeIndex = chunk.addConstant(three);
            std::uint8_t piIndex = chunk.addConstant(pi);
            std::uint8_t radiusIndex = chunk.addConstant(radius);

            chunk.writeInstruction(OpCode::Constant, fourIndex, 1);
            chunk.writeInstruction(OpCode::Constant, threeIndex, 1);
            chunk.writeInstruction(OpCode::Divide, 1);

            chunk.writeInstruction(OpCode::Constant, piIndex, 2);
            chunk.writeInstruction(OpCode::Multiply, 2);

            chunk.writeInstruction(OpCode::Constant, radiusIndex, 3);
            chunk.writeInstruction(OpCode::Constant, radiusIndex, 3);
            chunk.writeInstruction(OpCode::Multiply, 3);
            chunk.writeInstruction(OpCode::Constant, radiusIndex, 3);
            chunk.writeInstruction(OpCode::Multiply, 3);

            chunk.writeInstruction(OpCode::Multiply, 4);
            chunk.writeInstruction(OpCode::Return, 4);

            WHEN("an instruction is disassembled") {
                std::ostringstream stream;
                Disassembler diss(stream);
                diss.disassembleInstruction(chunk, 0x08);

                THEN("the disassembly will match the instruction") {
                    REQUIRE(chunk.constantPool()[radiusIndex] == radius);
                    REQUIRE(chunk.getLineForOffset(0x08) == 3);
                    REQUIRE(chunk.getLineForOffset(0x09) == 3);
                    REQUIRE(chunk.bytecode()[0x08] == static_cast<std::uint8_t>(OpCode::Constant));
                    REQUIRE(chunk.bytecode()[0x09] == radiusIndex);

                    REQUIRE(stream.str() == "$0008    3 loadconst    3    // Constant 20.0\n");
                }
            }

            WHEN("an invalid instruction is disassembled") {
                // this assumes we don't ever get 255 opcodes
                std::uint8_t badOpcode = std::numeric_limits<std::uint8_t>::max();
                chunk.writeInstruction(static_cast<OpCode>(badOpcode), 420);

                std::ostringstream stream;
                Disassembler diss{stream};
                diss.disassembleInstruction(chunk, static_cast<std::uint8_t>(chunk.bytecode().size() - 1));

                THEN("unknown opcode will be displayed") {
                    REQUIRE(stream.str() == std::format("$0012  420 Unknown opcode {}\n", badOpcode));
                }
            }

            WHEN("the chunk is disassembled") {
                std::ostringstream stream;
                Disassembler diss(stream);
                diss.disassembleChunk(chunk, "Sphere Volume");

                THEN("the disassembly will match the written code") {
                    // remember that the offset specifies
                    // the beginning of the instruction, not the end
                    REQUIRE(chunk.bytecode().size() == 0x0012);
                    REQUIRE(stream.str() ==
                        "=== Sphere Volume ===\n"
                        "$0000    1 loadconst    0    // Constant 4.0\n"
                        "$0002    | loadconst    1    // Constant 3.0\n"
                        "$0004    | divide\n"
                        "$0005    2 loadconst    2    // Constant 3.1415926535\n"
                        "$0007    | multiply\n"
                        "$0008    3 loadconst    3    // Constant 20.0\n"
                        "$000A    | loadconst    3    // Constant 20.0\n"
                        "$000C    | multiply\n"
                        "$000D    | loadconst    3    // Constant 20.0\n"
                        "$000F    | multiply\n"
                        "$0010    4 multiply\n"
                        "$0011    | return\n");
                }
            }
        }
    }
}