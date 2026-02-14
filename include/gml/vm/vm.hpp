
#pragma once

#include <vector>
#include <functional>
#include "gml/vm/storage.hpp"



namespace gml::vm
{

    class vm
    {
    private:

    public:
        gml::vm::storage::memory        memory;
        gml::vm::storage::stack         stack;
        //gml::vm::storage::reg           reg;

        std::vector<gml::type::value>   program;
        std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instruction;

        vm(uint32_t program_size, uint32_t mem_size, uint32_t stack_size, 
                std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instructions)
        {
            this->program.clear();
            program_size += program_size % 2;   //  за каждой командой идёт передаваемое значение, 
                                                //  по этому длинна программы должна быть чётной
            for (uint32_t i = 0; i < program_size; i++) this->program.push_back({});
            this->memory.init(mem_size);
            this->stack.init(stack_size);
            this->instruction = instructions;
        }
    };

}

