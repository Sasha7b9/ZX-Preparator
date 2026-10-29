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


bool File::IsHEX() const
{
    return dynamic_cast<const FileHEX *>(this) != nullptr;
}


bool File::IsTAP() const
{
    return dynamic_cast<const FileTAP *>(this) != nullptr;
}


bool File::IsTZX() const
{
    return dynamic_cast<const FileTZX *>(this) != nullptr;
}
