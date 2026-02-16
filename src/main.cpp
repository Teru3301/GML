
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
        10000,  //  propulation size
        2,      //  size input memory
        1,      //  size output memory
        100,    //  size stack
        20,     //  start program size
        500,    //  maximum steps
        instr   //  instructions
    );
    for (auto& m : p.members) m.mutate(0.1, 10);

    gml::vm::vm member = p.members[0];

    std::vector<gml::population::example> dataset;
    for (uint32_t i = 0; i < 1; i++)
    {
        gml::population::example ex;
        ex.input.data = {{i}, {i+1}, {2}};
        ex.output.data = {{i-i-1}};
        dataset.push_back(ex);

        std::cout << i << "+" << i+1 << "=" << i+i+1 << std::endl;
        std::cout << print_helper::b_to_s(ex.output.data[0]) << std::endl;
    }
    
    p.evaluate(dataset);
    p.sort_by_score();

    for (uint32_t i = 0; i < 10; i++)
    {
        std::cout << "----------------" << std::endl;
        std::cout << gml::interpreter::translate_all(p.members[i].program);
    }

    for (uint32_t i = 0; i < 10; i++)
    {
        gml::vm::vm vm = p.members[i];
        std::cout << vm.score << std::endl;
        std::cout << "input:" << std::endl;
        for (auto& m : vm.input.data)
            std::cout << "\t" << print_helper::b_to_s(m) << std::endl;
        std::cout << "output:" << std::endl;
        for (auto& m : vm.output.data)
            std::cout << "\t" << print_helper::b_to_s(m) << std::endl;
        std::cout << "stack:" << std::endl;
        for (auto& m : vm.stack.data)
            std::cout << "\t" << print_helper::b_to_s(m) << std::endl;
        std::cout << "----------------" << std::endl;
    }

    return 0;
}

