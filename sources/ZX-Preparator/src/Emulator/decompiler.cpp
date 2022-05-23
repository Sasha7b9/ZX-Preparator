// 2022/05/23 16:56:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/DataBase.h"
#include "Emulator/Emulator.h"
#include "Emulator/RAM48.h"



DataBase base;

OutStruct params;



int decompiler()
{
    params.RAM = RAM48;

    Emulator::Init(&params);

    int address = base.NextAddress();

    while(address >= 0)
    {
        bool res = Emulator::Decode((uint16)address) != 0;

        base.AddNewData(res, address, &params);

        address = base.NextAddress();
    }

    base.CreateReport();

    return 0;
}