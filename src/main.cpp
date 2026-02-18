
#include <iostream>
#include <random>

#include "gml/vm/instructions.hpp"
#include "gml/vm/interpreter.hpp"
#include "gml/population/population.hpp"



int main()
{
    
    //  создание популяции

    std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instr;
    for (auto i : gml::vm::isa::instr) instr.push_back(i.func);

    gml::population::population p(
        10000,  //  размер популяции
        2,      //  размер входной памяти
        1,      //  размер вызодной памяти
        100,    //  размер стека
        2,      //  начальный размер программы
        100,    //  максимальное количество "ходов" программы
        instr   //  список инструкций
    );

    //  генерация датасета

    thread_local std::minstd_rand rng(std::random_device{}());
    std::uniform_int_distribution<uint32_t> rnd(0, -1);
    std::vector<gml::population::example> dataset;
    for (uint32_t i = 0; i < 1000; i++)
    {
        uint32_t a = rnd(rng);
        uint32_t b = rnd(rng);
        gml::population::example ex;
        ex.input.data = {{a}, {b}};
        ex.output.data = {{a+b}};
        dataset.push_back(ex);
    }
    uint32_t max_score = dataset.size() * dataset[0].output.data.size() * 32;
    std::cout << "max score: " << max_score << std::endl;

    //  обучение

    p.train(
        500,        //  количество эпох
        10,         //  количество элитных особей
        0.01,       //  щанс мутации (для каждой ячейки)
        dataset,    //  датасет
        max_score,  //  максимальное количество очков для датасета
        100.0,      //  балл при достижении которого обучение будет остановлено
        40          //  количество потоков процессора
    );

    gml::vm::vm best_individ = p.best();

    std::cout << std::endl;
    std::cout << best_individ.program.size();
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << gml::interpreter::translate_all(best_individ.program) << std::endl;

    //  ручное тестирование
    
    while (true)
    {
        std::cout << std::endl;
        uint32_t a;
        uint32_t b;
        std::cout << "a="; std::cin >> a;
        std::cout << "b="; std::cin >> b;
        best_individ.run({{a},{b}});
        gml::type::value o = best_individ.output.read({});
        std::cout << a << "+" << b << "=" << o.u32 << " \t | " << a+b << std::endl;
        std::cout << "o: " << gml::interpreter::b_to_s(o) << std::endl;
        std::cout << "t: " << gml::interpreter::b_to_s({a+b}) << std::endl;
    }


    return 0;
}

