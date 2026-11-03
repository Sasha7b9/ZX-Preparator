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

    virtual pchar NameASCII() const;

    BlockTAP_Header *ToHeader() const;
    BlockTAP_BASIC *ToBASIC() const;
    BlockTAP_ArrayNumber *ToArrayNumber() const;
    BlockTAP_ArrayCharacter *ToArrayCharacter() const;
    BlockTAP_Code *ToCode() const;
    BlockTAP_PureBytes *ToPureBytes() const;
    BlockTAP_Undefined *ToUndefined() const;

    void WriteType(Writer &, const wxString &, int);

    char file_name[11];

    uint16 param1 = 0;      // program    00 : number line start (or a number >= 32768 if no line parameter was given)
                            // arrays 01, 02 : variable name
                            // code       03 : holds the start of the code block when saved

    uint16 param2 = 0;      // program    00 : holds the start of the variable area relative to the start of the program
                            // arrays 01, 02 : N/A
                            // code       03 : holds 32768

    int GetSize() const
    {
        return data.AllBytes();
    }

    struct Data
    {
        void Push16(uint16);

        void Push8(uint8);

        void AppendBytes(wxDataInputStream &, int size);

        uint8 *BeginData()
        {
            return data.data();
        }

        void ReadBytes(uint8 *, int size);

        uint16 Read16Reverse();

        uint16 Read16();

        uint8 Read8();

        char ReadChar();

        bool IsEmpty() const;

        // Всего байт в блоке
        int AllBytes() const
        {
            return (int)data.size();
        }

        // Осталось байт для чтения
        int BytesLast() const
        {
            if (pointer >= AllBytes())
            {
                return 0;
            }

            return AllBytes() - pointer;
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
            PURE_BYTES
        };
    };

    bool IsValid() const
    {
        return is_valid;
    }

protected:

    bool is_valid = false;

    uint8 crc = 0;

    uint8 Read8(int offset = -1);
    uint16 Read16();
    uint16 Read16Reverse();

    void ReadBytes(uint8 *, int);
};


struct BlockTAP_Header : public BlockTAP
{
    BlockTAP_Header(wxDataInputStream &);

    int size_data = 0;      // Это размер данных, на которые указывает данный заголовок

    Type::E type = (Type::E)128;

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
};


struct BlockTAP_BASIC : public BlockTAP
{
    BlockTAP_BASIC(wxDataInputStream &, BlockTAP_Header *);

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
    // num_bytes - столько байт нужно распарсить
    bool ParseBASIC(uint16 num_bytes);

    // nym_bytes - количество распарсенных байт
    LineBASIC ParseLineProgram(uint16 &num_bytes);

    ProgramBASIC program;

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
};


struct BlockTAP_ArrayNumber : public BlockTAP
{
    BlockTAP_ArrayNumber(wxDataInputStream &, BlockTAP_Header *);

    char NameVariable() const
    {
        return (char)(param1 >> 8);
    }

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
};


struct BlockTAP_ArrayCharacter : public BlockTAP
{
    BlockTAP_ArrayCharacter(wxDataInputStream &, BlockTAP_Header *);

    char NameVariable() const
    {
        return (char)(param1 >> 8);
    }

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
};


struct BlockTAP_Code : public BlockTAP
{
    BlockTAP_Code(wxDataInputStream &, BlockTAP_Header *);

    // Адрес загрузки
    int StartAddressCode() const
    {
        return param1;
    }

    // Parse machine ocde block to asm program
    bool ParseASM(ProgramASM &);

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
};


struct BlockTAP_PureBytes : public BlockTAP
{
    BlockTAP_PureBytes(wxDataInputStream &);

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
};


struct BlockTAP_Undefined : public BlockTAP
{
    BlockTAP_Undefined(wxDataInputStream &);

    virtual pchar NameASCII() const override;

    virtual void Write(Writer &, int) override;
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
