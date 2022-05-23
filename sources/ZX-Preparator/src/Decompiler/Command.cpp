// 2022/05/23 16:56:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"


#include "Command.h"


bool Command::operator<(const Command &first)
{
    return address < first.address;
};
