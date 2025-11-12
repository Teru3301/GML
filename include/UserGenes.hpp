#pragma once
#include "GML.hpp"
#include <iostream>
#include <string>
#include <random>


// ===============================================================================
//
// Пользовательские гены
//
// Каждый новый пользовательский ген должен переопределять методы:
// - void Init()
// - void Run()
// - void Mutate()
//
// Каждый отдельный пользовательский ген может иметь любые поля и методы,
// но модель всегда будет взаимодействовать только с перечисленными выше методами.
//
// ===============================================================================



class G1: public BaseGen {
public:
    void Init() override {
        Mutate();
    }
    void Run() override {
        std::cout << "[G1] - " << std::to_string(value) << std::endl;
    }
    void Mutate() override {
        value = rand() % 1000;
    }

private:
    int value = 0;
};


class G2: public BaseGen {
public:
    void Init() override {
        Mutate();
    }
    void Run() override {
        std::cout << "[G2] - " << std::to_string(value) << std::endl;
    }
    void Mutate() override {
        value = rand() % 1000;
    }

private:
    int value = 0;
};


class G3: public BaseGen {
public:
    void Init() override {
        Mutate();
    }
    void Run() override {
        std::cout << "[G3] - " << std::to_string(value) << std::endl;
    }
    void Mutate() override {
        value = rand() % 1000;
    }

private:
    int value = 0;
};


