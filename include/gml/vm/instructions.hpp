
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

    //  register
    
    //  копирование между памятью и регистрами
    //  v1 - адрес в кеше
    //  v2 - адрес в регистре
    //  v3 - если %2 == 0, из mem в reg; если %2 == 1, из reg в mem
    void MOV(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  очистка значения
    //  v1 - адрес
    //  v2 - если %2 == 0, затирание reg, если %2 == 1, затирание mem
    //  v3 - не используется
    void DEL(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);
    
    //  обмен местами значений в регистре
    //  v1 - адрес
    //  v2 - адрес
    //  v3 - не используется
    void SWP(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  логическое или
    //  v1 - адрес первого параметра
    //  v2 - адрес второго параметра
    //  v3 - адрес записи результата
    void OR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  логическое и
    //  v1 - адрес первого параметра
    //  v2 - адрес второго параметра
    //  v3 - адрес записи результата
    void AND(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  исключающее или
    //  v1 - адрес первого параметра
    //  v2 - адрес второго параметра
    //  v3 - адрес записи результата
    void XOR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  логическое не
    //  v1 - адрес параметра
    //  v2 - не используется
    //  v3 - не используется
    void NOT(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  логический сдвиг в лево
    //  v1 - адрес параметра
    //  v2 - не используется
    //  v3 - не используется
    void SHL(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  логический сдвиг в право
    //  v1 - адрес параметра
    //  v2 - не используется
    //  v3 - не используется
    void SHR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  циклический сдвиг в лево
    //  v1 - адрес параметра
    //  v2 - не используется
    //  v3 - не используется
    void ROL(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  циклический сдвиг в право
    //  v1 - адрес параметра
    //  v2 - не используется
    //  v3 - не используется
    void ROR(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  сравнение
    //  v1 - адрес первого параметра    a
    //  v2 - адрес второго параметра    b
    //  v3 - адрес записи результата    a > b ? FFFF...FFFF : 0000...0000
    void CMP(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  безусловный переход
    //  v1 - адрес параметра перерехода
    //  v2 - не используется
    //  v3 - %2 ? 1 вперёд : 0 назад
    void JMP(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  условный переход
    //  v1 - адрес параметра перехода
    //  v2 - адрес проверяегомого значения
    //  v3 - %2 ? 1 вперёд : 0 назад
    void JZ(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    //  условный переход
    //  v1 - адрес параметра перехода
    //  v2 - адрес проверяегомого значения
    //  v3 - %2 ? 1 вперёд : 0 назад
    void JNZ(gml::vm::vm& vm, gml::type::value& v1, gml::type::value& v2, gml::type::value& v3);

    inline std::vector<instruction> instr = {
        {MOV, "MOV"},
        {DEL, "DEL"},
        {SWP, "SWP"},

        {OR, "OR"},
        {AND, "AND"},
        {XOR, "XOR"},
        {NOT, "NOT"},

        {SHL, "SHL"},
        {SHR, "SHR"},
        {ROL, "ROL"},
        {ROR, "ROR"},

        {CMP, "CMP"},
        {JMP, "JMP"},
        {JZ, "JZ"},
        {JNZ, "JNZ"}
    };

}

