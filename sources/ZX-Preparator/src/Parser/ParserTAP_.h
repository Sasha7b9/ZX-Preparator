// 2022/05/20 15:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/StructuresZX.h"
#include "Parser/Parser.h"
#include <vector>
#include <string>


class Notebook;


struct BlockTAP : public Block
{
    BlockTAP(wxDataInputStream &stream) : Block(stream), header(this), data(this) {}

    // Читать данные из входного потока
    virtual Block *Read() override;

    // Распарсить считанные данные в массив lines
    bool ParseBASIC(ProgramBASIC &);

    // Parse machine ocde block to asm program
    bool ParseASM(ProgramASM &);

    bool IsValid() const;

    bool IsBASIC()          const { return header.type == Header::Type::BASIC_PROGRAM; }
    bool IsArrayNumber()    const { return header.type == Header::Type::NUMBER_ARRAY; }
    bool IsArrayCharacter() const { return header.type == Header::Type::CHARACTER_ARRAY; }
    bool IsCODE()           const { return header.type == Header::Type::CODE; }
    bool IsPureBytes()      const { return header.type == Header::Type::PURE_BYTES; }

    struct CommonStruct
    {
        CommonStruct(BlockTAP *_block) : block(_block) {}

        uint16 size = 0;
        uint8 crc = 0;

        void Clear() { valid = false; crc = 0; };

        bool IsValid() const { return valid; };

    protected:

        wxDataInputStream &GetStream()
        {
            return block->stream;
        }

        BlockTAP *block;

        bool valid = false;

        uint16 Read16();

        uint8 Read8();
    };

    struct Header : public CommonStruct
    {
        Header(BlockTAP *block) : CommonStruct(block)
        {
            std::memset(file_name, 0, 11);
        }

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

        char file_name[11];
        uint16 size_data = 0;   // Длина блока данных, следуюшего за заголовком, без учёта флага и контрольной суммы

        uint16 param1 = 0;      // program    00 : number line start (or a number >= 32768 if no line parameter was given)
                                // arrays 01, 02 : variable name
                                // code       03 : holds the start of the code block when saved
        uint16 param2 = 0;      // program    00 : holds the start of the variable area relative to the start of the program
                                // arrays 01, 02 : N/A
                                // code       03 : holds 32768

        bool Read();

    } header;

    struct Data : public CommonStruct
    {
        Data(BlockTAP *block) : CommonStruct(block) {}

        bool Read(const Header &);

        std::vector<uint8> data;

    } data;

private:

    void Clear();

    LineBASIC ParseLineProgram();
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
