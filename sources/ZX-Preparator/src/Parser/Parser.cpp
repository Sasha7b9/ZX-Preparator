// 2026/10/29 09:15:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/Parser.h"
#include "Parser/ParserHEX.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserTZX.h"


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
