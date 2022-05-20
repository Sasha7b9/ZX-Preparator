// 2022/05/20 15:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <vector>


class wxBufferedInputStream;
class wxDataInputStream;


struct BlockTAP
{
    bool Parse(wxDataInputStream &);

    bool IsValid() const;

    struct CommonStruct
    {
        uint16 size;

        void Clear() { valid = false; };

        bool IsValid() const { return valid; };

    protected:

        bool valid;
    };

    struct Header : public CommonStruct
    {
        uint8 type_data;    // Тип данных, следующих за заголовком
                            // 00
                            // 01
                            // 02
                            // 03 - code
        char file_name[11];

        bool Parse(wxDataInputStream &);

    } header;

    struct Data : public CommonStruct
    {
        bool Parse(wxDataInputStream &);

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
