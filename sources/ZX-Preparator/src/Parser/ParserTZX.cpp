// 2026/10/29 09:12:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserTZX.h"
#pragma warning (push, 0)
    #include <wx/statline.h>
    #include <wx/statline.h>
    #include <wx/file.h>
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)


/*
    Описание : https://worldofspectrum.net/TZXformat.html
*/


bool ParserTZX::Execute(pchar fileName)
{
    desc = std::make_shared<DescriptionTZX>();

    file.Clear();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file.dump.FillFrom(fileName);

    desc->full_size = (int)file.dump.bytes.size();

    wxFileInputStream file_input(fileName);

    wxBufferedInputStream stream(file_input);

    wxDataInputStream data(stream);

    HeaderTZX header;
    header.Read(data);

    if (!header.IsValid())
    {
        desc->valid = false;
    }
    else
    {
        while (data.IsOk())
        {
            BlockTZX block(data);

            if (block.Read())
            {
                file.AppendBlock(std::make_shared<BlockTZX>(std::move(block)));
                desc->numberBlocks++;
            }
            else
            {
                break;
            }
        }

        desc->valid = (file.NumberBlocks() != 0);

        desc->unused_memory = 0;

        while (data.IsOk())
        {
            data.Read8();
            desc->unused_memory++;
        }
    }

    return desc->valid;
}


bool BlockTZX::Read()
{
    return false;
}


void HeaderTZX::Read(wxDataInputStream &stream)
{
    uint8 *pointer = (uint8 *)this;

    for (uint i = 0; i < sizeof(*this); i++)
    {
        *pointer++ = stream.Read8();
    }
}


bool HeaderTZX::IsValid() const
{
    pchar key = "ZXTape!";

    if (std::memcmp(signature, key, 7) != 0)
    {
        return false;
    }

    if (marker_end_of_text != 0x1A)
    {
        return false;
    }

    return true;
}
