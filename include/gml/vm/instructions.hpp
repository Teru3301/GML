
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
        std::function<void(gml::vm::vm&,
                           gml::type::value&,
                           gml::type::value&,
                           gml::type::value&)> func;

        std::string name;

        instruction(
            std::function<void(gml::vm::vm&,
                               gml::type::value&,
                               gml::type::value&,
                               gml::type::value&)> f,
            const std::string& n
        ) : func(f), name(n) {}
    };


    // memory
    void IN    (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void OREAD (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void OWRITE(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void ODEL  (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    // stack
    void POP (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void PUSH(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void DUP (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SWAP(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    void SOR (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SAND(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SXOR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SNOT(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    void SSHL(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SSHR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SROL(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SROR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    void SCMV(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void VCMS(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SCMS(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    void SJMP(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void JMP (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SJNZ(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    void SJZ (gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

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

