// 2022/05/20 15:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/ParserStructures.h"
#include <vector>
#include <string>


class wxBufferedInputStream;
class wxDataInputStream;


struct BlockTAP
{
    BlockTAP(wxDataInputStream &stream) : header(stream), data(stream) {}

    // Читать данные из входного потока
    bool Read();

    // Распарсить считанные данные в массив lines
    bool ParseBASIC(ProgramBASIC &program);

    bool IsValid() const;

    bool IsBASIC()          const { return header.type_data == 0; }
    bool IsArrayNumber()    const { return header.type_data == 1; }
    bool IsArrayCharacter() const { return header.type_data == 2; }
    bool IsASM()            const { return header.type_data == 3; }
    bool IsPureBytes()      const { return header.type_data == 4; }

    struct CommonStruct
    {
        CommonStruct(wxDataInputStream &_stream) : stream(_stream) {}

        uint16 size = 0;
        uint8 crc = 0;

        void Clear() { valid = false; crc = 0; };

        bool IsValid() const { return valid; };

    protected:

        bool valid = false;

        wxDataInputStream &stream;

        uint16 Read16();

        uint8 Read8();
    };

    struct Header : public CommonStruct
    {
        Header(wxDataInputStream &stream) : CommonStruct(stream)
        {
            std::memset(file_name, 0, 11);
        }

        uint8 type_data = 0xff;     // Тип данных, следующих за заголовком
                                    // 00 - program
                                    // 01 - number array
                                    // 02 - character array
                                    // 03 - code
                                    // 04 - pure bytes (bytes without a header)
        char file_name[11];
        uint16 size_data = 0;   // Длина блока данных, следуюшего за заголовком, без учёта флага и контрольной суммы

        uint16 param1 = 0;      // program    00 : number line start (or a number >= 32768 if no line parameter was given)
                                // arrays 01, 02 : variable name
                                // code       03 : holds the start of the code block when saved
        uint16 param2 = 0;      // program 00    : holds the start of the variable area relative to the start of the program
                                // arrays 01, 02 : N/A
                                // code       03 : holds 32768

        bool Read();

    } header;

    struct Data : public CommonStruct
    {
        Data(wxDataInputStream &stream) : CommonStruct(stream) {}

        bool Read(const Header &);

        std::vector<uint8> data;

        uint16 GetData16();
        uint16 GetData16Reverse();
        uint8 GetData8();
        char GetChar();

    } data;

private:

    void Clear();

    bool ParseProgram(ProgramBASIC &program);

    LineBASIC ParseLineProgram();
};


struct FileTAP
{
    void Clear();

    void AppendBlock(BlockTAP &block) { blocks.push_back(block); }

    int NumberBlocks() const { return (int)blocks.size(); }

    BlockTAP &GetBlock(int index) { return blocks[(uint)index]; }

    std::vector<BlockTAP> blocks;

    DumpHEX dump;
};


class ParserTAP
{
public:

    bool Execute(pchar fileName);

    FileTAP file;

    DescriptionTAP desc;
};
