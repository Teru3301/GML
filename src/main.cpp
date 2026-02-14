
#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>

#include "gml/vm/vm.hpp"
#include "gml/vm/instructions.hpp"


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
    int program_size = 100;
    int memory_size = 100;
    int stack_size = 100;
    std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instr;
    for (auto i : gml::vm::isa::instr)
        instr.push_back(i.func);

    gml::vm::vm vm(program_size, memory_size, stack_size, instr);





    return 0;
}

