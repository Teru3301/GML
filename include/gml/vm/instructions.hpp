
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


    // memory
    void IN    (gml::vm::vm& vm, gml::type::value& v);  // read from input
    void OREAD (gml::vm::vm& vm, gml::type::value& v);  // read from output
    void OWRITE(gml::vm::vm& vm, gml::type::value& v);  // write to output
    void ODEL  (gml::vm::vm& vm, gml::type::value& v);  // erase output

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
        {IN,     "IN"},     //  копирование из входной памяти в стек
        {OREAD,  "OREAD"},  //  копирование из выходной памяти в стек
        {OWRITE, "OWRITE"}, //  запись в выходную память из стека
        {ODEL,   "ODEL"},   //  запись нуля в выходную память

        {POP,  "POP"},      //  удаление верхнего элемента стека
        {PUSH, "PUSH"},     //  добавление селемента в верх стека
        {DUP,  "DUP"},      //  дублирование верхнего элемента стека
        {SWAP, "SWAP"},     //  обмен местами верхних элементов тека

        {SOR,  "SOR"},      //  логическое или
        {SAND, "SAND"},     //  логическое и
        {SXOR, "SXOR"},     //  исключающее или
        {SNOT, "SNOT"},     //  инверсия

        {SSHL, "SSHL"},     //  сдвиг в лево
        {SSHR, "SSHR"},     //  сдвиг в право
        {SROL, "SROL"},     //  циклический сдвиг в лево
        {SROR, "SROR"},     //  циклический сдвиг в право

        {SCMV, "SCMV"},     //  значение стека > значение параметра
        {VCMS, "VCMS"},     //  значение параметра > значение стека
        {SCMS, "SCMS"},     //  значение стека > значение стека

        {SJMP, "SJMP"},     //  без условный переход на (значение из стека) позиций
        {JMP,  "JMP"},      //  без условынй переход на (значение параметра) позиций
        {SJNZ, "SJNZ"},     //  переход на (значение параметра) позиций, если значение стека != 0
        {SJZ,  "SJZ"}       //  переход на (значение параметра) позиций, если занчение стека == 0
    };

}

