#include "vm/VirtualMachine.h"

#include <catch2/catch.hpp>

#include <limits>
#include <sstream>


namespace ferrit::tests {
    SCENARIO("VM execution can fail", "[vm]") {
        GIVEN("an virtual machine") {
            std::ostringstream traceLog{};
            VirtualMachine vm{traceLog};

            WHEN("executing an empty chunk") {
                Chunk chunk{};

                THEN("an exception is thrown") {
                    REQUIRE_THROWS(vm.interpret(chunk));
                }
            }

            WHEN("executing an invalid opcode") {
                Chunk chunk{};
                auto invalidOpcode = static_cast<OpCode>(std::numeric_limits<std::uint8_t>::max());
                chunk.writeInstruction(invalidOpcode, 100);

                THEN("an exception is thrown") {
                    REQUIRE_THROWS(vm.interpret(chunk));
                }
            }

            WHEN("retrieving a nonexistent constant") {
                Chunk chunk{};
                std::uint8_t badConstantIndex = 0;
                chunk.writeInstruction(OpCode::Constant, badConstantIndex, 100);
                chunk.writeInstruction(OpCode::Return, 100);

                THEN("an exception is thrown") {
                    REQUIRE_THROWS(vm.interpret(chunk));
                }
            }

            WHEN("popping a value from an empty stack") {
                Chunk chunk{};
                chunk.writeInstruction(OpCode::Negate, 100);
                chunk.writeInstruction(OpCode::Return, 100);

                THEN("an exception is thrown") {
                    REQUIRE_THROWS(vm.interpret(chunk));
                }
            }
        }
    }

    SCENARIO("VM can execute simple instructions", "[vm]") {
        GIVEN("a virtual machine") {
            std::ostringstream traceLog{};
            VirtualMachine vm{traceLog};

            WHEN("executing a valid chunk") {
                Chunk chunk{};
                std::uint8_t constant = chunk.addConstant(Value{1.2});
                chunk.writeInstruction(OpCode::Constant, constant, 14);
                chunk.writeInstruction(OpCode::Negate, 14);
                chunk.writeInstruction(OpCode::Return, 14);

                THEN("the execution completes successfully") {
                    // TODO: check return value
                    REQUIRE_NOTHROW(vm.interpret(chunk));
                    REQUIRE(traceLog.str() ==
                        "$0000   14 loadconst    0    // Constant 1.2\n"
                        "         |  -> [1.2]\n"
                        "$0002    | negate\n"
                        "         |  -> [-1.2]\n"
                        "$0003    | return\n");
                }
            }

            WHEN("executing a simple expression") {
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

                THEN("the result is computed successfully") {
                    // TODO: check return value
                    REQUIRE_NOTHROW(vm.interpret(chunk));
                    REQUIRE(traceLog.str() ==
                        "$0000  123 loadconst    0    // Constant 1.2\n"
                        "         |  -> [1.2]\n"
                        "$0002    | loadconst    1    // Constant 3.4\n"
                        "         |  -> [3.4, 1.2]\n"
                        "$0004    | add\n"
                        "         |  -> [4.6]\n"
                        "$0005    | loadconst    2    // Constant 5.6\n"
                        "         |  -> [5.6, 4.6]\n"
                        "$0007    | divide\n"
                        "         |  -> [1.2173913043478262]\n"
                        "$0008    | negate\n"
                        "         |  -> [-1.2173913043478262]\n"
                        "$0009    | return\n");
                }
            }

            WHEN("executing a complex expression") {
                Chunk chunk{};
                auto a = chunk.addConstant(Value{381.14});
                auto b = chunk.addConstant(Value{146.0});
                auto two = chunk.addConstant(Value{2.0});
                //auto epsilon = chunk.addConstant(Value{0.000001});

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
                //chunk.writeInstruction(OpCode::Constant, epsilon, 3);
                //chunk.writeInstruction(OpCode::LessThan, 2);
                chunk.writeInstruction(OpCode::Return, 3);

                THEN("the result is computed successfully") {
                    REQUIRE_NOTHROW(vm.interpret(chunk));
                    REQUIRE(traceLog.str() ==
                        "$0000    1 loadconst    0    // Constant 381.14\n"
                        "         |  -> [381.14]\n"
                        "$0002    | loadconst    1    // Constant 146.0\n"
                        "         |  -> [146.0, 381.14]\n"
                        "$0004    | add\n"
                        "         |  -> [527.14]\n"
                        "$0005    | loadconst    0    // Constant 381.14\n"
                        "         |  -> [381.14, 527.14]\n"
                        "$0007    | loadconst    1    // Constant 146.0\n"
                        "         |  -> [146.0, 381.14, 527.14]\n"
                        "$0009    | add\n"
                        "         |  -> [527.14, 527.14]\n"
                        "$000A    | multiply\n"
                        "         |  -> [277876.5796]\n"
                        "$000B    2 loadconst    0    // Constant 381.14\n"
                        "         |  -> [381.14, 277876.5796]\n"
                        "$000D    | loadconst    0    // Constant 381.14\n"
                        "         |  -> [381.14, 381.14, 277876.5796]\n"
                        "$000F    | multiply\n"
                        "         |  -> [145267.6996, 277876.5796]\n"
                        "$0010    | loadconst    2    // Constant 2.0\n"
                        "         |  -> [2.0, 145267.6996, 277876.5796]\n"
                        "$0012    | loadconst    0    // Constant 381.14\n"
                        "         |  -> [381.14, 2.0, 145267.6996, 277876.5796]\n"
                        "$0014    | multiply\n"
                        "         |  -> [762.28, 145267.6996, 277876.5796]\n"
                        "$0015    | loadconst    1    // Constant 146.0\n"
                        "         |  -> [146.0, 762.28, 145267.6996, 277876.5796]\n"
                        "$0017    | multiply\n"
                        "         |  -> [111292.87999999999, 145267.6996, 277876.5796]\n"
                        "$0018    | add\n"
                        "         |  -> [256560.5796, 277876.5796]\n"
                        "$0019    | loadconst    1    // Constant 146.0\n"
                        "         |  -> [146.0, 256560.5796, 277876.5796]\n"
                        "$001B    | loadconst    1    // Constant 146.0\n"
                        "         |  -> [146.0, 146.0, 256560.5796, 277876.5796]\n"
                        "$001D    | multiply\n"
                        "         |  -> [21316.0, 256560.5796, 277876.5796]\n"
                        "$001E    | add\n"
                        "         |  -> [277876.5796, 277876.5796]\n"
                        "$001F    3 subtract\n"
                        "         |  -> [0.0]\n"
                        "$0020    | return\n");
                }
            }
        }
    }
}
