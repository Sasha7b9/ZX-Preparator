// 2022/5/22 10:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <string>
#include <vector>


struct DescriptionTAP
{
    void Clear();

    bool valid = false;

    int numberBlocks = 0;

    struct DescriptionBlock
    {
        DescriptionBlock(uint8 t, uint16 s) : type(t), size(s) {}
        uint8 type = 0;
        uint16 size = 0;
    };

    std::vector<DescriptionBlock> blocks;
};


// ��������� ������� � ������
struct SymbolBASIC
{
    SymbolBASIC(const char* symbol) : string(symbol) { }

    std::string string;
};


// ��������� ������
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
