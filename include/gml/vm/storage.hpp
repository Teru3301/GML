
#pragma once

#include <vector>
#include "gml/type/base.hpp"


namespace gml::vm::storage
{

    class memory
    {
    private:
        std::vector<gml::type::value> data;
        uint32_t storage_size = 10000;

    public:
        gml::type::value read(const gml::type::value& addr);
        void write(const gml::type::value& addr, const gml::type::value& val);
        void erase(const gml::type::value& addr);

    };


    class stack
    {
    private:
        std::vector<gml::type::value> data;
        uint32_t storage_size = 10000;

    public:
        void push(const gml::type::value& val);
        void pop();
        void dup();
        void swap();
        gml::type::value peek();

    };


    class reg
    {
    private:
        std::vector<gml::type::value> data;

    public:


    };

}

