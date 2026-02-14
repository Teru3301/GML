
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <functional>



namespace gml::type
{

    union value
    {
        std::uint32_t   u32;
        std::int32_t    i32;
        float           f32;
        struct {
            bool        b : 1;
        };
        std::byte       byte[4];
        char            c;
    };

}

