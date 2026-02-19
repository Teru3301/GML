
#pragma once

#include "gml/vm/storage.hpp"



namespace gml::type
{

struct example
    {
        gml::vm::storage::memory input;
        gml::vm::storage::memory output;
        gml::vm::storage::stack stack;
        gml::vm::storage::reg registr;
    };

}

