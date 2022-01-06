#include "VirtualMachine.h"
#include "Disassembler.h"

#include <cmath>
#include <stdexcept>
#include <format>

namespace ferrit {
    VirtualMachine::VirtualMachine(NativeHandler natives) noexcept :
        m_natives{natives} {
    }

    VirtualMachine::VirtualMachine(NativeHandler natives, std::ostream *traceLog) noexcept :
        m_natives{natives}, m_traceLog{traceLog} {
    }

    void VirtualMachine::init(const Chunk &chunk) {
        m_chunk = chunk;
        m_ip = m_chunk.bytecode().begin();
        m_stack.clear();
    }

    void VirtualMachine::interpret(const Chunk &chunk) {
        init(chunk);

        bool run = true;
        while (run) {
            if (m_traceLog) {
                int offset = static_cast<int>(m_ip - m_chunk.bytecode().begin());
                Disassembler debug{*m_traceLog};
                debug.disassembleInstruction(m_chunk, offset);
            }

            auto instruction = static_cast<OpCode>(readByte());
            try {
                run = interpretInstruction(instruction);
            } catch (const PanicError &) {
                run = false;
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

    bool VirtualMachine::interpretInstruction(OpCode instruction) {
        switch (static_cast<OpCode>(instruction)) {
        case OpCode::Constant: {
            Value constant = readConstant();
            push(constant);
            break;
        }
        case OpCode::IAdd: {
            std::int64_t right = pop().asInteger();
            std::int64_t left = pop().asInteger();
            push(Value{left + right});
            break;
        }
        case OpCode::ISubtract: {
            std::int64_t right = pop().asInteger();
            std::int64_t left = pop().asInteger();
            push(Value{left - right});
            break;
        }
        case OpCode::IMultiply: {
            std::int64_t right = pop().asInteger();
            std::int64_t left = pop().asInteger();
            push(Value{left * right});
            break;
        }
        case OpCode::IDivide: {
            std::int64_t right = pop().asInteger();
            std::int64_t left = pop().asInteger();
            if (right == 0) {
                m_natives.panic(ctx(), "error: attempted divide by zero");
            }
            push(Value{left / right});
            break;
        }
        case OpCode::IModulus: {
            std::int64_t right = pop().asInteger();
            std::int64_t left = pop().asInteger();
            if (right == 0) {
                m_natives.panic(ctx(), "error: attempted divide by zero");
            }
            push(Value{left % right});
            break;
        }
        case OpCode::INegate: {
            std::int64_t argument = pop().asInteger();
            push(Value{-argument});
            break;
        }
        case OpCode::FAdd: {
            double right = pop().asReal();
            double left = pop().asReal();
            push(Value{left + right});
            break;
        }
        case OpCode::FSubtract: {
            double right = pop().asReal();
            double left = pop().asReal();
            push(Value{left - right});
            break;
        }
        case OpCode::FMultiply: {
            double right = pop().asReal();
            double left = pop().asReal();
            push(Value{left * right});
            break;
        }
        case OpCode::FDivide: {
            double right = pop().asReal();
            double left = pop().asReal();
            // note division by zero is allowed for reals
            push(Value{left / right});
            break;
        }
        case OpCode::FModulus: {
            double right = pop().asReal();
            double left = pop().asReal();
            push(Value{std::fmod(left, right)});
            break;
        }
        case OpCode::FNegate: {
            double argument = pop().asReal();
            push(Value{-argument});
            break;
        }
        case OpCode::BAnd: {
            bool right = pop().asBoolean();
            bool left = pop().asBoolean();
            push(Value{left && right});
            break;
        }
        case OpCode::BOr: {
            bool right = pop().asBoolean();
            bool left = pop().asBoolean();
            push(Value{left || right});
            break;
        }
        case OpCode::BNot: {
            bool argument = pop().asBoolean();
            push(Value{!argument});
            break;
        }
        case OpCode::BEqual: {
            bool right = pop().asBoolean();
            bool left = pop().asBoolean();
            push(Value{left == right});
            break;
        }
        case OpCode::BNotEqual: {
            bool right = pop().asBoolean();
            bool left = pop().asBoolean();
            push(Value{left != right});
            break;
        }
        case OpCode::Return: {
            if (!m_stack.empty()) {
                m_natives.println(ctx(), std::format("{}", pop()));
            }
            return false;
        }
        default:
            throw std::runtime_error(std::format("Unknown opcode '{}'", static_cast<int>(instruction)));
        }
        return true;
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

    ExecutionContext VirtualMachine::ctx() const {
        // subtract 1 because we have already consumed the current instruction at this point
        auto offset = std::distance(m_chunk.bytecode().begin(), m_ip) - 1;
        int line = m_chunk.getLineForOffset(static_cast<int>(offset));
        return ExecutionContext{
            .line = line
        };
    }
}