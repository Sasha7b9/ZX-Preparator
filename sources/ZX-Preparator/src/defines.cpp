// 2022/5/23 19:37:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/Emulator.h"


void AddAddress(uint addr)
{
    out->addresses[out->numAddresses++] = addr;
}


void AddOpcode(uint8 code)
{
    out->opCodes[out->numOpCodes++] = code;
}


bool CalculateH(uint8 before, uint8 after)
{
    return GET_nBIT(before, 4) != GET_nBIT(after, 4);
}
