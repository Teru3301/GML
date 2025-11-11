
#include "BaseGene.hpp"
#include <iostream>



class G1 : public Gene 
{
public:
    void Run() override {
        std::cout << value << std::endl;
    }
    
    void Mutate() override {
        this->value = this->value + " m[" + std::to_string(rand() % 10) + "]";
    }

private:
    std::string value = "G1";
};



class G2 : public Gene 
{
public:
    void Run() override {
        std::cout << value << std::endl;
    }
    
    void Mutate() override {
        this->value = this->value + " m[" + std::to_string(rand() % 10) + "]";
    }

private:
    std::string value = "G2";
};


