
#pragma once

#include <vector>
#include <functional>
#include <random>

#include "gml/vm/storage.hpp"



namespace gml::vm
{
    class vm
    {
    private:

        uint32_t input_size;
        uint32_t output_size;
        uint32_t stack_size;
        uint32_t program_size;
        uint32_t register_size;
        uint32_t mem_size;

    public:
        gml::vm::storage::memory input;     // read-only from ISA
        gml::vm::storage::memory output;    // full access
        gml::vm::storage::memory reg;       // full access
        gml::vm::storage::memory mem;       // full access
        gml::vm::storage::stack  stack;

        std::vector<gml::type::value> program;
        std::vector<std::function<void(gml::vm::vm&, gml::type::value&, gml::type::value&, gml::type::value&)>> instruction;
        uint32_t id;
        uint32_t max_steps;
        int score;

        vm(
            uint32_t program_size,
            uint32_t max_steps,
            uint32_t input_size,
            uint32_t output_size,
            uint32_t stack_size,
            std::vector<std::function<void(gml::vm::vm&, gml::type::value&, gml::type::value&, gml::type::value&)>> instructions)
        {
            this->register_size = 4;
            this->mem_size = 1;
            this->output_size = output_size;
            this->stack_size = stack_size;
            this->program_size = program_size;
            this->instruction = instructions;
            this->max_steps = max_steps;
            this->id = 0;
            this->score = 0;

            program.clear();
            program.resize(this->program_size);

            this->reset();
        }

        void reset()
        {
            this->output.init(this->output_size);
            this->stack.init(this->stack_size);
            this->reg.init(this->register_size);
            this->mem.init(this->mem_size);
        }


        void execute(uint32_t opcode, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3)
        {
            uint32_t old_id = this->id;
            auto& func = this->instruction[opcode];
            func(*this, v1, v2, v3);
            if (this->id == old_id) this->id += 4;
        }


        void run(const std::vector<gml::type::value>& input)
        {
            this->reset();
            uint32_t step = 0;
            this->id = 0;
            this->input.data = input;

            while (step < max_steps && id + 3 < program.size())
            {
                uint32_t opcode = this->program[id].u32 % this->instruction.size();
                this->execute(opcode, this->program[id + 1], this->program[id + 2], this->program[id + 3]);
                step++;
            }
        }


        void mutate(double chance, uint32_t max_value)
        {
            thread_local std::minstd_rand rng(std::random_device{}());

            std::bernoulli_distribution mutate_dist(chance);
            std::uniform_int_distribution<uint32_t> value_dist(0, max_value - 1);

            for (auto& p : this->program)
            {
                if (mutate_dist(rng))
                {
                    p = { value_dist(rng) };
                }
            }
        }


    };
}

