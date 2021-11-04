#pragma once

#include <memory>
#include <vector>

#include "../CompileOptions.h"
#include "Chunk.h"

namespace ferrit {
    class VirtualMachine {
    public:
        explicit VirtualMachine(std::shared_ptr<const CompileOptions> compileOpts) noexcept;

        void interpret(const Chunk &chunk);

    private:
        void run();

        void push(Value value);
        Value pop() noexcept;

        std::uint8_t readByte();
        Value readConstant();

    private:
        std::shared_ptr<const CompileOptions> m_compileOpts;
        Chunk m_chunk{};
        std::vector<std::uint8_t>::const_iterator m_ip{};
        std::vector<Value> m_stack{};
    };
}
