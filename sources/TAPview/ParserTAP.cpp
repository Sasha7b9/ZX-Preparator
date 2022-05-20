// 2022/05/20 15:48:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ParserTAP.h"
#pragma warning (push, 0)
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#pragma warning (pop)


void ParserTAP::Run(wxBufferedInputStream &stream)
{
    blocks.clear();

    wxDataInputStream data(stream);

    BlockTAP block(data);

    while (data.IsOk() && block.Parse())
    {
        blocks.push_back(block);
    }

    if (data.IsOk() || !block.IsValid())
    {
        blocks.clear();
    }
}


bool BlockTAP::Parse()
{
    Clear();

    if (header.Parse())
    {
        data.Parse();
    }

    return header.IsValid() && data.IsValid();
}


bool BlockTAP::IsValid() const
{
    return header.IsValid() && data.IsValid();
}


void BlockTAP::Clear()
{
    header.Clear();

    data.Clear();
}


bool BlockTAP::Header::Parse()
{
    valid = false;

    if (Read16() == 19)      // Для заголовка здесь всегда 19
    {
        if (Read8() == 0)    // Чтение флага - для заголовка 0
        {
            type_data = Read8();

            if (type_data < 4)
            {
                for (int i = 0; i < 10; i++)
                {
                    file_name[i] = (char)Read8();
                }

                file_name[10] = 0;

                size_data = Read16();

                param1 = Read16();

                param2 = Read16();
            }
        }
    }

    return valid;
}


bool BlockTAP::Data::Parse()
{
    valid = false;

    return valid;
}


uint8 BlockTAP::CommonStruct::Read8()
{
    uint8 byte = stream.Read8();

    crc ^= byte;

    return byte;
}


uint16 BlockTAP::CommonStruct::Read16()
{
    uint16 result = Read8();

    result |= Read8() << 8;

    return result;
}
