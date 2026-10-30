// 2026/10/29 09:15:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/Parser.h"
#include "Parser/ParserHEX.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserTZX.h"
#pragma warning (push, 0)
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)


uint8 Block::Read8()
{
    return stream.Read8();
}


void Block::Read8(void *buffer, size_t size)
{
    stream.Read8((uint8 *)buffer, size);
}


uint16 Block::Read16()
{
    return stream.Read16();
}


uint Block::Read32()
{
    return stream.Read32();
}


void File::Clear()
{
    blocks.clear();
    dump.Clear();
};


FileHEX *File::ToHEX()
{
    return dynamic_cast<FileHEX *>(this);
}


FileTAP *File::ToTAP()
{
    return dynamic_cast<FileTAP *>(this);
}


FileTZX *File::ToTZX()
{
    return dynamic_cast<FileTZX *>(this);
}
