
#include "gml/functions/binary.hpp"


namespace gml::function::binary
{

    //  битовое ИЛИ (OR) - побитовая операция "ИЛИ" над 32-битными значениями
    gml::type::value OR(const gml::type::value& a,
                        const gml::type::value& b)
    {
        return { a.u32 | b.u32 };
    }

    //  битовое И (AND) - побитовая операция "И" над 32-битными значениями
    gml::type::value AND(const gml::type::value& a,
                         const gml::type::value& b)
    {
        return { a.u32 & b.u32 };
    }

    //  исключающее ИЛИ (XOR) - побитовая операция "исключающее ИЛИ"
    gml::type::value XOR(const gml::type::value& a,
                         const gml::type::value& b)
    {
        return { a.u32 ^ b.u32 };
    }

    //  битовое НЕ (NOT) - побитовое отрицание (инверсия) 32-битного значения
    gml::type::value NOT(const gml::type::value& a)
    {
        return { ~a.u32 };
    }

    //  логический сдвиг влево (SHL) - сдвиг битов на 1 позицию влево
    gml::type::value SHL(const gml::type::value& a)
    {
        return { a.u32 << 1 };
    }

    //  логический сдвиг вправо (SHR) - сдвиг битов на 1 позицию вправо
    gml::type::value SHR(const gml::type::value& a)
    {
        return { a.u32 >> 1 };
    }

    //  циклический сдвиг влево (ROL) - биты сдвигаются влево,
    //  при этом старший бит переходит в младшую позицию
    gml::type::value ROL(const gml::type::value& a)
    {
        return { (a.u32 << 1) | (a.u32 >> 31) };
    }

    //  циклический сдвиг вправо (ROR) - биты сдвигаются вправо,
    //  при этом младший бит переходит в старшую позицию
    gml::type::value ROR(const gml::type::value& a)
    {
        return { (a.u32 >> 1) | (a.u32 << 31) };
    }

}

