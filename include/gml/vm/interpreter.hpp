
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
        const gml::type::value& v1,
        const gml::type::value& v2,
        const gml::type::value& v3,
        const uint32_t len
    )
    {
        std::string name = gml::vm::isa::instr[instruction.u32 % len].name;
        std::string value1 = std::to_string(v1.u32);
        std::string value2 = std::to_string(v2.u32);
        std::string value3 = std::to_string(v3.u32);
        return name + "\t" + value1 + "\t" + value2 + "\t" + value3;
    }


    std::string translate_all(const std::vector<gml::type::value>& program)
    {
        uint32_t id = 0;
        uint32_t len = gml::vm::isa::instr.size();
        std::string str = "";

        while(id + 3 <= program.size())
        {
            str += translate(program[id], program[id+1], program[id+2], program[id+3], len) + '\n';
            id += 4;
        }

        return str;
    }


    std::string debug_run(
        gml::vm::vm& vm, 
        const std::vector<gml::vm::isa::instruction>& instructions,
        std::vector<gml::type::value> input
        )
    {
        std::string log = "";

        uint32_t len = gml::vm::isa::instr.size();
        vm.reset();
        uint32_t step = 0;
        vm.id = 0;
        vm.input.data = input;
        
        while (step < vm.max_steps && vm.id + 3 < vm.program.size())
        {
            log += translate(vm.program[vm.id], vm.program[vm.id+1], vm.program[vm.id+2], vm.program[vm.id+3], len) + "\n";
            log += "id:    " + std::to_string(vm.id) + "\n";
            log += "step : " + std::to_string(step) + "\n";
            uint32_t opcode = vm.program[vm.id].u32 % vm.instruction.size();
            vm.execute(opcode, vm.program[vm.id + 1], vm.program[vm.id + 2], vm.program[vm.id + 3]);
            step++;
            log += "input:\n";
            for (auto& m : vm.input.data) log += "\t" + b_to_s(m) + "\n";
            log += "output:\n";
            for (auto& m : vm.output.data) log += "\t" + b_to_s(m) + "\n";
            log += "stack:\n";
            for (auto& m : vm.stack.data) log += "\t" + b_to_s(m) + "\n";
            log += "\n--------\n";
        }

        return log;
    }

}

