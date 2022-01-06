#include "Disassembler.h"

#include <format>

namespace ferrit {
    Disassembler::Disassembler(std::ostream &output) noexcept
        : m_output(output) {
    }

    void Disassembler::disassembleChunk(const Chunk &chunk, const std::string &name) {
        m_output << std::format("=== {} ===\n", name);

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
        switch (static_cast<OpCode>(instruction)) {
        case OpCode::Constant:
            return constantInstruction("const", chunk, offset);
        case OpCode::IAdd:
            return simpleInstruction("iadd", offset);
        case OpCode::ISubtract:
            return simpleInstruction("isub", offset);
        case OpCode::IMultiply:
            return simpleInstruction("imul", offset);
        case OpCode::IDivide:
            return simpleInstruction("idiv", offset);
        case OpCode::IModulus:
            return simpleInstruction("imod", offset);
        case OpCode::INegate:
            return simpleInstruction("ineg", offset);
        case OpCode::FAdd:
            return simpleInstruction("fadd", offset);
        case OpCode::FSubtract:
            return simpleInstruction("fsub", offset);
        case OpCode::FMultiply:
            return simpleInstruction("fmul", offset);
        case OpCode::FDivide:
            return simpleInstruction("fdiv", offset);
        case OpCode::FModulus:
            return simpleInstruction("fmod", offset);
        case OpCode::FNegate:
            return simpleInstruction("fneg", offset);
        case OpCode::Return:
            return simpleInstruction("ret", offset);
        default:
            m_output << std::format("Unknown opcode {}\n", instruction);
            return offset + 1;
        }
    }

    int Disassembler::simpleInstruction(const std::string &name, int offset) {
        m_output << std::format("{}\n", name);
        return offset + 1;
    }

    int Disassembler::constantInstruction(const std::string &name, const Chunk &chunk, int offset) {
        std::uint8_t constantIdx = chunk.bytecode()[offset + 1];
        if (constantIdx >= chunk.constantPool().size()) {
            throw std::logic_error("constant index too big");
        }

        Value constant = chunk.constantPool()[constantIdx];
        m_output << std::format("{:10} {:3}    // Constant {}\n", name, constantIdx, constant);
        return offset + 2;
    }

}