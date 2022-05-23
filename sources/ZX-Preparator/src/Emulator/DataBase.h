// 2022/05/23 16:56:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Emulator/Instruction.h"
#include "Emulator/Emulator.h"


class StorageInstructions
{
public:
    StorageInstructions(int _addr_min, int _addr_max) : addr_min(_addr_min), addr_max(_addr_max) { }

    // Append new entry point
    void AppendEntryPoint(int address);

    // Return address for decompilation. If == -1, addresses not exists
    int NextAddress();

    // Append data for instructions on address
    void AddNewData(bool succsefull, int address, OutStruct *params);

    // Write report to file
    void CreateReport(pchar file_name);

private:
    std::vector<Instruction> commands;
    std::vector<int> addressesForScan;

    int addr_min = 0;
    int addr_max = 0;

    void RemoveScanAddress(int address);
    bool AddressAlreadyScanOrFuture(int address);
    void WriteCommand(std::ofstream &file, Instruction &command);
    void WriteBinaryByte(std::ofstream &file, int value);
};
