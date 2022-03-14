#include "Chunk.h"

#include <stdexcept>

namespace ferrit {
    void Chunk::writeInstruction(OpCode opCode, int line) {
        writeRaw(static_cast<std::uint8_t>(opCode));
        addLineInfo(line);
    }

    void Chunk::writeInstruction(OpCode opCode, std::uint8_t arg, int line) {
        writeRaw(static_cast<std::uint8_t>(opCode));
        addLineInfo(line);
        writeRaw(arg);
        addLineInfo(line);
    }

    void Chunk::writeInstruction(OpCode opCode, std::uint16_t arg, int line) {
        writeRaw(static_cast<std::uint8_t>(opCode));
        addLineInfo(line);

        auto highByte = static_cast<std::uint8_t>((arg >> 8) & 0xFF);
        writeRaw(highByte);
        addLineInfo(line);

        auto lowByte = static_cast<std::uint8_t>(arg & 0xFF);
        writeRaw(lowByte);
        addLineInfo(line);
    }

    void Chunk::patchByte(int offset, std::uint8_t arg) {
        if (offset > m_bytecode.size())
        m_bytecode[offset] = arg;
    }

    void Chunk::patchShort(int offset, std::uint16_t arg) {
        m_bytecode[offset] = (arg >> 8) & 0xFF;
        m_bytecode[offset + 1] = arg & 0xFF;
    }

    std::uint8_t Chunk::byteAt(int offset) const {
        return m_bytecode[offset];
    }

    std::uint16_t Chunk::shortAt(int offset) const {
        auto hiByte = m_bytecode[offset];
        auto loByte = m_bytecode[offset + 1];
        return (hiByte << 8) | loByte;
    }

    int Chunk::size() const noexcept {
        return static_cast<int>(m_bytecode.size());
    }

    std::uint8_t Chunk::addConstant(Value value) {
        m_constantPool.push_back(value);
        return static_cast<std::uint8_t>(m_constantPool.size() - 1);
    }

    const std::vector<Value> &Chunk::constantPool() const noexcept {
        return m_constantPool;
    }

    void Chunk::writeRaw(std::uint8_t byte) {
        m_bytecode.push_back(byte);
    }

    void Chunk::addLineInfo(int line) {
        if (!m_lines.empty() && line == m_lines.back().line) {
            m_lines.back().run++;
        } else {
            m_lines.emplace_back(1, line);
        }
    }

    int Chunk::getLineForOffset(int offset) const {
        if (offset < 0) {
            throw std::invalid_argument("bytecode offset must be positive");
        }

        int onePastCurrentOffset = 0;
        for (const auto &lineInfo : m_lines) {
            onePastCurrentOffset += lineInfo.run;
            if (offset < onePastCurrentOffset) {
                return lineInfo.line;
            }
        }

        throw std::invalid_argument("bytecode offset too big; no line data");
    }
}