#include "GML.hpp"
#include <random>



GML::GML(const GML& other) {
    this->score = other.score;
    this->is_init = other.is_init;
    this->generators = other.generators;
    
    // Глубокое копирование DNA с использованием Clone()
    this->DNA.clear();
    for (const auto& gen : other.DNA) {
        this->DNA.push_back(gen->Clone());
    }
}


void GML::Cross(GML& other) {
    if (!this->is_init || !other.is_init) 
        throw "Both models must be initialized before cross!";
    
    if (this->DNA.size() != other.DNA.size())
        throw "Both models must have the same DNA length for cross!";
    
    // Если DNA слишком короткое для двухточечного скрещивания, используем одноточечное
    if (this->DNA.size() <= 2) {
        // Одноточечное скрещивание как fallback
        size_t cross_point = this->DNA.size() > 1 ? 1 : 0;
        for (size_t i = cross_point; i < this->DNA.size(); i++) {
            std::swap(this->DNA[i], other.DNA[i]);
        }
    } else {
        // Двухточечное скрещивание
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Генерируем две разные точки в диапазоне [1, size-2]
        std::uniform_int_distribution<size_t> dist(1, this->DNA.size() - 2);
        size_t point1 = dist(gen);
        size_t point2 = dist(gen);
        
        // Убеждаемся, что точки разные и point1 < point2
        while (point1 == point2) {
            point2 = dist(gen);
        }
        if (point1 > point2) {
            std::swap(point1, point2);
        }
        
        // Скрещиваем DNA между точками point1 и point2
        for (size_t i = point1; i <= point2; i++) {
            std::swap(this->DNA[i], other.DNA[i]);
        }
    }
    
    // Сбрасываем score для обеих моделей после скрещивания
    this->score = 0;
    other.score = 0;
}

// Остальные методы GML остаются без изменений...
void GML::Mutate(double mut) {
    if (!this->is_init) throw "The model must be initialized before mutation!";
    if (generators.empty()) throw "No generators available for mutation!";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> chance_dist(0.0, 1.0);
    std::uniform_int_distribution<size_t> gen_dist(0, generators.size() - 1);


    double chance = chance_dist(gen);
    //double chance = (double(rand() % 1000) / 1000.0);
    if (chance < mut) {
    for (size_t i = 0; i < this->DNA.size(); i++) {
        int r = rand() % 10;
        if (r == 0) {
            size_t random_index = gen_dist(gen);
            DNA[i] = generators[random_index]();
            DNA[i]->Init(this->DNA.size());
            DNA[i]->Mutate();
        }
        DNA[i]->Mutate();
        
    }
    }
}

void GML::Init(unsigned int length) {
    this->score = 0;
    DNA.clear();
    if (this->is_init) throw "The model has already been initialized!";
    if (generators.empty()) throw "Impossible to init without genes!";
    if (length == 0)  throw "Impossible to init without genes!";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, generators.size() - 1);

    for (int i = 0; i < length; i++) {
        size_t random_index = dis(gen);
        DNA.push_back(generators[random_index]());
    }
    for (int i = 0; i < length; i++) {
        this->DNA[i]->Init(i+1);
    }

    this->is_init = true;
}

bool GML::clearDNA() {
    DNA.clear();
    return true;
}

void GML::Run(std::vector<std::byte>& data) {
    if (!this->is_init) throw "The model must be initialized before running!";
    for (auto& gen : this->DNA)
        gen->Run(data);
}


std::string GML::code()
{
    std::string code = "";
    for (auto& gen : DNA)
    {
        code += " ";
        code += gen->code;
        code += " ";
    }
    return code;
}
