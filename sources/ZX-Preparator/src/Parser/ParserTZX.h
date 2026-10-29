// 2026/10/28 22:08:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/Parser.h"


struct HeaderTZX
{
    char signature[7];          // TZX signature = "ZXTape!"
    uint8 marker_end_of_text;   // 0x1A
    uint8 revision_major;       // 1    // Последняя версия - 1.20
    uint8 revision_minor;       // 20   //

    void Read(wxDataInputStream &);

    bool IsValid() const;
};


struct BlockTZX : public Block
{
    BlockTZX(wxDataInputStream &stream) : Block(stream)
    {
    }

    virtual bool Read() override;

private:
};


struct DescriptionTZX : public Description
{

};


struct FileTZX : public File
{
    HeaderTZX header;
};


class ParserTZX
{
public:

    bool Execute(pchar fileName);

    FileTZX file;

    std::shared_ptr<DescriptionTZX> desc;
};
