#pragma once
#include "GML.hpp"


namespace StdGenes
{
    //  REPLACE

    class gREPL : public Cloneable<gREPL>
    {
    public:
        gREPL() = default;
        gREPL(const gREPL& other)
        {
            this->is_init = other.is_init;
            this->params = other.params;
            this->max_index = other.max_index;
            this->code = other.code;
        }

        void Mutate() override 
        {
            if (this->is_init) 
            {
                this->params.clear();
                this->params.push_back(rand() % this->max_index);
            }
        }
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            Mutate();
            this->code = "RE";
        }

        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 1)
                if (data.size() > this->params[0])
                    data.erase(data.begin() + this->params[0]);
        }
    };


    //  SKIP

    class gSKIP : public Cloneable<gSKIP> 
    {
    public:
        gSKIP() = default;
        gSKIP(const gSKIP& other) 
        {
            this->max_index = other.max_index;
            this->code = other.code;
        }
        void Mutate() override {}

        void Init(unsigned int max_index) override 
        {
            this->is_init = true;
            this->code = "SK";
        }

        void Run(std::vector<std::byte>& data) override {}
    };
}



