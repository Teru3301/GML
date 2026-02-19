
#pragma once

#include "gml/type/base.hpp"
#include "gml/vm/instructions.hpp"
#include <vector>
#include <string>
#include <cstdint>
#include <bitset>




namespace gml::interpreter
{
    std::string b_to_s(const gml::type::value& value)
    {
        std::bitset<32> bits(value.u32);
        std::string s = bits.to_string();
        
        s.insert(8,  " ");  // после первых 8 бит
        s.insert(17, " ");  // после следующих 8 бит (8+1+8=17)
        s.insert(26, " ");  // после следующих 8 бит (17+1+8=26)
        
        return s;
    }


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

        while(id + 1 <= program.size())
        {
            str += translate(program[id], program[id+1], len) + '\n';
            id += 2;
        }

        return str;
    }

}

