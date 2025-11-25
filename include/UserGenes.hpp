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

    class ORdd : public byte_gene_template<ORdd> 
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

        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0] && agent.inp.size() > this->params[1])
                    agent.inp.push_back(agent.inp[this->params[0]] | agent.inp[this->params[1]]);
        }
    };

    class ORnd : public byte_gene_template<ORnd> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0])
                    agent.inp.push_back(std::byte{static_cast<unsigned char>(this->params[1])} | agent.inp[this->params[0]]);
        }
    };


    //  AND

    class ANDdd : public byte_gene_template<ANDdd> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0] && agent.inp.size() > this->params[1])
                    agent.inp.push_back(agent.inp[this->params[0]] & agent.inp[this->params[1]]);
        }
    };

    class ANDnd : public byte_gene_template<ANDnd> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0])
                    agent.inp.push_back(std::byte{static_cast<unsigned char>(this->params[1])} & agent.inp[this->params[0]]);
        }
    };


    //  XOR

    class XORdd : public byte_gene_template<XORdd> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0] && agent.inp.size() > this->params[1])
                    agent.inp.push_back(agent.inp[this->params[0]] ^ agent.inp[this->params[1]]);
        }
    };

    class XORnd : public byte_gene_template<XORnd> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0])
                    agent.inp.push_back(std::byte{static_cast<unsigned char>(this->params[1])} ^ agent.inp[this->params[0]]);
        }
    };


    //  NOT

    class NOT : public byte_gene_template<NOT> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 1)
                if (agent.inp.size() > this->params[0])
                    agent.inp.push_back(~agent.inp[this->params[0]]);
        }
    };


    //  SHIFT

    class ShiftL : public byte_gene_template<ShiftL> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0])
                    agent.inp.push_back(agent.inp[this->params[0]] << this->params[1]);
        }
    };

    class ShiftR : public byte_gene_template<ShiftR> 
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
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0])
                    agent.inp.push_back(agent.inp[this->params[0]] >> this->params[1]);
        }
    };


    // CONDITIONS
    
    class EQdd : public byte_gene_template<EQdd> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DD;
            Mutate();
            this->code = "=?";
        }

        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0] && agent.inp.size() > this->params[1])
                    agent.AddPredicat(agent.inp[this->params[0]] == agent.inp[this->params[1]]);
        }
    };

    class MOREdd : public byte_gene_template<MOREdd> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DD;
            Mutate();
            this->code = ">?";
        }

        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0] && agent.inp.size() > this->params[1])
                    agent.AddPredicat(agent.inp[this->params[0]] > agent.inp[this->params[1]]);
        }
    };

    class LESSdd: public byte_gene_template<LESSdd> 
    {
    public:
        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::DD;
            Mutate();
            this->code = "<?";
        }

        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 2)
                if (agent.inp.size() > this->params[0] && agent.inp.size() > this->params[1])
                    agent.AddPredicat(agent.inp[this->params[0]] == agent.inp[this->params[1]]);
        }
    };


    class JUMP : public byte_gene_template<JUMP> 
    {
    public:
        void Mutate() override 
        {
            if (this->is_init)
            {
                this->params.clear();
                unsigned int p = -1;
                while (p < 0)
                    p = this->max_index - (rand() % (this->max_index * 2));
                this->params.push_back(p);
            }
        }

        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            this->type = GEN_TYPE::D;
            Mutate();
            this->code = "JP";
        }
        
        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 1)
                if (agent.inp.size() > this->params[0])
                    if (agent.exec_index + params[0] >= 0)
                        agent.exec_index += params[0];
        }
    };
}



