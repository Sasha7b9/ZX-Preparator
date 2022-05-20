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

    BlockTAP block;

    while (data.IsOk() && block.Parse(data))
    {
        blocks.push_back(block);
    }

    if (data.IsOk() || !block.IsValid())
    {
        blocks.clear();
    }
}


bool BlockTAP::Parse(wxDataInputStream &stream)
{
    Clear();

    if (header.Parse(stream))
    {
        data.Parse(stream);
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


bool BlockTAP::Header::Parse(wxDataInputStream &stream)
{
    valid = false;

    if (stream.Read16() == 19)      // Для заголовка здесь всегда 19
    {
        if (stream.Read8() == 0)    // Чтение флага - для заголовка 0
        {
            stream >> type_data;

            if (type_data < 4)
            {
                for (int i = 0; i < 10; i++)
                {
                    file_name[i] = (char)stream.Read8();
                }

                file_name[10] = 0;
            }
        }
    }

    return valid;
}


bool BlockTAP::Data::Parse(wxDataInputStream &stream)
{
    valid = false;

    return valid;
}
