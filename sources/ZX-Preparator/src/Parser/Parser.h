// 2026/10/29 09:15:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Parser/StructuresZX.h"


/*
    Все типы образов состоят из блоков. Эта структура описывает общий интерфейс блоков
*/


struct FileTAP;
struct FileHEX;
struct FileTZX;
struct Writer;


struct Block
{
    Block(wxDataInputStream &_stream) : stream(_stream) { }
    virtual ~Block() = default;

    virtual Block *Read() = 0;

    wxDataInputStream &stream;

    virtual void Write(Writer &, int /*dx*/) { }

    uint8 Read8();
    void Read8(uint8 *, size_t);
    uint16 Read16();
    void Read16(uint16 *, size_t);
    uint Read32();
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

    uint FullSize() const
    {
        return (uint)dump.bytes.size();
    }

    std::vector<std::shared_ptr<Block>> blocks;

    DumpHEX dump;

    bool is_valid = false;

    uint number_unreaded_bytes = 0;

    FileTAP *ToTAP();
    FileHEX *ToHEX();
    FileTZX *ToTZX();
};
