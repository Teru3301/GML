#include "GML.hpp"
#include <random>



void GML::Init(unsigned int length) {
    DNA.clear();
    if (this->is_init) throw "The model has already been initialized!";
    if (generators.empty()) throw "Impossible to init without genes!";
    if (length == 0)  throw "Impossible to init without genes!";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, generators.size() - 1);

    for (int i = 0; i < length; ++i) {
        size_t random_index = dis(gen);
        DNA.push_back(generators[random_index]());
    }
    for (auto& gen : this->DNA)
        gen->Init();

    this->is_init = true;
}


bool GML::clearDNA() {
    DNA.clear();
    return true;
}


void GML::Run()
{
    if (!this->is_init) throw "The model must be initialized before running!";

    for (auto& gen : this->DNA)
        gen->Run();
}


