// 2022/05/23 16:56:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Command
{
public:
    bool bad = false;
    int address = 0;
    std::vector<uint8> opCodes;
    std::string mnemonic;
    int numTackts = 0;
    std::string transcript;
    std::string flags;
    std::string comment;
    int tackts = 0;

    bool operator<(const Command &first);
};
