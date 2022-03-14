#pragma once

#include <memory>
#include <vector>
#include <ostream>
#include <optional>

#include "Chunk.h"
#include "NativeHandler.h"

namespace ferrit {
    /**
     * Executes compiled bytecode.
     */
    class VirtualMachine final {
    public:
        /**
         * Constructs a new virtual machine with no trace logging.
         *
         * @param natives native function api
         */
        explicit VirtualMachine(NativeHandler natives) noexcept;

        /**
         * Constructs a new virtual machine with trace logging.
         *
         * @param natives native function api
         * @param traceLog optional ostream to print debug information to.
         */
        explicit VirtualMachine(NativeHandler natives, std::ostream *traceLog) noexcept;

    private:
        void init(const Chunk &chunk);

    public:
        /**
         * Interprets the given chunk.
         *
         * @param chunk the chunk to interpret
         * @throw if the VM attempts to perform an illegal operation
         */
        void interpret(const Chunk &chunk);

    private:
        bool interpretInstruction(OpCode instruction);

        /**
         * Pushes a value to the stack.
         *
         * @param value the value
         */
        void push(Value value);

        /**
         * Pops a value from the stack and returns it.
         *
         * @return the value
         * @throws std::runtime_error if the stack is empty
         */
        Value pop();

        /**
         * Reads the next byte from the bytecode and increments the instruction pointer.
         *
         * @return the byte
         * @throws std::runtime_error if the end of the bytecode has been reached
         */
        std::uint8_t readByte();

        /**
         * Reads the next (big-endian) short from the bytecode, incrementing the instruction pointer as necessary.
         *
         * @return the short
         * @throws std::runtime_error if the end of the bytecode has been reached
         */
        std::uint16_t readShort();

        /**
         * Reads the constant specified by the instruction pointer and increments the pointer.
         *
         * @return the constant
         * @throws std::runtime_error if no such constant exists
         */
        Value readConstant();

        /**
         * Returns the current execution context
         */
        [[nodiscard]] ExecutionContext ctx() const;

    private:
        NativeHandler m_natives;
        std::ostream *m_traceLog{nullptr};
        Chunk m_chunk{};
        int m_ip{0};
        std::vector<Value> m_stack{};
    };
}
