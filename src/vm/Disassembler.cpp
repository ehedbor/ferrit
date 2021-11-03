#include "Disassembler.h"

#include <format>

namespace ferrit {
    Disassembler::Disassembler(std::ostream &output) noexcept
        : m_output(output) {
    }

    void Disassembler::disassembleChunk(const Chunk &chunk, const std::string &name) {
        m_output << "=== " << name << " ===" << std::endl;

        int offset = 0;
        while (offset < chunk.bytecode().size()) {
            offset = disassembleInstruction(chunk, offset);
        }
    }

    int Disassembler::disassembleInstruction(const Chunk &chunk, int offset) {
        m_output << std::format("${:04X} ", offset);

        int currentLine = chunk.getLineForOffset(offset);
        if (offset > 0 && currentLine == chunk.getLineForOffset(offset - 1)) {
            m_output << "   | ";
        } else {
            m_output << std::format("{:4} ", currentLine);
        }

        std::uint8_t instruction = chunk.bytecode()[offset];
        switch (instruction) {
        case static_cast<std::uint8_t>(OpCode::Constant):
            return constantInstruction("loadconst", chunk, offset);
        case static_cast<std::uint8_t>(OpCode::Return):
            return simpleInstruction("return", offset);
        default:
            m_output << "Unknown opcode " << instruction << std::endl;
            return offset + 1;
        }
    }

    int Disassembler::simpleInstruction(const std::string &name, int offset) {
        m_output << name << std::endl;
        return offset + 1;
    }

    int Disassembler::constantInstruction(const std::string &name, const Chunk &chunk, int offset) {
        std::uint8_t constantIdx = chunk.bytecode()[offset + 1];
        Value constant = chunk.constantPool()[constantIdx];
        m_output << std::format("{:10} {:3}    // Constant {}", name, constantIdx, constant) << std::endl;
        return offset + 2;
    }

}