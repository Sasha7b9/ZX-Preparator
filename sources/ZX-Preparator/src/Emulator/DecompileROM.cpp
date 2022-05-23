// 2022/05/23 16:56:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Emulator/DataBase.h"
#include "Emulator/Emulator.h"
#include "Emulator/Memory.h"


static StorageInstructions storage(0, 16384);

static OutStruct params;


void DecompileROM()
{
    std::memcpy(params.MEMORY, Memory::_48, 64 * 1024);

    Emulator::Init(&params);

    storage.AddScanAddress(0);
    storage.AddScanAddress(8);
    storage.AddScanAddress(0x10);
    storage.AddScanAddress(0x18);
    storage.AddScanAddress(0x20);
    storage.AddScanAddress(0x28);
    storage.AddScanAddress(0x30);
    storage.AddScanAddress(0x38);

    int address = storage.NextAddress();

    while(address >= 0)
    {
        bool res = Emulator::Decode((uint16)address) != 0;

        storage.AddNewData(res, address, &params);

        address = storage.NextAddress();
    }

    storage.CreateReport("report.txt");
}