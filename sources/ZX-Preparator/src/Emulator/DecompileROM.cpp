// 2022/05/23 16:56:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/DataBase.h"
#include "Emulator/Emulator.h"
#include "Emulator/Memory.h"


static DataBase base(0, 16384);

static OutStruct params;


void DecompileROM()
{
    std::memcpy(params.MEMORY, Memory::_48, 64 * 1024);

    Emulator::Init(&params);

    base.AddScanAddress(0);
    base.AddScanAddress(8);
    base.AddScanAddress(0x10);
    base.AddScanAddress(0x18);
    base.AddScanAddress(0x20);
    base.AddScanAddress(0x28);
    base.AddScanAddress(0x30);
    base.AddScanAddress(0x38);

    int address = base.NextAddress();

    while(address >= 0)
    {
        bool res = Emulator::Decode((uint16)address) != 0;

        base.AddNewData(res, address, &params);

        address = base.NextAddress();
    }

    base.CreateReport("report.txt");
}