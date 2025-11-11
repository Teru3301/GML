
#include "GML.hpp"

template<typename T>
void GML::AddGen()
{
    // Добавляем фабричную функцию, которая создает новый объект типа T
    this->BaseGenes.push_back([]() -> Gene* { return new T(); });
}

void GML::Init(unsigned int length)
{
    if (this->BaseGenes.size() == 0) throw "Impossible to init without genes!";
    if (length == 0) throw "Impossible to init without genes!";
    
    // Очищаем предыдущие гены, если они есть
    this->Gen.clear();
    
    // Создаем новые объекты через фабрики
    for (int i = 0; i < length; i++)
    {
        int randomIndex = rand() % this->BaseGenes.size();
        Gene* gen = this->BaseGenes[randomIndex](); // Создаем новый объект
        gen->Mutate();
        this->Gen.push_back(gen);
    }
    this->DNA_LENGTH = length;
    this->is_init = true;
}

void GML::Run()
{
    if (this->Gen.size() == 0) throw "Impossible to run without genes!";
    if (this->is_init) for (auto gen : this->Gen) gen->Run();
    else throw "Cannot be started before initialization!";
}

// Явная инстанциация шаблонов для используемых типов
template void GML::AddGen<G1>();
template void GML::AddGen<G2>();


