
#include <iostream>
#include <iomanip>
#include "gml/type/base.hpp"
#include "gml/functions/binary.hpp"
#include <string>
#include <bitset>

#include "gml/vm/storage.hpp"


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
    gml::type::value g;

    g.u32 = 41;

    std::cout << std::setprecision(64) << std::fixed;

    std::cout << "u32 : " << g.u32  << std::endl;
    std::cout << "i32 : " << g.i32  << std::endl;
    std::cout << "f32 : " << g.f32  << std::endl;
    std::cout << "bool: " << g.b    << std::endl;
    std::cout << "byte: " << print_helper::b_to_s(g) << std::endl;
    std::cout << "c   : " << g.c << std::endl;

    std::cout << "--------------" << std::endl;

    std::cout << "OR : " << gml::function::binary::OR(g, g).b   << std::endl;
    std::cout << "AND: " << gml::function::binary::AND(g, g).b  << std::endl;
    std::cout << "XOR: " << gml::function::binary::XOR(g, g).b  << std::endl;
    std::cout << "NOT: " << gml::function::binary::NOT(g).b     << std::endl;
    std::cout << "SHL: " << print_helper::b_to_s(gml::function::binary::SHL(g))  << std::endl;
    std::cout << "SHR: " << print_helper::b_to_s(gml::function::binary::SHR(g)) << std::endl;
    std::cout << "ROL: " << print_helper::b_to_s(gml::function::binary::ROL(g)) << std::endl;
    std::cout << "ROR: " << print_helper::b_to_s(gml::function::binary::ROR(g)) << std::endl;

    return 0;
}

