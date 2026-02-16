
#pragma once

#include <iostream>
#include "gml/type/base.hpp"
#include "gml/vm/instructions.hpp"
#include <vector>
#include <string>
#include <cstdint>




namespace gml::interpreter
{

    std::string translate(
        const gml::type::value& instruction,
        const gml::type::value& param,
        const uint32_t len
    )
    {
        std::string name = gml::vm::isa::instr[instruction.u32 % len].name;
        std::string value = std::to_string(param.u32);
        return name + "\t" + value;
    }

    std::string translate_all(const std::vector<gml::type::value>& program)
    {
        uint32_t id = 0;
        uint32_t len = gml::vm::isa::instr.size();
        std::string str = "";

        while(id + 1 < program.size())
        {
            str += translate(program[id], program[id+1], len) + '\n';
            id += 2;
        }

        return str;
    }

}

