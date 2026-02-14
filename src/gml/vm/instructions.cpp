
#include "gml/vm/instructions.hpp"
#include "gml/functions/binary.hpp"



namespace gml::vm::isa
{

    void MMS(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(vm.memory.read(v));
    }

    void MSM(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.memory.write(v, vm.stack.peek());
    }

    void MDEL(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.memory.erase(v);
    }


    void POP(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.pop();
    }

    void PUSH(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(v);
    }

    void DUP(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(vm.stack.peek());
    }

    void SWAP(gml::vm::vm& vm, gml::type::value& v)
    {
        auto a = vm.stack.pop();
        auto b = vm.stack.pop();

        vm.stack.push(a);
        vm.stack.push(b);
    }

    
    void SOR(gml::vm::vm& vm, gml::type::value& v)
    {
        auto b = vm.stack.pop();
        auto a = vm.stack.pop();

        vm.stack.push(gml::function::binary::OR(a, b));
    }

    void SAND(gml::vm::vm& vm, gml::type::value& v)
    {
        auto b = vm.stack.pop();
        auto a = vm.stack.pop();

        vm.stack.push(gml::function::binary::AND(a, b));
    }

    void SXOR(gml::vm::vm& vm, gml::type::value& v)
    {
        auto b = vm.stack.pop();
        auto a = vm.stack.pop();

        vm.stack.push(gml::function::binary::XOR(a, b));
    }

    void SNOT(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(gml::function::binary::NOT(vm.stack.pop()));
    }

    void SSHL(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(gml::function::binary::SHL(vm.stack.pop()));
    }

    void SSHR(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(gml::function::binary::SHR(vm.stack.pop()));
    }

    void SROL(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(gml::function::binary::ROL(vm.stack.pop()));
    }

    void SROR(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(gml::function::binary::ROR(vm.stack.pop()));
    }

}

