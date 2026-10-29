// 2026/10/29 11:11:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserHEX.h"


bool ParserHEX::Execute(pchar fileName)
{
    file = std::make_shared<FileHEX>();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file->dump.FillFrom(fileName);

    file->is_valid = (file->FullSize() != 0);

    return file->is_valid;
}
