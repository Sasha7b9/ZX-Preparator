// 2022/05/20 15:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <vector>
#include <string>


/*

    Описание : https://sinclair.wiki.zxnet.co.uk/wiki/TAP_format

*/


class wxBufferedInputStream;
class wxDataInputStream;
struct ProgramBASIC;
struct ProgramASM;


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


struct BlockTAP
{
    BlockTAP(wxDataInputStream &stream) : header(stream), data(stream) {}

    // Читать данные из входного потока
    bool Read();

    // Распарсить считанные данные в массив lines
    bool ParseBASIC(ProgramBASIC &);

    // Parse machine ocde block to asm program
    bool ParseASM(ProgramASM &);

    bool IsValid() const;

    bool IsBASIC()          const { return header.type_data == Header::TypeData::BASIC_PROGRAM; }
    bool IsArrayNumber()    const { return header.type_data == Header::TypeData::NUMBER_ARRAY; }
    bool IsArrayCharacter() const { return header.type_data == Header::TypeData::CHARACTER_ARRAY; }
    bool IsASM()            const { return header.type_data == Header::TypeData::CODE; }
    bool IsPureBytes()      const { return header.type_data == Header::TypeData::PURE_BYTES; }

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

        struct TypeData
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

        TypeData::E type_data = TypeData::Undefined;

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


struct DescriptionTAP
{
    void Clear();

    bool valid = false;

    int numberBlocks = 0;

    int unused_memory = 0;      // КОличество нераспознанной памяти

    int full_size = 0;          // Количество байт в исходном файле

    struct Block
    {
        Block(pchar n, BlockTAP::Header::TypeData::E t, uint16 s, uint16 p1, uint16 p2) : type(t), size(s), param1(p1), param2(p2), name(n)
        {}
        BlockTAP::Header::TypeData::E type = BlockTAP::Header::TypeData::Undefined;
        uint16 size = 0;
        uint16 param1 = 0;
        uint16 param2 = 0;
        std::string name;
    };

    std::vector<Block> blocks;
};


class ParserTAP
{
public:

    bool Execute(pchar fileName);

    FileTAP file;

    DescriptionTAP desc;
};
