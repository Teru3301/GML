
#include "gml/vm/storage.hpp"


namespace gml::vm::storage
{

    //      stack
    void stack::push(const gml::type::value& val)
    {
        if (this->data.size() < this->storage_size)
            this->data.push_back({val});
    }

    void stack::pop()
    {
        if (this->data.size() > 0) this->data.pop_back();
    }

    void stack::dup()
    {
        if (this->data.size() < this->storage_size)
            this->data.push_back(this->data.back());
    }

    void stack::swap()
    {
        gml::type::value tmp1 = this->data.back();
        this->pop();
        gml::type::value tmp2 = this->data.back();
        this->pop();
        this->data.push_back(tmp1);
        this->data.push_back(tmp2);
    }

    gml::type::value stack::peek()
    {
        if (this->data.size() > 0)
            return this->data.back();
        return {};
    }

    //      memory
    gml::type::value memory::read(const gml::type::value& addr)
    {
        uint32_t addres = addr.u32 % this->storage_size;
        return this->data.at(addres);
    }


    void memory::write(const gml::type::value& addr, const gml::type::value& val)
    {
        uint32_t addres = addr.u32 % this->storage_size;
        this->data[addres] = {val};
    }


    void memory::erase(const gml::type::value& addr)
    {
        uint32_t addres = addr.u32 % this->storage_size;
        this->data[addres] = {};
    }

}

