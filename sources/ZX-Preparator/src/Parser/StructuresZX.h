// 2026/10/29 09:03:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


/*
    Структуры, употребляемые во всех типах файлов
*/


struct SymbolASM
{
    SymbolASM(pchar symbol) : string(symbol)
    {}

    SymbolASM(std::string symbol) : string(symbol)
    {}

    std::string string;
};


struct LineASM
{
    LineASM(std::string &mnemonic)
    {
        symbols.emplace_back(SymbolASM(mnemonic));
    }

    std::vector<SymbolASM> symbols;

    std::vector<uint8> codes;

    int address = -1;
};


struct ProgramASM
{
    void Clear()
    {
        lines.clear();
    }

    std::vector<LineASM> lines;

    std::vector<LineASM> bad_lines;
};


// Описывает элемент в строке
struct SymbolBASIC
{
    SymbolBASIC(const char *symbol) : string(symbol)
    {}

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
    void Clear()
    {
        lines.clear();
    }

    std::vector<LineBASIC> lines;
};


struct DumpHEX
{
    void Clear()
    {
        bytes.clear();
    }

    void FillFrom(pchar name_file);

    std::vector<uint8> bytes;
};
