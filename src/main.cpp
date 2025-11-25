#include "SandardGenes.hpp"
#include "UserGenes.hpp"

#include <cstddef>
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

void print_binary(std::byte b) {
    std::cout << std::bitset<8>(std::to_integer<unsigned>(b)) << '\n';
}

// Функция для оценки части популяции в отдельном потоке
void evaluate_population_segment(
    std::vector<GML>& gml_vec,
    const std::vector<std::vector<std::byte>>& x_train,
    const std::vector<std::byte>& y_train,
    size_t start_idx, 
    size_t end_idx,
    unsigned int train_size,
    unsigned int max_i,
    std::mutex& cout_mutex)
{
    for (size_t i = start_idx; i < end_idx; i++) {
        gml_vec[i].score = 0;
        for (unsigned int j = 0; j < train_size; j++) {
            std::vector<std::byte> inp = x_train[j];
            std::byte out = y_train[j];

            Agent a;
            a.max_iterations = max_i;
            a.inp = inp;

            gml_vec[i].Run(a);
            
            if (a.inp.size() > 0) {
                // Сравниваем каждый бит между полученным и ожидаемым байтом
                std::byte result = a.inp[0] ^ out; // XOR: 0 если биты совпадают, 1 если различаются
                std::bitset<8> bits(std::to_integer<unsigned char>(result));
                // Количество совпавших битов = 8 - количество различных битов
                int matched_bits = 8 - bits.count();
                //gml_vec[i].score += matched_bits;
                //if (gml_vec[i].DNAlen() < 10) gml_vec[i].score -= 10;
                if (a.inp[0] == out) gml_vec[i].score += 8;
            }
        }
    }
}

int main() {
    std::vector<GML> gml_vec;           // Популяция
    unsigned int count_gml = 10000;     // Размер популяции
    unsigned int gml_len = 100;         // Длина ДНК
    unsigned int elite = 100;          // Количество элитных особей
    unsigned int epohs = 100;           // Количество эпох обучения
    unsigned int max_i = 100;           // Количество эпох обучения
    double mut1 = 0.1;                  // Шанс мутации отдельного гена
    double mut2 = 0.01;                 // Шанс замены гена
    int train_size = 5;                 // Количество примеров для обучения

    // Определяем количество потоков
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4; // fallback
    std::cout << "Using " << num_threads << " threads for evaluation" << std::endl;

    gml_vec.reserve(count_gml);
    for (int i = 0; i < count_gml; i++) {
        GML gml;                                // Создание объекта моели
        gml.AddGen<ByteGenes::ANDdd>();         // ...
        gml.AddGen<ByteGenes::ANDnd>();         // ...
        gml.AddGen<ByteGenes::XORdd>();         // ...
        gml.AddGen<ByteGenes::XORnd>();         // ...
        gml.AddGen<ByteGenes::NOT>();           // ...
        gml.AddGen<ByteGenes::ShiftL>();        // ...
        gml.AddGen<ByteGenes::ShiftR>();        // ...
        gml.AddGen<StdGenes::REPL>();           // ...
        gml.AddGen<StdGenes::COPY>();           // ...
        gml.AddGen<StdGenes::SWAP>();           // ...
        gml.AddGen<StdGenes::IF>();             // ...
        gml.AddGen<StdGenes::FI>();             // ...
        gml.AddGen<ByteGenes::EQdd>();          // ...
        gml.AddGen<ByteGenes::MOREdd>();        // ...
        gml.AddGen<ByteGenes::LESSdd>();        // ...
        gml.AddGen<ByteGenes::JUMP>();          // ...
        gml_vec.push_back(gml);                 // Добавление новой особи в популяцию
    }

    for (auto& gml : gml_vec) {
        gml.Init(gml_len);                      // Инициализация модели случайными генами
        gml.Mutate(mut1, mut2);                 // Случайная мутация
    }

    // Создание обучающей выборки
    std::vector<std::vector<std::byte>> x_train;
    std::vector<std::byte>              y_train;
    for (int i = 0; i < train_size; i++) {
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

    // Мьютекс для синхронизации вывода
    std::mutex cout_mutex;

    // Селекция популяции
    for (int ep = 0; ep < epohs; ep++) {
        std::cout << "epoh: " << ep+1;

        // Многопоточная оценка
        std::vector<std::thread> threads;
        size_t segment_size = count_gml / num_threads;
        
        for (unsigned int t = 0; t < num_threads; t++) {
            size_t start_idx = t * segment_size;
            size_t end_idx = (t == num_threads - 1) ? count_gml : (t + 1) * segment_size;
            
            threads.emplace_back(
                evaluate_population_segment, 
                std::ref(gml_vec), 
                std::cref(x_train), 
                std::cref(y_train),
                start_idx, 
                end_idx,
                train_size,
                max_i,
                std::ref(cout_mutex)
            );
        }

        // Ждем завершения всех потоков
        for (auto& thread : threads) {
            thread.join();
        }

        // Сортировка по набранным очкам
        static std::vector<GML*> gml_ptrs;
        gml_ptrs.clear();
        gml_ptrs.reserve(gml_vec.size());
        
        for (auto& gml : gml_vec) {
            gml_ptrs.push_back(&gml);
        }

        // Сортируем указатели
        std::sort(gml_ptrs.begin(), gml_ptrs.end(), 
            [ep](const GML* a, const GML* b) {
                if (ep < 10)
                    return a->score > b->score;  // Высший score = лучше
                else
                    if (a->score != b->score)
                        return a->score > b->score;  // Высший score = лучше
                    else
                        return a->DNAlen() < b->DNAlen();  // Меньшая длина = лучше
            });



        // Пересобираем gml_vec в отсортированном порядке
        std::vector<GML> sorted_vec;
        sorted_vec.reserve(gml_vec.size());
        for (auto* ptr : gml_ptrs) {
            sorted_vec.push_back(std::move(*ptr));
        }
        gml_vec = std::move(sorted_vec);

        int bs = gml_vec.front().score;
        int ms = train_size * 8;
        std::cout << " \tbest score: " << bs << " / " << ms << " \t" << int((double(bs) / double(ms)) * 100) << "%" << std::endl;

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            for (int i = 0; i < gml_vec.size() && i < 20; i++) {
                std::cout << "[" << gml_vec[i].Code().substr(0, 140) << "... ] [" << gml_vec[i].score << "]" << std::endl;
            }
        }


        // Замена и мутация особей не прошедших отбор
        for (int i = elite; i < count_gml; i++)
        {
            GML e1 = gml_vec[rand() % elite];
            e1.Mutate(mut1, mut2);
            gml_vec[i].Cross(e1);
            gml_vec[i].Mutate(mut1, mut2);
            if (i > (count_gml - elite))
                gml_vec[i].Mutate(0.5, mut2);
        }
    }

    // Тестирование лучшей особи
    for (int i = 0; i < x_train.size(); i++) {
        std::vector<std::byte> inp = x_train[i];
        std::byte out = y_train[i];
        Agent a;
        a.max_iterations = max_i;
        a.inp = inp;
        gml_vec[0].Run(a);
        std::cout << "-------" << std::endl;
        print_binary(inp[0]);
        std::cout << "+" << std::endl;
        print_binary(inp[1]);
        std::cout << "=" << std::endl;
        print_binary(a.inp[0]);
        print_binary(out);
    }

    return 0;
}


