
#pragma once

#include <vector>
#include <functional>
#include "gml/vm/storage.hpp"

namespace gml::vm
{
    class vm
    {
    private:
        uint32_t max_steps;

        uint32_t input_size;
        uint32_t output_size;
        uint32_t stack_size;
        uint32_t program_size;

    public:
        gml::vm::storage::memory input;   // read-only from ISA
        gml::vm::storage::memory output;  // full access
        gml::vm::storage::stack  stack;

        std::vector<gml::type::value> program;
        std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instruction;
        uint32_t id;

        int score;

        vm(
            uint32_t program_size,
            uint32_t max_steps,
            uint32_t input_size,
            uint32_t output_size,
            uint32_t stack_size,
            std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instructions)
        {
            this->output_size = output_size;
            this->stack_size = stack_size;
            this->program_size = program_size;
            this->instruction = instructions;
            this->max_steps = max_steps;
            this->id = 0;

            program.clear();
            program.resize(this->program_size);

            this->reset();
        }

        void reset()
        {
            output.init(output_size);
            stack.init(stack_size);
        }


        void run(const std::vector<gml::type::value>& input)
        {
            uint32_t step = 0;
            this->id = 0;
            this->input.data = input;

            this->reset();

            while (step < max_steps && id + 1 < program.size())
            {
                uint32_t old_id = this->id;

                uint32_t opcode = this->program[id].u32 % this->instruction.size();
                auto& func = this->instruction[opcode];

                func(*this, this->program[id + 1]);

                if (this->id == old_id) this->id += 2;

                step++;
            }
        }


        void mutate(double chance, uint32_t value)
        {
            for (auto& p : this->program)
                if ((rand() % 1000) / 1000.0 < chance)
                    p = {rand() % value};
        }
    };
}

