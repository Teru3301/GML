
#pragma once

#include <vector>
#include <cstdint>
#include <functional>

#include "gml/vm/vm.hpp"



namespace gml::population
{
    class population
    {
    private:


    public:
        std::vector<gml::vm::vm> members;

        population(
            uint32_t size,
            uint32_t input_size,
            uint32_t output_size,
            uint32_t stack_size,
            uint32_t program_size,
            uint32_t max_steps,
            std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instructions
        )
        {
            for (uint32_t i = 0; i < size; i++)
            {
                gml::vm::vm m(
                    program_size, max_steps,
                    input_size, output_size, stack_size,
                    instructions
                );
                m.mutate(1, -1);
                this->members.push_back(m);
            }
        }
    };
}

