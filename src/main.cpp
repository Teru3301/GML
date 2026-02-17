
#include <iostream>
#include <string>
#include <bitset>
#include <iomanip>

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
        200,    //  size stack
        100,    //  start program size
        500,    //  maximum steps
        instr   //  instructions
    );
    for (auto& m : p.members) m.mutate(1, 10);

    gml::vm::vm member = p.members[0];

    std::vector<gml::population::example> dataset;
    for (uint32_t i = 0; i < 1000; i++)
    {
        uint32_t a = rand();
        uint32_t b = rand();
        gml::population::example ex;
        ex.input.data = {{a}, {b}};
        ex.output.data = {{a+b}};
        dataset.push_back(ex);

        std::cout << a << "+" << b << "=" << a+b << std::endl;
        std::cout << print_helper::b_to_s(ex.output.data[0]) << std::endl;
    }
    uint32_t max_score = dataset.size() * dataset[0].output.data.size() * 32;
    std::cout << "max score: " << max_score << std::endl;
    

    for (int i = 0; i < 100; i++)
    {
        std::cout << std::setprecision(3) << std::fixed;
        uint32_t score = p.members[0].score;
        double precent = static_cast<double>(score) / static_cast<double>(max_score) * 100.0;
        std::cout << "ep: " << i << " \t score: " << score << " \t " << precent << "%" << std::endl;
        p.evaluate(dataset, 40);
        p.next_generation(10, 0.02);
        if (precent > 99.9999) break;
    }


    for (uint32_t i = 0; i < 5; i++)
    {
        std::cout << "----------------" << std::endl;
        std::cout << gml::interpreter::translate_all(p.members[i].program);
    }

    for (uint32_t i = 0; i < 5; i++)
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

    
    gml::vm::vm vm = p.members[0];

    for (auto d : dataset)
    {
        vm.run(d.input.data);
        uint32_t ansver = vm.output.read({0}).u32;
        std::cout << d.input.data[0].u32 << "+" << d.input.data[1].u32 << "=" << ansver << "  \t| " << d.output.read({0}).u32 << std::endl;
    }


    while (true)
    {
        std::cout << "-------------" << std::endl;
        uint32_t a;
        uint32_t b;
        std::cout << "a:"; std::cin >> a;
        std::cout << "b:"; std::cin >> b;
        std::cout << std::endl;
        vm.run({{a}, {b}});
        std::cout << a << "+" << b << "=" << vm.output.read({}).u32 << std::endl;
    }


    return 0;
}

