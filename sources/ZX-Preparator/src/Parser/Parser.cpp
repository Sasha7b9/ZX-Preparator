// 2026/10/29 09:15:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/Parser.h"
#include "Parser/ParserHEX.h"
#include "Parser/ParserTAP.h"


void File::Clear()
{
    blocks.clear();
    dump.Clear();
};


bool Description::IsHEX() const
{
    return dynamic_cast<const DescriptionHEX *>(this) != nullptr;
}


bool Description::IsTAP() const
{
    return dynamic_cast<const DescriptionTAP *>(this) != nullptr;
}
