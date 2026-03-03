
#include "gml/vm/instructions.hpp"
#include "gml/functions/binary.hpp"



namespace gml::vm::isa
{

    //==================================================
    // вспомогательный jump (как в старой версии)
    //==================================================

    static uint32_t jump(uint32_t id, uint32_t len, uint32_t shift, bool add)
    {
        /*
        shift %= static_cast<int32_t>(len);
        shift += shift % 4;
        id += shift;

        if (id < 0) id = 0;
        if (id > static_cast<int32_t>(len)) id = len;

        return static_cast<uint32_t>(id);
    */
        shift %= 4;
        if (add)
        {
            id += shift;
        }
        else
        {
            id = (id >= shift ? id - shift : 0);
        }
        return id;
    }

    //==================================================
    // MOV
    // v1 - адрес в mem
    // v2 - адрес в reg
    // v3 - mode: 0 mem->reg, 1 reg->mem
    //==================================================

    void MOV(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value& v3)
    {
        uint32_t mode = v3.u32 % 7;

        gml::type::value val;

        switch (mode)
        {
            //==============================
            // input → mem
            //==============================
            case 0:
                val = vm.input.read(v1);
                vm.mem.write(v2, val);
                break;

            //==============================
            // input → reg
            //==============================
            case 1:
                val = vm.input.read(v1);
                vm.reg.write(v2, val);
                break;

            //==============================
            // input → output
            //==============================
            case 2:
                val = vm.input.read(v1);
                vm.output.write(v2, val);
                break;

            //==============================
            // mem → reg
            //==============================
            case 3:
                val = vm.mem.read(v1);
                vm.reg.write(v2, val);
                break;

            //==============================
            // mem → output
            //==============================
            case 4:
                val = vm.mem.read(v1);
                vm.output.write(v2, val);
                break;

            //==============================
            // reg → mem
            //==============================
            case 5:
                val = vm.reg.read(v1);
                vm.mem.write(v2, val);
                break;

            //==============================
            // reg → output
            //==============================
            case 6:
                val = vm.reg.read(v1);
                vm.output.write(v2, val);
                break;
        }
    }

    //==================================================
    // DEL
    // v1 - адрес
    // v2 - 0 reg, 1 mem
    //==================================================

    void DEL(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value&)
    {
        if ((v2.u32 & 1) == 0)
            vm.reg.erase(v1);
        else
            vm.mem.erase(v1);
    }

    //==================================================
    // SWP (регистр ↔ регистр)
    //==================================================

    void SWP(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value&)
    {
        auto a = vm.reg.read(v1);
        auto b = vm.reg.read(v2);

        vm.reg.write(v1, b);
        vm.reg.write(v2, a);
    }

    //==================================================
    // Логические операции (только регистры)
    //==================================================

    void OR(gml::vm::vm& vm,
            gml::type::value& v1,
            gml::type::value& v2,
            gml::type::value& v3)
    {
        auto a = vm.reg.read(v1);
        auto b = vm.reg.read(v2);
        vm.reg.write(v3, gml::function::binary::OR(a, b));
    }

    void AND(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value& v3)
    {
        auto a = vm.reg.read(v1);
        auto b = vm.reg.read(v2);
        vm.reg.write(v3, gml::function::binary::AND(a, b));
    }

    void XOR(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value& v3)
    {
        auto a = vm.reg.read(v1);
        auto b = vm.reg.read(v2);
        vm.reg.write(v3, gml::function::binary::XOR(a, b));
    }

    //==================================================
    // Унарные операции
    //==================================================

    void NOT(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value&,
             gml::type::value&)
    {
        auto a = vm.reg.read(v1);
        vm.reg.write(v1, gml::function::binary::NOT(a));
    }

    void SHL(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value&,
             gml::type::value&)
    {
        auto a = vm.reg.read(v1);
        vm.reg.write(v1, gml::function::binary::SHL(a));
    }

    void SHR(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value&,
             gml::type::value&)
    {
        auto a = vm.reg.read(v1);
        vm.reg.write(v1, gml::function::binary::SHR(a));
    }

    void ROL(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value&,
             gml::type::value&)
    {
        auto a = vm.reg.read(v1);
        vm.reg.write(v1, gml::function::binary::ROL(a));
    }

    void ROR(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value&,
             gml::type::value&)
    {
        auto a = vm.reg.read(v1);
        vm.reg.write(v1, gml::function::binary::ROR(a));
    }

    //==================================================
    // CMP
    // v3 = FFFF... если v1 > v2 иначе 0
    //==================================================

    void CMP(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value& v3)
    {
        auto a = vm.reg.read(v1);
        auto b = vm.reg.read(v2);

        gml::type::value result;
        result.u32 = (a.u32 > b.u32) ? 0xFFFFFFFFu : 0u;

        vm.reg.write(v3, result);
    }

    //==================================================
    // JMP
    //==================================================

    void JMP(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value& v3)
    {
        auto shift = vm.reg.read(v1);
        vm.id = jump(vm.id, vm.program.size(), shift.i32, v3.u32%2==1);
    }

    //==================================================
    // JZ
    //==================================================

    void JZ(gml::vm::vm& vm,
            gml::type::value& v1,
            gml::type::value& v2,
            gml::type::value& v3)
    {
        auto shift = vm.reg.read(v1);
        auto cond  = vm.reg.read(v2);

        if (cond.u32 == 0)
            vm.id = jump(vm.id, vm.program.size(), shift.i32, v3.u32%2==1);
    }

    //==================================================
    // JNZ
    //==================================================

    void JNZ(gml::vm::vm& vm,
             gml::type::value& v1,
             gml::type::value& v2,
             gml::type::value& v3)
    {
        auto shift = vm.reg.read(v1);
        auto cond  = vm.reg.read(v2);

        if (cond.u32 != 0)
            vm.id = jump(vm.id, vm.program.size(), shift.i32, v3.u32%2==1);
    }

}

