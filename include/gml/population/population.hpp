
#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <thread>

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

        return score;
    }


    class population
    {
    private:
        gml::vm::vm tournament_select(uint32_t tournament_size)
        {
            gml::vm::vm* best = nullptr;

            for (uint32_t i = 0; i < tournament_size; ++i)
            {
                uint32_t idx = rand() % members.size();
                gml::vm::vm& candidate = members[idx];

                if (!best || candidate.score > best->score)
                {
                    best = &candidate;
                }
            }

            return *best;
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
                gml::vm::vm parent = tournament_select(3);

                parent.mutate(mutation_rate, -1);

                new_members.push_back(parent);
            }

            members = std::move(new_members);
        }

    };
}

