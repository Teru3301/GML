
#include <cstdint>
#include <iostream>
#include <string>
#include <bitset>

#include "gml/vm/vm.hpp"
#include "gml/vm/instructions.hpp"
#include "gml/vm/interpreter.hpp"


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
    int program_size = 10;
    int max_steps = 100;
    int input_size = 2;
    int output_size = 1;
    int stack_size = 100;
    std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instr;
    for (auto i : gml::vm::isa::instr) instr.push_back(i.func);

    gml::vm::vm vm(program_size, max_steps, input_size, output_size, stack_size, instr);

    for (auto& i : vm.program) i.u32 = rand() % 100;
    vm.input.write({0}, {254});
    vm.input.write({1}, {1023});

    vm.run();

    std::cout << vm.id << std::endl;
    std::cout << "------- input" << std::endl;
    for (uint32_t i = 0; i < input_size; i++) std::cout << print_helper::b_to_s(vm.input.read({i})) << std::endl;
    std::cout << "------- output" << std::endl;
    for (uint32_t i = 0; i < output_size; i++) std::cout << print_helper::b_to_s(vm.output.read({i})) << std::endl;
    std::cout << "------- stack" << std::endl;
    while(vm.stack.data.size()) std::cout << print_helper::b_to_s(vm.stack.pop()) << std::endl;
    std::cout << "------- assembly" << std::endl;
    std::cout << gml::interpreter::translate_all(vm.program);
    std::cout << "-------" << std::endl;
    
    vm.mutate(0.1, 100);
    vm.run();
    std::cout << "------- input" << std::endl;
    for (uint32_t i = 0; i < input_size; i++) std::cout << print_helper::b_to_s(vm.input.read({i})) << std::endl;
    std::cout << "------- output" << std::endl;
    for (uint32_t i = 0; i < output_size; i++) std::cout << print_helper::b_to_s(vm.output.read({i})) << std::endl;
    std::cout << "------- stack" << std::endl;
    while(vm.stack.data.size()) std::cout << print_helper::b_to_s(vm.stack.pop()) << std::endl;
    std::cout << "------- assembly" << std::endl;
    std::cout << gml::interpreter::translate_all(vm.program);

    return 0;
}

