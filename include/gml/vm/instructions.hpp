
#pragma once

#include <functional>
#include <vector>
#include <string>
#include "gml/vm/vm.hpp"


namespace gml::vm::isa
{
    class instruction
    {
    public:
        std::function <void(gml::vm::vm&, gml::type::value&)> func;
        std::string name;

        instruction(
            std::function<void(gml::vm::vm&, gml::type::value&)> f,
            const std::string& n
        ) : func(f), name(n) {}
    };


    //  memory
    void MMS (gml::vm::vm& vm, gml::type::value& v);
    void MSM (gml::vm::vm& vm, gml::type::value& v);
    void MDEL(gml::vm::vm& vm, gml::type::value& v);

    //  stack
    void POP (gml::vm::vm& vm, gml::type::value& v);
    void PUSH(gml::vm::vm& vm, gml::type::value& v);
    void DUP (gml::vm::vm& vm, gml::type::value& v);
    void SWAP(gml::vm::vm& vm, gml::type::value& v);

    void SOR (gml::vm::vm& vm, gml::type::value& v);
    void SAND(gml::vm::vm& vm, gml::type::value& v);
    void SXOR(gml::vm::vm& vm, gml::type::value& v);
    void SNOT(gml::vm::vm& vm, gml::type::value& v);

    void SSHL(gml::vm::vm& vm, gml::type::value& v);
    void SSHR(gml::vm::vm& vm, gml::type::value& v);
    void SROL(gml::vm::vm& vm, gml::type::value& v);
    void SROR(gml::vm::vm& vm, gml::type::value& v);

    void SCMV(gml::vm::vm& vm, gml::type::value& v);    //  stack.  CMP     compare stack value and program value. push result to stack
    void VCMS(gml::vm::vm& vm, gml::type::value& v);    //  stack.  CMP     compare program value and stack value. push result to stack
    void SCMS(gml::vm::vm& vm, gml::type::value& v);    //  stack.  CMP     compare stack and stack. push result to stack

    void SJMP(gml::vm::vm& vm, gml::type::value& v);    //  stack.  JMP     jump stack value
    void JMP(gml::vm::vm& vm, gml::type::value& v);     //  program.JMP     jump program value
    void SJNZ(gml::vm::vm& vm, gml::type::value& v);    //  stack.  JMP     jump program value if stack value not zero
    void SJZ(gml::vm::vm& vm, gml::type::value& v);     //  stack.  JMP     jump program value if stack value zero

    inline std::vector<instruction> instr = {
        {MMS,  "MMS"},
        {MSM,  "MSM"},
        {MDEL, "MDEL"},

        {POP,  "POP"},
        {PUSH, "PUSH"},
        {DUP,  "DUP"},
        {SWAP, "SWAP"},

        {SOR,  "SOR"},
        {SAND, "SAND"},
        {SXOR, "SXOR"},
        {SNOT, "SNOT"},

        {SSHL, "SSHL"},
        {SSHR, "SSHR"},
        {SROL, "SROL"},
        {SROR, "SROR"},

        {SCMV, "SCMV"},
        {SCMS, "SCMS"},

        {SJMP, "SJMP"},
        {JMP, "JMP"},
        {SJNZ, "SJNZ"},
        {SJZ, "SJZ"}
    };

}

