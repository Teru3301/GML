
#pragma once

#include <cstddef>



namespace gml::type
{

    union value
    {
        int i;
        float f;
        struct {
            bool b : 1;
        };
        std::byte byte1;
        std::byte byte2;
        std::byte byte3;
        std::byte byte4;
    };

}

