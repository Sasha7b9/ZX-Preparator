// 2026/10/29 11:11:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserHEX.h"


bool ParserHEX::Execute(pchar fileName)
{
    desc = std::make_shared<DescriptionHEX>();

    file.Clear();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file.dump.FillFrom(fileName);

    desc->full_size = (int)file.dump.bytes.size();

    desc->unused_memory = 0;

    desc->valid = (desc->full_size != 0);

    return desc->valid;
}
