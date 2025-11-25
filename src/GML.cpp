#include "GML.hpp"
#include <random>
#include <stdexcept>



GML::GML(const GML& other) {
    this->score = other.score;
    this->is_init = other.is_init;
    this->generators = other.generators;
    
    this->DNA.clear();
    for (const auto& gen : other.DNA) {
        this->DNA.push_back(gen->Clone());
    }
}


/*
// Равномерный кросенговер
void GML::Cross(GML& other) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (size_t i = 0; i < this->DNA.size(); i++) {
        if (dist(gen) < 0.5) {
            std::swap(this->DNA[i], other.DNA[i]);
        }
    }
}
*/



void GML::Cross(GML& other) {
    if (!this->is_init || !other.is_init) 
        throw std::runtime_error("Both models must be initialized before cross!");

    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Создаем копии текущих ДНК
    std::vector<std::unique_ptr<BaseGen>> dna1_copy, dna2_copy;
    
    // Глубокое копирование this->DNA
    for (const auto& gen : this->DNA) {
        dna1_copy.push_back(gen->Clone());
    }
    
    // Глубокое копирование other.DNA  
    for (const auto& gen : other.DNA) {
        dna2_copy.push_back(gen->Clone());
    }
    
    // Выбираем точки для первого родителя (this)
    std::uniform_int_distribution<size_t> dist1(0, dna1_copy.size());
    size_t point11 = dist1(gen);
    size_t point12 = dist1(gen);
    
    // Упорядочиваем точки для первого родителя
    if (point11 > point12) std::swap(point11, point12);
    
    // Выбираем точки для второго родителя (other)  
    std::uniform_int_distribution<size_t> dist2(0, dna2_copy.size());
    size_t point21 = dist2(gen);
    size_t point22 = dist2(gen);
    
    // Упорядочиваем точки для второго родителя
    if (point21 > point22) std::swap(point21, point22);

    // Создаем новых потомков
    std::vector<std::unique_ptr<BaseGen>> new_dna1, new_dna2;
    
    // Потомок 1: [0, point11) от parent1 + [point21, point22) от parent2 + [point12, end) от parent1
    for (size_t i = 0; i < point11; i++)
        new_dna1.push_back(dna1_copy[i]->Clone());
    for (size_t i = point21; i < point22; i++)
        new_dna1.push_back(dna2_copy[i]->Clone());
    for (size_t i = point12; i < dna1_copy.size(); i++)
        new_dna1.push_back(dna1_copy[i]->Clone());

    // Потомок 2: [0, point21) от parent2 + [point11, point12) от parent1 + [point22, end) от parent2
    for (size_t i = 0; i < point21; i++)
        new_dna2.push_back(dna2_copy[i]->Clone());
    for (size_t i = point11; i < point12; i++)
        new_dna2.push_back(dna1_copy[i]->Clone());
    for (size_t i = point22; i < dna2_copy.size(); i++)
        new_dna2.push_back(dna2_copy[i]->Clone());

    // Заменяем оригинальные ДНК
    this->DNA = std::move(new_dna1);
    other.DNA = std::move(new_dna2);
    
    // Обновляем max_index для всех генов в новых ДНК
    for (auto& gen : this->DNA) {
        gen->Init(this->DNA.size());
    }
    for (auto& gen : other.DNA) {
        gen->Init(other.DNA.size());
    }

    this->score = 0;
    other.score = 0;
}







void GML::Mutate(double mut1, double mut2) {
    if (!this->is_init) throw std::runtime_error("The model must be initialized before running!");
    if (generators.empty()) throw std::runtime_error("No generators available for mutation!");
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> chance_dist(0.0, 1.0);
    std::uniform_int_distribution<size_t> gen_dist(0, generators.size() - 1);

    for (size_t i = 0; i < this->DNA.size(); i++) {
        double chance = chance_dist(gen);
        //double chance = double(double(rand() % 1000) / 1000.0);
        if (chance <= mut1) {
            chance = chance_dist(gen);
            //chance = double(double(rand() % 1000) / 1000.0);
            if (chance < mut2) {
                size_t random_index = gen_dist(gen);
                //size_t random_index = rand() % (generators.size() - 1);
                DNA[i] = generators[random_index]();
                DNA[i]->Init(this->DNA.size());
            }
            DNA[i]->Mutate();
        }
    }
}


void GML::Init(unsigned int length) {
    this->score = 0;
    DNA.clear();
    if (this->is_init) throw std::runtime_error("The model has already been initialized!");
    if (generators.empty()) throw std::runtime_error("Impossible to init without genes!");
    if (length == 0)  throw std::runtime_error("Impossible to init without genes!");
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, generators.size() - 1);

    for (int i = 0; i < length; i++) {
        size_t random_index = dis(gen);
        DNA.push_back(generators[random_index]());
    }
    for (int i = 0; i < length; i++) {
        this->DNA[i]->Init(length);
    }

    this->is_init = true;
}


bool GML::clearDNA() {
    DNA.clear();
    return true;
}


void GML::Run(Agent& agent) {
    if (!this->is_init) throw std::runtime_error("The model must be initialized before running!");

    unsigned it = 0;
    while (agent.exec)
    {
        if (agent.exec_index >= this->DNA.size()) break;
        if (it > agent.max_iterations) break;
        if (!agent.find_fi || this->DNA[agent.exec_index]->GetCode() == "))")
            this->DNA[agent.exec_index]->Run(agent);
        agent.exec_index++;
        it++;
    }
}


std::string GML::Code()
{
    std::string code = "";
    for (auto& gen : DNA)
    {
        code += " ";
        code += gen->GetCode();
        code += " ";
    }
    return code;
}



GML& GML::operator=(const GML& other) {
    if (this != &other) {  // Защита от самоприсваивания
        // Создаем временный объект
        GML temp(other);
        
        // Обмениваем содержимое текущего объекта с временным
        using std::swap;
        swap(score, temp.score);
        swap(is_init, temp.is_init);
        swap(generators, temp.generators);
        swap(DNA, temp.DNA);
    }
    return *this;
}


unsigned int GML::DNAlen() const
{
    return this->DNA.size();
}

