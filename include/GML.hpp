#pragma once
#include <memory>
#include <vector>
#include <functional>



// ===============================================================================
//
//  Базовый класс гена
//
//  Виртуальные методы обязаны быть переопределены в классах наследниках.
//
// ===============================================================================
class BaseGen {
public:
    virtual ~BaseGen() = default;
    virtual void Init() = 0;
    virtual void Mutate() = 0;
    virtual void Run() = 0;
};



// ===============================================================================
//
//  Класс модели
//
//  AddGen(Usergen) - Добавляет новый ген в геновонд
//  Init(length)    - Создаёт список генов случайных зараннее переданных генов
//                    длины length. Не может быть выполнено если не было передано
//                    ни одного наследника класса BaseGen.
//  Run()           - Последовательно выполняет каждый ген из цепочки полученной
//                    после инициализации. Не может быть выполнено до 
//                    инициализации.
//
// ===============================================================================
class GML {
public:
    template<typename T>
    void AddGen() {
        static_assert(std::is_base_of<BaseGen, T>::value, 
                     "T must be derived from BaseGen");
        
        generators.push_back([]() -> std::unique_ptr<BaseGen> {
            return std::make_unique<T>();
        });
    }

    void Init(unsigned int length);
    void Run();


private:
    std::vector<std::function<std::unique_ptr<BaseGen>()>> generators;
    std::vector<std::unique_ptr<BaseGen>> DNA;

    bool clearDNA();
    bool is_init = false;
};
