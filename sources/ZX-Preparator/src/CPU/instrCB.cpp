// 2022/05/23 17:29:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CPU/registers.h"
#include "defines.h"
#include "CPU/instrCB.h"
#include "defines.h"
#include "Computer/ports.h"
#include "Computer/RAM.h"
#include "CPU/instrShift.h"
#include <string.h>
#include <stdio.h>



int BIT_B_R(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "BIT %d,%s", (prevPC >> 3) & 7, R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int CRL_R(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "SRL %s", R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int CRL_pHL(void)
{
#ifdef LISTING

    AddAddress(rPC);
    strcpy(MNEMONIC, "SRL [HL]");
    return -1;

#else

    return 0;

#endif
}


int RLC_R(void)
{
    return RLC(Operand_Reg8);
}



int RRC_R(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "RRC %s", R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int RES_B_M(void)
{
#ifdef LISTING

    AddAddress(rPC);
    uint8 numBit = (prevPC >> 3) & 7;
    sprintf(MNEMONIC, "RES %d,%s", numBit, R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int RES_B_pHL(void)
{
#ifdef LISTING

    AddAddress(rPC);
    uint8 numBit = (prevPC >> 3) & 7;
    sprintf(MNEMONIC, "RES %d,[HL]", numBit);
    return -1;

#else

    return 0;

#endif
}


int SET_B_R(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "SET %d,%s", (prevPC >> 3) & 7, R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int SET_B_pHL(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "SET %d,[HL]", (prevPC >> 3) & 7);
    return -1;

#else

    return 0;

#endif
}


int RL_R(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "RL %s", R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int RR_R(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "RR %s", R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int SLA(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "SLA %s", R8_LO_Name(prevPC));

    return -1;

#else

    return 0;

#endif
}


int SRA(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "SRA %s", R8_LO_Name(prevPC));
    return -1;

#else

    return 0;

#endif
}


int RLC_pHL(void)
{
#ifdef LISTING

    AddAddress(rPC);
    sprintf(MNEMONIC, "RLC [HL]");
    return -1;

#else

    return RLC(Operand_pHL);

#endif
}


#include "FuncSecondCB.h"


int RunCommandCB(void)
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelCB[index][1] == 0)
    {
        return 0;
    }

    return secondLevelCB[index][1]();
}


int DecodeCommandCB(void)
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelCB[index][0] == 0)
    {
        return 0;
    }

    return  secondLevelCB[index][0]();
}
