
#include "Genes.hpp"
#include <vector>
#include <functional>



class GML
{
public:
    template<typename T>
    void AddGen();                         // добавление типа гена
    void Init(unsigned int length);        // инициализация модели
    void Run();                            // запускает выполнение модели
    int  Fit();                            // возвращает коэффициент приспособленности модели

private:
    unsigned int DNA_LENGTH;
    std::vector<std::function<Gene*()>> BaseGenes;  // фабрики генов
    std::vector<Gene*> Gen;                // копии объектов
    bool is_init = false;
};
