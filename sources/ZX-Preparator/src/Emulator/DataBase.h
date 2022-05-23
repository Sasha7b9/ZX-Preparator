// 2022/05/23 16:56:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Emulator/Command.h"
#include "Emulator/Emulator.h"


class DataBase
{
public:
    DataBase();

    // Return address for decompilation. If == -1, addresses not exists
    int NextAddress();

    void AddNewData(bool succsefull, int address, OutStruct *params);

    void CreateReport(pchar file_name);

private:
    std::vector<Command> commands;
    std::vector<int> addressesForScan;

    void RemoveScanAddress(int address);
    void AddScanAddress(int address);
    bool AddressAlreadyScanOrFuture(int address);
    void WriteCommand(std::ofstream &file, Command &command);
    void WriteBinaryByte(std::ofstream &file, int value);
};
