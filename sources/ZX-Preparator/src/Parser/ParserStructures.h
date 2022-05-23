// 2022/5/22 10:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <string>
#include <vector>


struct DescriptionTAP
{
    void Clear();

    bool valid = false;

    int numberBlocks = 0;

    int unused_memory = 0;      // КОличество нераспознанной памяти

    struct Block
    {
        Block(uint8 t, uint16 s, uint16 p1, uint16 p2) : type(t), size(s), param1(p1), param2(p2) {}
        uint8 type = 0;
        uint16 size = 0;
        uint16 param1 = 0;
        uint16 param2 = 0;
    };

    std::vector<Block> blocks;
};


// Описывает элемент в строке
struct SymbolBASIC
{
    SymbolBASIC(const char* symbol) : string(symbol) { }

    std::string string;
};


// Описывает строку
struct LineBASIC
{
    int number;
    int size;

    std::vector<SymbolBASIC> symbols;
};


struct ProgramBASIC
{
    void Clear() { lines.clear(); }

    std::vector<LineBASIC> lines;
};


struct SymbolASM
{
    SymbolASM(pchar symbol) : string(symbol) { }

    std::string string;
};


struct LineASM
{
    std::vector<SymbolASM> symbols;
};


struct ProgramASM
{
    void Clear() { lines.clear(); }

    std::vector<LineASM> lines;
};
