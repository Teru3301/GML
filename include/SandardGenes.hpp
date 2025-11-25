#pragma once
#include "GML.hpp"



namespace StdGenes
{
    //  REPLACE

    class REPL : public Cloneable<REPL>
    {
    public:
        REPL() = default;
        REPL(const REPL& other)
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

        void Run(Agent& agent) override 
        {
            if (this->is_init && this->params.size() >= 1)
                if (agent.inp.size() > this->params[0])
                    agent.inp.erase(agent.inp.begin() + this->params[0]);
        }
    };


    //  SKIP

    class SKIP : public Cloneable<SKIP> 
    {
    public:
        SKIP() = default;
        SKIP(const SKIP& other) 
        {
            this->code = other.code;
        }
        void Mutate() override {}

        void Init(unsigned int max_index) override 
        {
            this->is_init = true;
            this->code = "SK";
        }

        void Run(Agent& agent) override {}
    };


    // COPY 
    
    class COPY : public Cloneable<COPY>
    {
    public:
        COPY() = default;
        COPY(const COPY& other)
        {
            this->code = other.code;
            this->params = other.params;
        }
        void Mutate() override
        {
            if (this->is_init)
            {
                this->params.clear();
                this->params.push_back(rand() % this->max_index);
                this->params.push_back(rand() % this->max_index);
            }
        }

        void Init(unsigned int max_index) override 
        {
            this->is_init = true;
            this->max_index = max_index;
            this->code = "CP";
            Mutate();
        }

        void Run(Agent& agent) override 
        {
            if (this->is_init)
                if (this->params[0] < agent.inp.size() && this->params[1] < agent.inp.size())
                    agent.inp[params[0]] = agent.inp[params[1]];
        }
    };


    // SWAP

    class SWAP : public Cloneable<SWAP>
    {
    public:
        SWAP() = default;
        SWAP(const SWAP& other)
        {
            this->code = other.code;
            this->params = other.params;
        }
        void Mutate() override
        {
            if (this->is_init)
            {
                this->params.clear();
                this->params.push_back(rand() % this->max_index);
                this->params.push_back(rand() % this->max_index);
            }
        }

        void Init(unsigned int max_index) override 
        {
            this->is_init = true;
            this->max_index = max_index;
            this->code = "SW";
            Mutate();
        }

        void Run(Agent& agent) override 
        {
            if (this->is_init)
                if (this->params[0] < agent.inp.size() && this->params[1] < agent.inp.size())
                    std::swap(agent.inp[this->params[0]], agent.inp[this->params[1]]);
        }
    };


    // CONDITIONS
    
    class IF : public Cloneable<IF>
    {
    public:
        IF() = default;
        IF(const IF& other)
        {
            this->code = other.code;
        }
        void Mutate() override {}

        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            Mutate();
            this->code = "((";
        }
        
        void Run(Agent& agent) override 
        {
            agent.ConditionStart();
        }
    };

    class FI : public Cloneable<FI>
    {
    public:
        FI() = default;
        FI(const FI& other)
        {
            this->code = other.code;
        }
        void Mutate() override {}

        void Init(unsigned int max_index) override 
        {
            this->max_index = max_index;
            this->is_init = true;
            Mutate();
            this->code = "))";
        }

        void Run(Agent& agent) override 
        {
            agent.ConditionEnd();
        }
    };
}



