#pragma once
#include "GML.hpp"


namespace ByteGenes
{
    template<typename Derived>
    class byte_gene_template : public Cloneable<Derived>
    {
    protected:
        enum class GEN_TYPE {
            DD,
            DN,
            D,
            DN_LEN,
            NO
        };
        GEN_TYPE type;
        std::vector<unsigned int> params;

    public: 
        void Mutate() override 
        {
            if (this->is_init)
            {
                this->params.clear();
                switch (this->type)
                {
                    case GEN_TYPE::DD:
                        this->params.push_back(rand() % this->max_index);
                        this->params.push_back(rand() % this->max_index);
                        break;
                    case GEN_TYPE::DN:
                        this->params.push_back(rand() % this->max_index);
                        this->params.push_back(rand() % MAX_BYTE);
                        break;
                    case GEN_TYPE::D:
                        this->params.push_back(rand() % this->max_index);
                        break;
                    case GEN_TYPE::DN_LEN:
                        this->params.push_back(rand() % this->max_index);
                        this->params.push_back(rand() % MAX_BYTE_LEN);
                        break;
                    case GEN_TYPE::NO:
                        break;
                }
            }
        }

        byte_gene_template() = default;
        byte_gene_template(const byte_gene_template& other) 
        {
            this->is_init = other.is_init;
            this->max_index = other.max_index;
            this->code = other.code;
            this->params = other.params;
            this->type = other.type;
        }
    };


    //  OR 

    class gddOR : public byte_gene_template<gddOR> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DD;
            Mutate();
            this->code = "do";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0] && data.size() > this->params[1])
                    data.push_back(data[this->params[0]] | data[this->params[1]]);
        }
    };

    class gndOR : public byte_gene_template<gndOR> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DN;
            Mutate();
            this->code = "no";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0])
                    data.push_back(std::byte{static_cast<unsigned char>(this->params[1])} | data[this->params[0]]);
        }
    };


    //  AND

    class gddAND : public byte_gene_template<gddAND> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DD;
            Mutate();
            this->code = "da";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0] && data.size() > this->params[1])
                    data.push_back(data[this->params[0]] & data[this->params[1]]);
        }
    };

    class gndAND : public byte_gene_template<gndAND> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DN;
            Mutate();
            this->code = "na";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0])
                    data.push_back(std::byte{static_cast<unsigned char>(this->params[1])} & data[this->params[0]]);
        }
    };


    //  XOR

    class gddXOR : public byte_gene_template<gddXOR> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DD;
            Mutate();
            this->code = "dx";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0] && data.size() > this->params[1])
                    data.push_back(data[this->params[0]] ^ data[this->params[1]]);
        }
    };

    class gndXOR : public byte_gene_template<gndXOR> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DN;
            Mutate();
            this->code = "nx";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0])
                    data.push_back(std::byte{static_cast<unsigned char>(this->params[1])} ^ data[this->params[0]]);
        }
    };


    //  NOT

    class gdNOT : public byte_gene_template<gdNOT> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::D;
            Mutate();
            this->code = "nt";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 1)
                if (data.size() > this->params[0])
                    data.push_back(~data[this->params[0]]);
        }
    };


    //  SHIFT

    class gdnSL : public byte_gene_template<gdnSL> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DN_LEN;
            Mutate();
            this->code = "sl";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0])
                    data.push_back(data[this->params[0]] << this->params[1]);
        }
    };

    class gdnSR : public byte_gene_template<gdnSR> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DN_LEN;
            Mutate();
            this->code = "sr";
        }
        
        void Run(std::vector<std::byte>& data) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (data.size() > this->params[0])
                    data.push_back(data[this->params[0]] >> this->params[1]);
        }
    };
}



