#include "UserGenes.hpp"

#include <cstddef>
#include <iostream>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <memory>
#include <list>

void print_binary(std::byte b) {
    std::cout << std::bitset<8>(std::to_integer<unsigned>(b)) << '\n';
}



int main() {
    std::list<GML> gml_vec;  // Популяция геномов
    unsigned int count_gml = 10000;  // Размер популяции
    unsigned int gml_len = 100;      // Исходная длина ДНК
    unsigned int elite = 10;
    unsigned int epohs = 100;
    double mut = 0.05;
    int train_size = 5;

    //  Инициализация популяции
    for (int i = 0; i < count_gml; i++)
    {
        GML gml;                    // Создание объекта моели
        gml.AddGen<gddOR>();        // Добавление пользовательского гена в генофонд
        gml.AddGen<gndOR>();        // Добавление пользовательского гена в генофонд
        gml.AddGen<gddAND>();       // ...
        gml.AddGen<gndAND>();       // ...
        gml.AddGen<gddXOR>();       // ...
        gml.AddGen<gndXOR>();       // ...
        gml.AddGen<gdNOT>();        // ...
        gml.AddGen<gdnSL>();        // ...
        gml.AddGen<gdnSR>();        // ...
        gml.AddGen<gREPL>();        // ...
        gml.AddGen<gSKIP>();        // ...

        gml_vec.push_back(gml);     // Добавление новой особи в популяцию
    }

    for (auto& gml : gml_vec)
    {
        gml.Init(gml_len);          // Инициализация модели случайными генами
        gml.Mutate(mut);               // Случайная мутация
    }

    // Создание обучающей выборки
    std::vector<std::vector<std::byte>> x_train;
    std::vector<std::byte>              y_train;
    for (int i = 0; i < train_size; i++)
    {
        unsigned char n1 = rand() % 128;
        unsigned char n2 = rand() % 128;
        unsigned char n3 = n1 + n2;
        std::byte a = std::byte{n1};
        std::byte b = std::byte{n2};
        std::byte c = std::byte{n3};

        std::vector<std::byte> data;
        data.push_back(a);
        data.push_back(b);

        x_train.push_back(data);
        y_train.push_back(c);
    }


    //  Селекция популяции
    for (int ep = 0; ep < epohs; ep++)
    {
        std::cout << "epoh: " << ep+1;

        // Оценка
        for (auto& gml : gml_vec) {
            for (int i = 0; i < x_train.size(); i++)
            {
                std::vector<std::byte> inp = x_train[i];
                std::byte out = y_train[i];
                gml.Run(inp);
                //gml.score -= inp.size();
                
                if (inp.size() > 0)
                {
                    // Сравниваем каждый бит между полученным и ожидаемым байтом
                    std::byte result = inp[0] ^ out; // XOR: 0 если биты совпадают, 1 если различаются
                    std::bitset<8> bits(std::to_integer<unsigned char>(result));
                    // Количество совпавших битов = 8 - количество различных битов
                    int matched_bits = 8 - bits.count();
                    gml.score += matched_bits;
                }
            }
        }
        
        // Сортировка по набранным очкам
        gml_vec.sort([](const GML& a, const GML& b) {
            return a.score > b.score; 
        });

        int bs = gml_vec.front().score;
        int ms = train_size*8;
        std::cout << " \tbest score: " << bs << " / " << ms << " \t" << int((double(bs) / double(ms)) * 100) << "%" << std::endl;


        int ii = 0;
        for (auto& gml : gml_vec)
        {
            std::cout << "[" << gml.code().substr(0, 140) << "... ] [" << gml.score << "]" << std::endl;
            ii++;
            if (ii > 20) break;
        }


        // Замена и мутация особей не прошедших отбор
        int i = 0;
        for (auto& gml : gml_vec) {
            gml.score = 0;
            if (i > elite) {
                int cross = rand() % 4;
                if (cross != 1) {
                    auto elite_it = gml_vec.begin();
                    std::advance(elite_it, rand() % elite);
                    GML elite_copy = *elite_it; 
                    gml.Cross(elite_copy);
                }
                gml.Mutate(mut);
                if (i > count_gml - elite)
                {
                    gml.Mutate(1);
                }
            }
            i++;
        }
    }


    //  Вывод предсказанного значения и целевого
    for (int i = 0; i < x_train.size(); i++)
    {
        std::vector<std::byte> inp = x_train[i];
        std::byte out              = y_train[i];
        gml_vec.front().Run(inp);
        if (inp.size() > 0)
        {
            if (inp.size() == 1) gml_vec.front().score += 1;
            if (inp[0] == out) gml_vec.front().score += 1;
        }
        std::cout << "-------" << std::endl;
        for (auto v : inp)
        {
            print_binary(v);
            print_binary(out);
            break;
        }
    }

    return 0;
}



