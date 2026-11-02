// 2026/11/02 09:44:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Parser/Parser.h"


struct BlockTAP_Code;
struct BlockTAP_BASIC;
struct BlockTAP_ArrayNumber;
struct BlockTAP_ArrayCharacter;
struct BlockTAP_PureBytes;
struct BlockTAP_Header;
struct BlockTAP_Undefined;


struct BlockTAP : public Block
{
    BlockTAP(wxDataInputStream &stream) : Block(stream)
    {
    }

    virtual Block *Read()
    {
        return nullptr;
    }

    static BlockTAP_Header *ReadHeaderBlock(wxDataInputStream &);
    static BlockTAP *ReadDataBlock(wxDataInputStream &, BlockTAP_Header *);

    BlockTAP_Header *ToHeader() const;
    BlockTAP_BASIC *ToBASIC() const;
    BlockTAP_ArrayNumber *ToArrayNumber() const;
    BlockTAP_ArrayCharacter *ToArrayCharacter() const;
    BlockTAP_Code *ToCode() const;
    BlockTAP_PureBytes *ToPureBytes() const;
    BlockTAP_Undefined *ToUndefined() const;

    // Распарсить считанные данные в массив lines
    bool ParseBASIC(ProgramBASIC &);

    // Parse machine ocde block to asm program
    bool ParseASM(ProgramASM &);

    char file_name[11];

    uint16 param1 = 0;
    uint16 param2 = 0;

    bool valid = false;

    int GetSize() const;

    struct Data
    {
        void Resize(int size)
        {
            data.resize(size);
        }

        uint8 *BeginData()
        {
            return data.data();
        }

    private:

        std::vector<uint8> data;

    } data;

    struct Type
    {
        enum E
        {
            BASIC_PROGRAM,
            ARRAY_NUMBER,
            ARRAY_CHARACTER,
            CODE,
            PURE_BYTES,
            Undefined = 255
        };
    };

    Type::E type = Type::Undefined;
};


struct BlockTAP_Header : public BlockTAP
{
    BlockTAP_Header(wxDataInputStream &stream) : BlockTAP(stream) { }

    int size_data = 0;      // Это размер данных, на которые указывает данный заголовок
};


struct BlockTAP_BASIC : public BlockTAP
{
    BlockTAP_BASIC(wxDataInputStream &stream) : BlockTAP(stream) { }

    // С этой строки программа должна запуститься автоматически после загрузки
    int NumberLineStart() const
    {
        return param1;
    }

    // Смещение адреса переменных относительно начала программы
    int OffsetVariables() const
    {
        return param2;
    }
};


struct BlockTAP_ArrayNumber : public BlockTAP
{
    BlockTAP_ArrayNumber(wxDataInputStream &stream) : BlockTAP(stream) { }

    char NameVariable() const
    {
        return (char)(param1 >> 8);
    }
};


struct BlockTAP_ArrayCharacter : public BlockTAP
{
    BlockTAP_ArrayCharacter(wxDataInputStream &stream) : BlockTAP(stream) { }

    char NameVariable() const
    {
        return (char)(param1 >> 8);
    }
};


struct BlockTAP_Code : public BlockTAP
{
    BlockTAP_Code(wxDataInputStream &stream) : BlockTAP(stream) { }

    // Адрес загрузки
    int StartAddressCode() const
    {
        return param1;
    }
};


struct BlockTAP_PureBytes : public BlockTAP
{

};


struct BlockTAP_Undefined : public BlockTAP
{

};


struct FileTAP : public File
{
};


class ParserTAP
{
public:

    bool Execute(pchar fileName);

    std::shared_ptr<FileTAP> file;
};
