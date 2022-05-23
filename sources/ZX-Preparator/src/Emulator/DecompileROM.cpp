// 2022/05/23 16:56:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/DataBase.h"
#include "Emulator/Emulator.h"
#include "Emulator/RAM48.h"



DataBase base(0, 16384);

OutStruct params;



void DecompileROM()
{
    std::memcpy(params.RAM, RAM48, 64 * 1024);

    Emulator::Init(&params);

    int address = base.NextAddress();

    while(address >= 0)
    {
        bool res = Emulator::Decode((uint16)address) != 0;

        base.AddNewData(res, address, &params);

        address = base.NextAddress();
    }

    base.CreateReport("report.txt");
}