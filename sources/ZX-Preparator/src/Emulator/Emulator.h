// 2022/05/23 17:19:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push)
#pragma warning(disable:4820)


#define ADDR_C 0
#define ADDR_B 1
#define ADDR_E 2
#define ADDR_D 3
#define ADDR_L 4
#define ADDR_H 5
#define ADDR_F 6
#define ADDR_A 7

#define ADDR_BC 0
#define ADDR_DE 1
#define ADDR_HL 2
#define ADDR_AF 3
#define ADDR_SP 4
#define ADDR_IX 5
#define ADDR_IY 6
#define ADDR_PC 7


union REGS
{
    unsigned char r8[8];    // C B  E D  L H  F A
    unsigned short r16[8];  // BC   DE   HL   AF    SP  IX  IY  PC
};


struct InfoStruct
{
    unsigned char  MEMORY[65536];       // Full computer memory
    char           mnemonic[100];       // Mnemonic current instruction
    char           comment[100];
    char           flags[100];          // Incstruction effect onf flags
    char           transcript[100];     // Instruction summary
    unsigned int   addresses[10];
    int            numAddresses;
    unsigned char  opCodes[10];
    int            numOpCodes;
    int            tackts;
};


namespace Emulator
{
    void Init(InfoStruct *out);
    int  Decode(int address);  // ret value : 0 - unknown instruction, else - successful
    int  Run(int address);     // ret value : 0 - unknown instruction, else - successful
    int  RunNext(void);        // ret value : 0 - unknown instruction, else - successful
}


#pragma warning(pop)
