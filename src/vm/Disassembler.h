#pragma once

#include <ostream>
#include <string>

#include "Chunk.h"

namespace ferrit {
    /**
     * Class to view a disassembled version of compiled bytecode.
     */
    class Disassembler final {
    public:
        /**
         * Constructs a new disassembler.
         *
         * @param output the ostream to which all output is printed
         */
        explicit Disassembler(std::ostream &output) noexcept;

        /**
         * Writes the disassembly of the given chunk to the output stream.
         *
         * @param chunk the chunk
         * @param name the chunk's name
         */
        void disassembleChunk(const Chunk &chunk, const std::string &name);

        /**
         * Writes the disassembled instruction at the given offset to the output stream.
         *
         * @param chunk the chunk
         * @param offset instruction byte offset in the chunk
         * @return next offset to check.
         */
        int disassembleInstruction(const Chunk &chunk, int offset);

    private:
        /**
         * Write an instruction taking no parameters.
         *
         * @param name the name to display for the instruction
         * @param offset the instruction's offset in the chunk
         * @return the next offset
         */
        int simpleInstruction(const std::string &name, int offset);

        /**
         * Write an instruction that takes one parameter.
         *
         * @param name the name to display for the instruction
         * @param chunk the instruction's chunk
         * @param offset the instruction's offset in the chunk
         * @return the next offset
         */
        int constantInstruction(const std::string &name, const Chunk &chunk, int offset);

        /**
         * Write a jump instruction.
         *
         * @param name the name to display for the instruction
         * @param chunk the instruction's chunk
         * @param offset the instruction's offset in the chunk
         * @return the next offset
         */
        int jumpInstruction(const std::string &name, const Chunk &chunk, int offset);

    private:
        std::ostream &m_output;
    };
}
