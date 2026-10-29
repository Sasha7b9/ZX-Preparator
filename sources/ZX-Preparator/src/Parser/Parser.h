// 2026/10/29 09:15:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


/*
    Все типы образов состоят из блоков. Эта структура описывает общий интерфейс блоков
*/

struct Block
{
    Block(wxDataInputStream &_stream) : stream(_stream) { }
    virtual ~Block() = default;

    virtual bool Read() = 0;

    wxDataInputStream &stream;
};


struct File
{
    virtual ~File() = default;

    DumpHEX dump;
};
