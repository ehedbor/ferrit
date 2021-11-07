#include "VirtualMachine.h"
#include "Disassembler.h"

#include <stdexcept>
#include <format>

namespace ferrit {
    VirtualMachine::VirtualMachine(std::ostream &traceLog) noexcept :
        m_traceLog{&traceLog} {
    }

    void VirtualMachine::init(const Chunk &chunk) {
        m_chunk = chunk;
        m_ip = m_chunk.bytecode().begin();
        m_stack.clear();
    }

    void VirtualMachine::interpret(const Chunk &chunk) {
        init(chunk);

        while (true) {
            if (m_traceLog) {
                int offset = static_cast<int>(m_ip - m_chunk.bytecode().begin());
                Disassembler debug{*m_traceLog};
                debug.disassembleInstruction(m_chunk, offset);
            }

            auto instruction = readByte();
            switch (static_cast<OpCode>(instruction)) {
            case OpCode::Constant: {
                Value constant = readConstant();
                push(constant);
                break;
            }
            case OpCode::Add: {
                Value left = pop();
                Value right = pop();
                push(Value{left.asReal() + right.asReal()});
                break;
            }
            case OpCode::Subtract: {
                Value left = pop();
                Value right = pop();
                push(Value{left.asReal() - right.asReal()});
                break;
            }
            case OpCode::Multiply: {
                Value left = pop();
                Value right = pop();
                push(Value{left.asReal() * right.asReal()});
                break;
            }
            case OpCode::Divide: {
                Value left = pop();
                Value right = pop();
                push(Value{left.asReal() / right.asReal()});
                break;
            }
            case OpCode::Negate:
                push(Value{-pop().asReal()});
                break;
            case OpCode::Return:
                return;
            default:
                throw std::runtime_error(std::format("Unknown opcode '{}'", instruction));
            }

            if (m_traceLog) {
                *m_traceLog << "         |  -> [";
                int index = static_cast<int>(m_stack.size()) - 1;
                for (auto it = m_stack.crbegin(); it != m_stack.crend(); ++it) {
                    *m_traceLog << *it;
                    if (index-- > 0) {
                        *m_traceLog << ", ";
                    }
                }
                *m_traceLog << ']' << std::endl;
            }
        }
    }

    void VirtualMachine::push(Value value) {
        m_stack.push_back(value);
    }

    Value VirtualMachine::pop() {
        if (m_stack.empty()) {
            throw std::runtime_error("attempted to pop value off empty stack");
        }

        Value result = m_stack.back();
        m_stack.pop_back();
        return result;
    }

    std::uint8_t VirtualMachine::readByte() {
        if (m_ip == m_chunk.bytecode().end()) {
            throw std::runtime_error("attempted to read past end of bytecode");
        }

        return *m_ip++;
    }

    Value VirtualMachine::readConstant() {
        std::uint8_t constantIdx = readByte();
        if (constantIdx >= m_chunk.constantPool().size()) {
            throw std::runtime_error(std::format("attempted to read invalid constant index '{}'", constantIdx));
        }
        return m_chunk.constantPool().at(constantIdx);
    }
}