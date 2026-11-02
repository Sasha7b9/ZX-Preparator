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
    BlockTAP() : Block()
    {
    }

    virtual Block *Read()
    {
        return nullptr;
    }

    static BlockTAP *ReadDataBlock(wxDataInputStream &, BlockTAP_Header *);

    BlockTAP_Header *ToHeader() const;
    BlockTAP_BASIC *ToBASIC() const;
    BlockTAP_ArrayNumber *ToArrayNumber() const;
    BlockTAP_ArrayCharacter *ToArrayCharacter() const;
    BlockTAP_Code *ToCode() const;
    BlockTAP_PureBytes *ToPureBytes() const;
    BlockTAP_Undefined *ToUndefined() const;

    char file_name[11];

    uint16 param1 = 0;      // program    00 : number line start (or a number >= 32768 if no line parameter was given)
                            // arrays 01, 02 : variable name
                            // code       03 : holds the start of the code block when saved

    uint16 param2 = 0;      // program    00 : holds the start of the variable area relative to the start of the program
                            // arrays 01, 02 : N/A
                            // code       03 : holds 32768

    int GetSize() const
    {
        return data.GetSize();
    }

    struct Data
    {
        void Push16(uint16);

        void AppendBytes(wxDataInputStream &, int size);

        uint8 *BeginData()
        {
            return data.data();
        }

        int GetSize() const
        {
            return (int)data.size();
        }

        void ReadBytes(uint8 *, int size);

        uint16 Read16Reverse();

        uint16 Read16();

        uint8 Read8();

        char ReadChar();

        bool IsEmpty() const;

        // Осталось байт для чтения
        int BytesLast() const
        {
            if (pointer >= GetSize())
            {
                return 0;
            }

            return GetSize() - pointer;
        }

        void SetPointer(int offset)
        {
            pointer = offset;
        }

    private:

        std::vector<uint8> data;

        int pointer = 0;

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

    bool IsValid() const
    {
        return is_valid;
    }

protected:

    bool is_valid = false;
};


struct BlockTAP_Header : public BlockTAP
{
    BlockTAP_Header(wxDataInputStream &);

    int size_data = 0;      // Это размер данных, на которые указывает данный заголовок

private:

    uint8 Read8();
    uint16 Read16();

    void ReadBytes(uint8 *, int);

    uint8 crc = 0;
};


struct BlockTAP_BASIC : public BlockTAP
{
    BlockTAP_BASIC(wxDataInputStream &, BlockTAP_Header *) : BlockTAP() { }

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

    // Распарсить считанные данные в массив lines
    bool ParseBASIC(ProgramBASIC &);

    LineBASIC ParseLineProgram();
};


struct BlockTAP_ArrayNumber : public BlockTAP
{
    BlockTAP_ArrayNumber(wxDataInputStream &, BlockTAP_Header *) : BlockTAP() { }

    char NameVariable() const
    {
        return (char)(param1 >> 8);
    }
};


struct BlockTAP_ArrayCharacter : public BlockTAP
{
    BlockTAP_ArrayCharacter(wxDataInputStream &, BlockTAP_Header *) : BlockTAP() { }

    char NameVariable() const
    {
        return (char)(param1 >> 8);
    }
};


struct BlockTAP_Code : public BlockTAP
{
    BlockTAP_Code(wxDataInputStream &, BlockTAP_Header *) : BlockTAP() { }

    // Адрес загрузки
    int StartAddressCode() const
    {
        return param1;
    }

    // Parse machine ocde block to asm program
    bool ParseASM(ProgramASM &);
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
