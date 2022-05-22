// 2022/5/22 10:29:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <string>
#include <vector>


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
