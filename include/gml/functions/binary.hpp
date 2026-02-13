
#pragma once

#include "gml/type/base.hpp"


namespace gml::function::binary
{
    gml::type::value OR(const gml::type::value& a, const gml::type::value& b);
    gml::type::value AND(const gml::type::value& a, const gml::type::value& b);
    gml::type::value XOR(const gml::type::value& a, const gml::type::value& b);

    gml::type::value NOT(const gml::type::value& a);
    gml::type::value SHL(const gml::type::value& a);
    gml::type::value SHR(const gml::type::value& a);
    gml::type::value ROL(const gml::type::value& a);
    gml::type::value ROR(const gml::type::value& a);
}

