// 2022/05/23 17:33:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "registers.h"
#include "../RAM.h"
#include "common.h"



uint8 prevPC = 0;

uint8 regI = 0;     // Interrupt register
uint8 regR = 0;     // Regeneration register

REGS regs;

REGS regsAlt;

uint8 iff1 = 0;
uint8 iff2 = 0;

uint8 imfA = 0;
uint8 imfB = 0;

---
static uint8 *FuncRegA(void)
{
    return &regs.r8[7];
}

---
static uint8 *FuncRegB(void)
{
    return &regs.r8[1];
}

---
static uint8 *FuncRegC(void)
{
    return &regs.r8[0];
}

---
static uint8 *FuncRegD(void)
{
    return &regs.r8[3];
}


---
static uint8 *FuncRegE(void)
{
    return &regs.r8[2];
}


---
static uint8 *FuncRegH(void)
{
    return &regs.r8[5];
}


---
static uint8 *FuncRegL(void)
{
    return &regs.r8[4];
}


---
static uint8 *FuncRegpHL(void)
{
    return &RAM[regs.r16[2]];
}


---
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

---
static uint16 *FuncRegSP(void)
{
    return &regs.r16[4];
}


---
static uint16 *FuncRegHL(void)
{
    return &regs.r16[2];
}


---
static uint16 *FuncRegDE(void)
{
    return &regs.r16[1];
}


---
static uint16 *FuncRegBC(void)
{
    return &regs.r16[0];
}


---
pFuncpU16V funcsRegDD[4] =
{
    FuncRegBC,
    FuncRegDE,
    FuncRegHL,
    FuncRegSP
};


---
static uint16 *FuncRegIX(void)
{
    return &regs.r16[5];
}


---
pFuncpU16V funcsRegPP[4] =
{
    FuncRegBC,
    FuncRegDE,
    FuncRegIX,
    FuncRegSP
};


---
char *PP_45_Name(uint8 value)
{
    const char *names[4] =
    {
        "BC",
        "DE",
        "IX",
        "SP"
    };

    return (char*)names[(value >> 4) & 3];
}


---
void AddPC(uint8 delta)
{
    int16 address = (int16)PC + (int8)delta;
    PC = (uint16)address;
}


---
uint8 PCandInc(void)
{
    prevPC = RAM[PC];
    PC++;
    return prevPC;
}


---
uint16 PC16andInc(void)
{
    uint8 low = RAM[PC++];
    uint8 hi = RAM[PC++];

    return low + hi * 256;
}


---
uint16 ValuePC(void)
{
    return PC;
}


---
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


---
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


---
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


---
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


---
char *SS_45_Name(uint8 value)
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

