// 2022/5/22 10:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/ParserTAP.h"
#pragma warning(push, 0)
    #include <string>
    #include <vector>
#pragma warning(pop)


struct ProgramBASIC
{
    void Clear() { lines.clear(); }

    std::vector<LineBASIC> lines;
};


struct SymbolASM
{
    SymbolASM(pchar symbol) : string(symbol) { }

    SymbolASM(std::string symbol) : string(symbol) { }

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
    void Clear() { lines.clear(); }

    std::vector<LineASM> lines;

    std::vector<LineASM> bad_lines;
};
