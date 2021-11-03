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

    std::uint8_t Chunk::addConstant(Value value) {
        m_constantPool.push_back(value);
        return static_cast<std::uint8_t>(m_constantPool.size() - 1);
    }

    const std::vector<std::uint8_t> &Chunk::bytecode() const noexcept {
        return m_bytecode;
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