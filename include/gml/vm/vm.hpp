
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
        gml::vm::storage::memory        memory;
        gml::vm::storage::stack         stack;
        std::vector<gml::type::value>   program;
        std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instruction;
        uint32_t id;

        vm(
            uint32_t program_size, uint32_t max_sateps,
            uint32_t mem_size, uint32_t stack_size, 
            std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instructions)
        {
            this->program.clear();
            program_size += program_size % 2;   //  за каждой командой идёт передаваемое значение, 
                                                //  по этому длинна программы должна быть чётной
            for (uint32_t i = 0; i < program_size; i++) this->program.push_back({});
            this->memory.init(mem_size);
            this->stack.init(stack_size);
            this->instruction = instructions;
            this->id = 0;
            this->max_steps = max_sateps;
        }

        void run()
        {
            uint32_t step = 0;
            uint32_t opcode;
            uint32_t old_id = 0;
            this->id = 0;

            while (step < max_steps && this->id + 1 < program.size())
            {
                old_id = this->id;
                opcode = program[this->id].u32 % this->instruction.size();
                auto& func = instruction[opcode];
                func(*this, this->program[id+1]);
                if (this->id == old_id) id += 2;
                step++;
            }
        }
    };

}

