#pragma once

#include <cstdint>
#include <vector>

#include "Value.h"


namespace ferrit {
    /**
     * Represents a possible virtual machine operation.
     */
    enum class OpCode {
        NoOp,
        Constant,
        Pop,
        IAdd,
        ISubtract,
        IMultiply,
        IDivide,
        IModulus,
        INegate,
        FAdd,
        FSubtract,
        FMultiply,
        FDivide,
        FModulus,
        FNegate,
        BAnd,
        BOr,
        BNot,
        BEqual,
        BNotEqual,
        Return,
        Jump,
        JumpIfFalse,
    };

    /**
     * Represents a collection of VM operations.
     */
    class Chunk final {
    public:
        explicit Chunk() = default;

        /**
         * Write the given instruction to the chunk.
         *
         * @param opCode the instruction's opcode
         * @param line the line that the instruction was generated on
         */
        void writeInstruction(OpCode opCode, int line);

        /**
         * Write the given instruction and its argument to the chunk.
         *
         * @param opCode the instruction's opcode
         * @param arg the argument
         * @param line the line that the instruction was generated on
         */
        void writeInstruction(OpCode opCode, std::uint8_t arg, int line);

        /**
         * Write the given instruction and its argument to the chunk.
         *
         * @param opCode the instruction's opcode
         * @param arg the argument
         * @param line the line that the instruction was generated on
         */
        void writeInstruction(OpCode opCode, std::uint16_t arg, int line);

        /**
         * This function is deleted to prevent unintentional coercion of a
         * byte argument to a line number.
         */
        void writeInstruction(OpCode, std::uint8_t) = delete;

        /**
         * Overwrites the byte at the specified index.
         */
        void patchByte(int offset, std::uint8_t arg);

        /**
         * Overwrites the short at the specified index.
         */
        void patchShort(int offset, std::uint16_t arg);

        /**
         * Returns the byte at the specified offset.
         */
        [[nodiscard]] std::uint8_t byteAt(int offset) const;


        /**
         * Returns the short at the specified offset.
         */
        [[nodiscard]] std::uint16_t shortAt(int offset) const;

        /**
         * Returns the number of bytes in this chunk's bytecode.
         */
        [[nodiscard]] int size() const noexcept;

        /**
         * Adds the given value to the constant pool.
         *
         * @param value the value to add
         * @return index of the newly added constant
         */
        std::uint8_t addConstant(Value value);

        [[nodiscard]] const std::vector<Value> &constantPool() const noexcept;

        /**
         * Retrieves the line information for the given offset.
         *
         * @param offset the byte offset
         * @return the line number at that offset
         * @throws std::invalid_argument if no line number exists for that offset.
         */
        [[nodiscard]] int getLineForOffset(int offset) const;

    private:
        /**
         * Simple struct to store line information, for debugging purposes.
         * Each additional byte should either increment the current LineInfo's run
         * or create a new LineInfo struct.
         */
        struct LineInfo {
            int run;
            int line;
        };

        /**
         * Append the given byte to the chunk.
         *
         * @param byte the byte to append
         */
        void writeRaw(std::uint8_t byte);

        /**
         * Add debug line information for the next bytecode offset.
         *
         * @param line the line
         */
        void addLineInfo(int line);

    private:
        std::vector<std::uint8_t> m_bytecode{};
        std::vector<LineInfo> m_lines{};
        std::vector<Value> m_constantPool{};
    };
}