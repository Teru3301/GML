
#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <thread>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>

#include "gml/vm/vm.hpp"



namespace gml::population
{
    struct example
    {
        gml::vm::storage::memory input;
        gml::vm::storage::memory output;
        gml::vm::storage::stack stack;
        gml::vm::storage::reg registr;
    };


    uint32_t fitness(gml::vm::vm& vm, const std::vector<example>& dataset)
    {
        uint32_t score = 0;

        for (const auto& d : dataset)
        {
            vm.run(d.input.data);

            const auto& expected_out = d.output.data;
            const auto& actual_out   = vm.output.data;

            size_t n = std::min(expected_out.size(), actual_out.size());

            for (size_t i = 0; i < n; ++i)
            {
                uint32_t expected = expected_out[i].u32;
                uint32_t actual   = actual_out[i].u32;

                uint32_t diff = expected ^ actual;

                diff = diff - ((diff >> 1) & 0x55555555);
                diff = (diff & 0x33333333) + ((diff >> 2) & 0x33333333);
                diff = (diff + (diff >> 4)) & 0x0F0F0F0F;
                diff = diff + (diff >> 8);
                diff = diff + (diff >> 16);
                uint32_t different_bits = diff & 0x3F;

                score += 32 - different_bits;
            }
        }

        uint32_t program_len = 100;
        if (vm.program.size() > program_len) score -= (vm.program.size() - program_len);

        return score;
    }


    class population
    {
    private:

        gml::vm::vm tournament_select(uint32_t tournament_size)
        {
            thread_local std::minstd_rand rng(std::random_device{}());
            std::uniform_int_distribution<uint32_t> dist(0, members.size() - 1);

            gml::vm::vm* best = nullptr;

            for (uint32_t i = 0; i < tournament_size; ++i)
            {
                uint32_t idx = dist(rng);
                gml::vm::vm& candidate = members[idx];

                if (!best || candidate.score > best->score)
                {
                    best = &candidate;
                }
            }

            return *best;
        }

        gml::vm::vm crossover(const gml::vm::vm& a, const gml::vm::vm& b)
        {
            thread_local std::minstd_rand rng(std::random_device{}());

            const auto& pa = a.program;
            const auto& pb = b.program;

            if (pa.empty() || pb.empty()) return b;

            std::uniform_int_distribution<size_t> distA(0, pa.size() - 1);
            std::uniform_int_distribution<size_t> distB(0, pb.size() - 1);

            size_t a1 = distA(rng);
            size_t a2 = distA(rng);
            if (a1 > a2) std::swap(a1, a2);
            a2++;

            size_t b1 = distB(rng);
            size_t b2 = distB(rng);
            if (b1 > b2) std::swap(b1, b2);
            b2++;

            gml::vm::vm child = b;

            std::vector<gml::type::value> new_program;
            new_program.reserve(
                pb.size() - (b2 - b1) + (a2 - a1)
            );

            new_program.insert(new_program.end(), pb.begin(), pb.begin() + b1);
            new_program.insert(new_program.end(), pa.begin() + a1, pa.begin() + a2);
            new_program.insert(new_program.end(), pb.begin() + b2, pb.end());

            child.program = std::move(new_program);

            return child;
        }


    public:
        std::vector<gml::vm::vm> members;

        population(
            uint32_t size,
            uint32_t input_size,
            uint32_t output_size,
            uint32_t stack_size,
            uint32_t program_size,
            uint32_t max_steps,
            std::vector<std::function<void(gml::vm::vm&, gml::type::value&)>> instructions
        )
        {
            for (uint32_t i = 0; i < size; i++)
            {
                gml::vm::vm m(
                    program_size, max_steps,
                    input_size, output_size, stack_size,
                    instructions
                );
                m.mutate(1, -1);
                this->members.push_back(m);
            }
        }


        gml::vm::vm best()
        {
            return this->members[0];
        }


        uint32_t evaluate(const std::vector<example>& dataset, uint32_t threads = 1)
        {
            if (threads <= 1)
            {
                uint32_t best_score = 0;

                for (auto& member : this->members)
                {
                    uint32_t s = fitness(member, dataset);
                    member.score = s;

                    if (s > best_score)
                        best_score = s;
                }

                return best_score;
            }

            std::vector<std::thread> workers;
            std::vector<uint32_t> local_best(threads, 0);

            uint32_t total = members.size();
            uint32_t chunk = total / threads;
            uint32_t remainder = total % threads;

            uint32_t start = 0;

            for (uint32_t t = 0; t < threads; ++t)
            {
                uint32_t end = start + chunk + (t < remainder ? 1 : 0);

                workers.emplace_back([this, &dataset, start, end, &local_best, t]()
                {
                    uint32_t best_score = 0;

                    for (uint32_t i = start; i < end; ++i)
                    {
                        uint32_t s = fitness(this->members[i], dataset);
                        this->members[i].score = s;

                        if (s > best_score)
                            best_score = s;
                    }

                    local_best[t] = best_score;
                });

                start = end;
            }

            for (auto& w : workers)
                w.join();

            uint32_t best_score = 0;
            for (auto s : local_best)
                if (s > best_score)
                    best_score = s;

            return best_score;
        }

        void sort_by_score()
        {
            std::sort(members.begin(), members.end(),
                [](const gml::vm::vm& a, const gml::vm::vm& b)
                {
                    return a.score > b.score;
                });
        }


        void next_generation(uint32_t elite_count, float mutation_rate)
        {
            sort_by_score();

            std::vector<gml::vm::vm> new_members;

            for (uint32_t i = 0; i < elite_count && i < members.size(); ++i)
            {
                new_members.push_back(members[i]);
            }

            while (new_members.size() < members.size())
            {
                gml::vm::vm p1 = tournament_select(3);
                gml::vm::vm p2 = tournament_select(3);

                gml::vm::vm child = crossover(p1, p2);

                child.mutate(mutation_rate, -1);

                new_members.push_back(child);
            }

            members = std::move(new_members);
        }


        void train(
                uint32_t epochs, 
                uint32_t elite, float mutation_rate, 
                const std::vector<example>& dataset, 
                uint32_t max_score,
                double min_precent = 100.0,
                uint32_t threads = 1
                )
        {
            uint32_t t0 = time(nullptr);    // начало всего обучения
            uint32_t t1 = t0;
            uint32_t t2 = t0;
            std::cout << "------------------------------------" << std::endl;
            for (int ep = 0; ep < epochs; ep++)
            {
                std::cout << "epoch: " << ep+1 << std::endl;
                t1 = time(nullptr);         //  начало эпохи
                this->evaluate(dataset, threads);
                this->next_generation(elite, mutation_rate);
                t2 = time(nullptr);         // конец эпохи
                std::cout << "time:  \t" << t2 - t1 << "s." << std::endl;
                gml::vm::vm individ = this->best();
                double percent = static_cast<double>(individ.score) / static_cast<double>(max_score) * 100.0;
                std::cout << "score: \t" << std::setprecision(2) << std::fixed
                    << individ.score << "/" << max_score << " \t "
                    << percent << "%" << std::endl;
                std::cout << "------------------------------------" << std::endl;
                if (percent >= min_precent)
                {
                    std::cout << "epochs 1-" << ep << " \t time: " << t2 - t0 << "s.";
                    break;
                }
            }
            std::cout << "epochs 1-" << epochs << " \t time: " << t2 - t0 << "s.";
            std::cout << std::endl;
        }

    };
}

