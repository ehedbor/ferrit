#include <limits>

#include <catch2/catch.hpp>

#include "vm/VirtualMachine.h"

namespace ferrit::tests {
    SCENARIO("VM execution can fail", "[vm]") {
        GIVEN("an virtual machine") {
            auto opts = std::make_shared<CompileOptions>();
            opts->setVmTraceExecution(true);
            VirtualMachine vm{std::move(opts)};

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
        // TODO: check that the result of the execution is the expected result
        GIVEN("a virtual machine") {
            auto opts = std::make_shared<CompileOptions>();
            opts->setVmTraceExecution(true);
            VirtualMachine vm{std::move(opts)};

            WHEN("executing a valid chunk") {
                Chunk chunk{};
                std::uint8_t constant = chunk.addConstant(Value{1.2});
                chunk.writeInstruction(OpCode::Constant, constant, 14);
                chunk.writeInstruction(OpCode::Negate, 14);
                chunk.writeInstruction(OpCode::Return, 14);

                THEN("the execution completes successfully") {
                    // TODO: check return value
                    REQUIRE_NOTHROW(vm.interpret(chunk));
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
                    // TODO: check result of comparison
                    REQUIRE_NOTHROW(vm.interpret(chunk));
                }
            }
        }
    }
}
