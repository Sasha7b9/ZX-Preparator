// 2022/05/20 15:48:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ParserTAP.h"
#pragma warning (push, 0)
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#pragma warning (pop)


void ParserTAP::Run(wxBufferedInputStream &stream)
{
    blocks.clear();

    while (!stream.Eof())
    {
        blocks.push_back(ParseBlock(stream));
    }
}


BlockTAP ParserTAP::ParseBlock(wxBufferedInputStream &stream)
{

}
