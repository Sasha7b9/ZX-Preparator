// 2026/10/29 09:15:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Parser/StructuresZX.h"


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

    void Clear();

    void AppendBlock(std::shared_ptr<Block> block)
    {
        blocks.push_back(std::move(block));
    }

    std::shared_ptr<Block> GetBlock(int index)
    {
        return blocks[(uint)index];
    }

    int NumberBlocks() const
    {
        return (int)blocks.size();
    }

    std::vector<std::shared_ptr<Block>> blocks;

    DumpHEX dump;
};


struct Description
{
    virtual ~Description() = default;

    bool valid = false;

    int full_size = 0;          // Количество байт в исходном файле

    int unused_memory = 0;      // КОличество нераспознанной памяти

    int numberBlocks = 0;
};
