#pragma once
#include "GML.hpp"
#include <random>
#include <string>


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


class gddOR : public Cloneable<gddOR> {
public:
    gddOR() = default;
    gddOR(const gddOR& other) {
        this->init = other.init;
        this->index1 = other.index1;
        this->index2 = other.index2;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index1 = rand() % this->max_index;
            this->index2 = rand() % this->max_index;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "do";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index1 && data.size() > this->index2)
                data.push_back(data[this->index1] | data[this->index2]);
    }
private:
    bool init = false;
    int index1;
    int index2;
};

class gndOR : public Cloneable<gndOR> {
public:
    gndOR() = default;
    gndOR(const gndOR& other) {
        this->init = other.init;
        this->index = other.index;
        this->number = other.number;
        this->byte = other.byte;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
            this->number = rand() % MAX_BYTE;
            this->byte = std::byte{this->number};
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "no";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.push_back(byte | data[this->index]);
    }
private:
    bool init = false;
    unsigned int index;
    unsigned char number;
    std::byte byte;
};

class gddAND : public Cloneable<gddAND> {
public:
    gddAND() = default;
    gddAND(const gddAND& other) {
        this->init = other.init;
        this->index1 = other.index1;
        this->index2 = other.index2;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index1 = rand() % this->max_index;
            this->index2 = rand() % this->max_index;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "da";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index1 && data.size() > this->index2)
                data.push_back(data[this->index1] & data[this->index2]);
    }
private:
    bool init = false;
    unsigned int index1;
    unsigned int index2;
};

class gndAND : public Cloneable<gndAND> {
public:
    gndAND() = default;
    gndAND(const gndAND& other) {
        this->init = other.init;
        this->index = other.index;
        this->number = other.number;
        this->byte = other.byte;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
            this->number = rand() % MAX_BYTE;
            this->byte = std::byte{this->number};
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "na";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.push_back(byte & data[this->index]);
    }
private:
    bool init = false;
    unsigned int index;
    unsigned char number;
    std::byte byte;
};

class gddXOR : public Cloneable<gddXOR> {
public:
    gddXOR() = default;
    gddXOR(const gddXOR& other) {
        this->init = other.init;
        this->index1 = other.index1;
        this->index2 = other.index2;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index1 = rand() % this->max_index;
            this->index2 = rand() % this->max_index;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "dx";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index1 && data.size() > this->index2)
                data.push_back(data[this->index1] ^ data[this->index2]);
    }
private:
    bool init = false;
    unsigned int index1;
    unsigned int index2;
};

class gndXOR : public Cloneable<gndXOR> {
public:
    gndXOR() = default;
    gndXOR(const gndXOR& other) {
        this->init = other.init;
        this->index = other.index;
        this->number = other.number;
        this->byte = other.byte;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
            this->number = rand() % MAX_BYTE;
            this->byte = std::byte{this->number};
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "nx";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.push_back(byte ^ data[this->index]);
    }
private:
    bool init = false;
    unsigned int index;
    unsigned char number;
    std::byte byte;
};

class gdNOT : public Cloneable<gdNOT> {
public:
    gdNOT() = default;
    gdNOT(const gdNOT& other) {
        this->init = other.init;
        this->index = other.index;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "nt";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.push_back(~data[this->index]);
    }
private:
    bool init = false;
    unsigned int index;
};

class gdnSL : public Cloneable<gdnSL> {
public:
    gdnSL() = default;
    gdnSL(const gdnSL& other) {
        this->init = other.init;
        this->index = other.index;
        this->number = other.number;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
            this->number = rand() % MAX_BYTE_LEN;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "sl";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.push_back(data[this->index] << number);
    }
private:
    bool init = false;
    unsigned int index;
    unsigned int number;
};

class gdnSR : public Cloneable<gdnSR> {
public:
    gdnSR() = default;
    gdnSR(const gdnSR& other) {
        this->init = other.init;
        this->index = other.index;
        this->number = other.number;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
            this->number = rand() % MAX_BYTE_LEN;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "sr";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.push_back(data[this->index] >> number);
    }
private:
    bool init = false;
    unsigned int index;
    unsigned int number;
};

class gREPL : public Cloneable<gREPL> {
public:
    gREPL() = default;
    gREPL(const gREPL& other) {
        this->init = other.init;
        this->index = other.index;
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {
        if (this->init) {
            this->index = rand() % this->max_index;
        }
    }
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        this->init = true;
        Mutate();
        this->code = "re";
    }
    void Run(std::vector<std::byte>& data) override {
        if (this->init)
            if (data.size() > this->index)
                data.erase(data.begin() + this->index);
    }
private:
    bool init = false;
    unsigned int index;
};

class gSKIP : public Cloneable<gSKIP> {
public:
    gSKIP() = default;
    gSKIP(const gSKIP& other) {
        this->max_index = other.max_index;
        this->code = other.code;
    }
    
    void Mutate() override {}
    void Init(unsigned int max_index) override {
        this->max_index = max_index;
        Mutate();
        this->code = "sk";
    }
    void Run(std::vector<std::byte>& data) override {}
};



