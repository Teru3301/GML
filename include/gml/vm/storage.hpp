
#pragma once

#include <vector>
#include "gml/type/base.hpp"


namespace gml::vm::storage
{

    class memory
    {
    private:
        bool inited = false;
        uint32_t storage_size = 10000;

    public:
        std::vector<gml::type::value> data;
        gml::type::value read(const gml::type::value& addr);
        void write(const gml::type::value& addr, const gml::type::value& val);
        void erase(const gml::type::value& addr);

        void init(uint32_t storage_size)
        {
            this->data.clear();
            this->storage_size = storage_size;
            this->data.resize(storage_size);
        }
    };


    class stack
    {
    private:

    public:
        uint32_t storage_size = 10000;
        std::vector<gml::type::value> data;
        void push(const gml::type::value& val);
        gml::type::value pop();
        void dup();
        void swap();
        gml::type::value peek();

        void init(uint32_t storage_size)
        {
            this->data.clear();
            this->storage_size = storage_size;
        }

    };


    class reg
    {
    private:

    public:
        std::vector<gml::type::value> data;


    };

}

