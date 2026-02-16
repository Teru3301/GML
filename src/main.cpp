
#include <cstdint>
#include <iostream>
#include <string>
#include <bitset>

#include "gml/vm/instructions.hpp"
#include "gml/vm/interpreter.hpp"

#include "gml/population/population.hpp"


namespace print_helper
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
}


int main()
{
    std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instr;
    for (auto i : gml::vm::isa::instr) instr.push_back(i.func);

    gml::population::population p(
        10,     //  propulation size
        2,      //  size input memory
        1,      //  size output memory
        100,    //  size stack
        8,      //  start program size
        500,    //  maximum steps
        instr   //  instructions
    );

    gml::vm::vm member = p.members[0];

    for (auto& m : p.members)
    {
        std::cout << "----------------" << std::endl;
        std::cout << gml::interpreter::translate_all(m.program);
    }

    return 0;
}

