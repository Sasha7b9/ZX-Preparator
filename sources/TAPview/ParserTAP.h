// 2022/05/20 15:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <vector>


class wxBufferedInputStream;
class wxDataInputStream;


struct BlockTAP
{
    BlockTAP(wxDataInputStream &stream) : header(stream), data(stream) {}

    bool Parse();

    bool IsValid() const;

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
                                    // 00
                                    // 01
                                    // 02
                                    // 03 - code
        char file_name[11];

        bool Parse();

    } header;

    struct Data : public CommonStruct
    {
        Data(wxDataInputStream &stream) : CommonStruct(stream) {}

        bool Parse();

    } data;

private:

    void Clear();

    bool ParseHeader(wxDataInputStream &);

    bool ParseData(wxDataInputStream &);
};


class ParserTAP
{
public:

    void Run(wxBufferedInputStream &);

    std::vector<BlockTAP> blocks;
};
