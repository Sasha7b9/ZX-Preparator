// 2026/11/02 09:44:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Parser/Parser.h"


struct BlockTAP : public Block
{
    BlockTAP(wxDataInputStream &stream) : Block(stream)
    {
    }

    static BlockTAP *ReadHeaderBlock(wxDataInputStream &);
    static BlockTAP *ReadDataBlock(wxDataInputStream &, BlockTAP *header);

    bool IsHeader() const;
    bool IsBASIC() const;
    bool IsArrayNumber() const;
    bool IsArrayCharacter() const;
    bool IsCode() const;
    bool IsPureBytes() const;
    bool IsUndefined() const;

    // Распарсить считанные данные в массив lines
    bool ParseBASIC(ProgramBASIC &);

    // Parse machine ocde block to asm program
    bool ParseASM(ProgramASM &);

    char file_name[11];

    uint16 param1 = 0;
    uint16 param2 = 0;

    struct Data
    {
        std::vector<uint8> data;

    } data;

    struct Type
    {
        enum E
        {
            BASIC_PROGRAM,
            NUMBER_ARRAY,
            CHARACTER_ARRAY,
            CODE,
            PURE_BYTES,
            Undefined = 255
        };
    };

    Type::E type = Type::Undefined;
};


struct BlockTAP_Header : public BlockTAP
{

};


struct BlockTAP_BASIC : public BlockTAP
{

};


struct BlockTAP_NumberArray : public BlockTAP
{

};


struct BlockTAP_CharacterArray : public BlockTAP
{

};


struct BlockTAP_Code : public BlockTAP
{

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
