
#include "gml/vm/instructions.hpp"
#include "gml/functions/binary.hpp"



namespace gml::vm::isa
{

    void IN(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(vm.input.read(v));
    }

    void OREAD(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push(vm.output.read(v));
    }

    void OWRITE(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.output.write(v, vm.stack.peek());
    }

    void ODEL(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.output.erase(v);
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


    void SCMV(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push({vm.stack.pop().u32 > v.u32});
    }

    void VCMS(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.stack.push({v.u32 > vm.stack.pop().u32});
    }

    void SCMS(gml::vm::vm& vm, gml::type::value& v)
    {
        gml::type::value a = vm.stack.pop();
        gml::type::value b = vm.stack.pop();
        vm.stack.push({a.u32 > b.u32});
    }


    uint32_t jump(uint32_t id, uint32_t len, int32_t shift)
    {
        shift = (shift % static_cast<int32_t>(len) + len) % len;
        return static_cast<uint32_t>(shift);
    }

    void SJMP(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.id = jump(vm.id, vm.program.size(), vm.stack.pop().i32);
    }

    void JMP(gml::vm::vm& vm, gml::type::value& v)
    {
        vm.id = jump(vm.id, vm.program.size(), v.i32);
    }

    void SJNZ(gml::vm::vm& vm, gml::type::value& v)
    {
        if (vm.stack.pop().u32 != 0)
            vm.id = jump(vm.id, vm.program.size(), v.i32);
    }

    void SJZ(gml::vm::vm& vm, gml::type::value& v)
    {
        if (vm.stack.pop().u32 == 0)
            vm.id = jump(vm.id, vm.program.size(), v.i32);
    }

}

