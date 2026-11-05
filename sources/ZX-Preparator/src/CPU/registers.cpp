// 2022/05/23 17:33:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CPU/registers.h"
#include "defines.h"
#include "Computer/RAM.h"


uint8 prevValue = 0;

uint8 regI = 0;     // Interrupt register
uint8 regR = 0;     // Regeneration register

REGS regs;

REGS regsAlt;

uint8 iff1 = 0;
uint8 iff2 = 0;

uint8 imfA = 0;
uint8 imfB = 0;


static uint8 *FuncRegA()
{
    return &regs.r8[7];
}


static uint8 *FuncRegB()
{
    return &regs.r8[1];
}


static uint8 *FuncRegC()
{
    return &regs.r8[0];
}


static uint8 *FuncRegD()
{
    return &regs.r8[3];
}


static uint8 *FuncRegE()
{
    return &regs.r8[2];
}


static uint8 *FuncRegH()
{
    return &regs.r8[5];
}


static uint8 *FuncRegL()
{
    return &regs.r8[4];
}


static uint8 *FuncRegpHL()
{
    return &RAM[regs.r16[2]];
}


pFuncpU8V funcsReg8[8] =
{
    FuncRegB,
    FuncRegC,
    FuncRegD,
    FuncRegE,
    FuncRegH,
    FuncRegL,
    FuncRegpHL,
    FuncRegA
};


static uint16 *FuncRegSP()
{
    return &regs.r16[4];
}


static uint16 *FuncRegHL()
{
    return &regs.r16[2];
}


static uint16 *FuncRegDE()
{
    return &regs.r16[1];
}


static uint16 *FuncRegBC()
{
    return &regs.r16[0];
}


pFuncpU16V funcsRegDD[4] =
{
    FuncRegBC,
    FuncRegDE,
    FuncRegHL,
    FuncRegSP
};


static uint16 *FuncRegIX()
{
    return &regs.r16[5];
}


pFuncpU16V funcsRegPP[4] =
{
    FuncRegBC,
    FuncRegDE,
    FuncRegIX,
    FuncRegSP
};


void AddPC(uint8 delta)
{
    int16 address = (int16)rPC + (int8)delta;
    rPC = (uint16)address;
}


uint8 PCandInc()
{
    prevValue = RAM[rPC];
    rPC++;
    return prevValue;
}


uint16 PC16andInc()
{
    uint8 low = RAM[rPC++];
    uint8 hi = RAM[rPC++];

    return (uint16)(low + hi * 256);
}


uint16 ValuePC()
{
    return rPC;
}


char *R8_HI_Name(uint8 value)
{
    const char *names[8] =
    {
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "[HL]",
        "A"
    };

    return (char*)names[(value >> 3) & 7];
}


char *R8_LO_Name(uint8 value)
{
    const char *names[8] =
    {
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "[HL]",
        "A"
    };

    return (char*)names[value & 7];
}


char *DD_45_Name(uint8 value)
{
    const char *names[4] =
    {
        "BC",
        "DE",
        "HL",
        "SP"
    };

    return (char*)names[(value >> 4) & 3];
}


char *QQ_45_Name(uint8 value)
{
    const char *names[4] =
    {
        "BC",
        "DE",
        "HL",
        "AF"
    };

    return (char*)names[(value >> 4) & 3];
}


char *SS_45_Name(uint8 value) //-V524
{
    const char *names[4] =
    {
        "BC",
        "DE",
        "IY",
        "SP"
    };

    return (char*)names[(value >> 4) & 3];
}


char *PP_45_Name(uint8 value)
{
    const char *names[4] =
    {
        "BC",
        "DE",
        "IX",
        "SP"
    };

    return (char *)names[(value >> 4) & 3];
}
