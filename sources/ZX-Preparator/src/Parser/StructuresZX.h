// 2026/10/29 09:03:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


/*
    Структуры, употребляемые во всех типах файлов
*/


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


struct DumpHEX
{
    void Clear()
    {
        bytes.clear();
    }

    void FillFrom(pchar name_file);

    std::vector<uint8> bytes;
};
