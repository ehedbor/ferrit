#pragma once

#include <memory>
#include <vector>
#include <ostream>
#include <optional>

#include "Chunk.h"

namespace ferrit {
    /**
     * Executes compiled bytecode.
     */
    class VirtualMachine final {
    public:
        /**
         * Constructs a new virtual machine with no trace logging.
         */
        VirtualMachine() noexcept = default;

    private:
        void init(const Chunk &chunk);

    public:
        /**
         * Constructs a new virtual machine with trace logging.
         *
         * @param traceLog ostream to print debug information to.
         */
        explicit VirtualMachine(std::ostream &traceLog) noexcept;

        /**
         * Interprets the given chunk.
         *
         * @param chunk the chunk to interpret
         * @throw if the VM attempts to perform an illegal operation
         */
        void interpret(const Chunk &chunk);

    private:
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
         * Reads the constant specified by the instruction pointer and increments the pointer.
         *
         * @return the constant
         * @throws std::runtime_error if no such constant exists
         */
        Value readConstant();

    private:
        std::ostream *m_traceLog{nullptr};
        Chunk m_chunk{};
        std::vector<std::uint8_t>::const_iterator m_ip{};
        std::vector<Value> m_stack{};
    };
}
