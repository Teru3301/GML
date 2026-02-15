
#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include "gml/vm/storage.hpp"

namespace gml::vm
{
    class vm
    {
    private:
        uint32_t max_steps;

    public:
        gml::vm::storage::memory input;   // read-only from ISA
        gml::vm::storage::memory output;  // full access
        gml::vm::storage::stack  stack;

        std::vector<gml::type::value> program;
        std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instruction;

        uint32_t id;

        vm(
            uint32_t program_size,
            uint32_t max_steps,
            uint32_t input_size,
            uint32_t output_size,
            uint32_t stack_size,
            std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instructions)
        {
            program.clear();
            program_size += program_size % 2;

            program.resize(program_size);

            input.init(input_size);
            output.init(output_size);
            stack.init(stack_size);

            instruction = instructions;

            id = 0;
            this->max_steps = max_steps;
        }

        void run()
        {
            uint32_t step = 0;
            id = 0;

            while (step < max_steps && id + 1 < program.size())
            {
                uint32_t old_id = id;

                uint32_t opcode = program[id].u32 % instruction.size();
                auto& func = instruction[opcode];

                func(*this, program[id + 1]);

                if (id == old_id)
                    id += 2;

                step++;
            }
        }
    };
}

