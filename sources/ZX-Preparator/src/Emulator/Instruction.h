// 2022/05/23 16:56:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <vector>
#include <string>


struct Instruction
{
    bool bad = false;
    int address = 0;                    // Command location address
    std::vector<uint8> opCodes;         // Command codes
    std::string mnemonic;               // Command mnemonic
    int numTackts = 0;
    std::string transcript;
    std::string flags;
    std::string comment;
    int tackts = 0;                     // Cycles for execute command

    bool operator<(const Instruction &first);
};
