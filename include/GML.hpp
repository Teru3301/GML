#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <type_traits>
#include <string>

constexpr int MAX_BYTE = 255;
constexpr int MAX_BYTE_LEN = 8;



// ===============================================================================
//
//  Базовый класс гена
//
// ===============================================================================

class BaseGen {
public:
    virtual ~BaseGen() = default;
    virtual void Init(unsigned int max_index) = 0;
    virtual void Mutate() = 0;
    virtual void Run(std::vector<std::byte>& data) = 0;
    virtual std::unique_ptr<BaseGen> Clone() const = 0;
    const std::string& GetCode() const { return code; }
protected: 
    int max_index;
    bool is_init = false;
    std::vector<int> params;
    std::string code = "--";
};

// CRTP класс для клонирования
template<typename Derived>
class Cloneable : public BaseGen {
public:
    std::unique_ptr<BaseGen> Clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
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
    void AddGen()
    {
        static_assert(std::is_base_of<BaseGen, T>::value, 
                     "T must be derived from BaseGen");
        
        generators.push_back([]() -> std::unique_ptr<BaseGen>
        {
            return std::make_unique<T>();
        });
    }

    GML (){};
    GML(const GML& other); 

    void Mutate(double mut);
    void Cross(GML& other);
    void Init(unsigned int length);
    void Run(std::vector<std::byte>& data);
    int score;
    std::string code();

private:
    std::vector<std::function<std::unique_ptr<BaseGen>()>> generators;
    std::vector<std::unique_ptr<BaseGen>> DNA;

    bool clearDNA();
    bool is_init = false;
};



