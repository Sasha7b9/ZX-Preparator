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



int BIT_B_R_dec()
{
    sprintf(MNEMONIC, "BIT %d,%s", (prevPC >> 3) & 7, R8_LO_Name(prevPC));
    return -1;
}
int BIT_B_R_run()
{
    return 0;
}


int CRL_R_dec()
{
    sprintf(MNEMONIC, "SRL %s", R8_LO_Name(prevPC));
    return -1;
}
int CRL_R_run()
{
    return 0;
}


int CRL_pHL_dec()
{
    strcpy(MNEMONIC, "SRL [HL]");
    return -1;
}
int CRL_pHL_run()
{
    return 0;
}


int RLC_R_dec()
{
    sprintf(MNEMONIC, "RLC %s", R8_LO_Name(prevPC));
    return -1;
}
int RLC_R_run()
{
    return RLC(Operand_Reg8);
}



int RRC_R_dec()
{
    sprintf(MNEMONIC, "RRC %s", R8_LO_Name(prevPC));
    return -1;
}
int RRC_R_run()
{
    return RRC(Operand_Reg8);
}


int RES_B_M_dec()
{
    uint8 numBit = (prevPC >> 3) & 7;
    sprintf(MNEMONIC, "RES %d,%s", numBit, R8_LO_Name(prevPC));
    return -1;
}
int RES_B_M_run()
{
    return 0;
}


int RES_B_pHL_dec()
{
    uint8 numBit = (prevPC >> 3) & 7;
    sprintf(MNEMONIC, "RES %d,[HL]", numBit);
    return -1;
}
int RES_B_pHL_run()
{
    return 0;
}


int SET_B_R_dec()
{
    sprintf(MNEMONIC, "SET %d,%s", (prevPC >> 3) & 7, R8_LO_Name(prevPC));
    return -1;
}
int SET_B_R_run()
{
    return 0;
}


int SET_B_pHL_dec()
{
    sprintf(MNEMONIC, "SET %d,[HL]", (prevPC >> 3) & 7);
    return -1;
}
int SET_B_pHL_run()
{
    return 0;
}


int RL_R_dec()
{
    sprintf(MNEMONIC, "RL %s", R8_LO_Name(prevPC));
    return -1;
}
int RL_R_run()
{
    return 0;
}


int RR_R_dec()
{
    sprintf(MNEMONIC, "RR %s", R8_LO_Name(prevPC));
    return -1;
}
int RR_R_run()
{
    return 0;
}


int SLA_dec()
{
    sprintf(MNEMONIC, "SLA %s", R8_LO_Name(prevPC));

    return -1;
}
int SLA_run()
{
    return 0;
}


int SRA_dec()
{
    sprintf(MNEMONIC, "SRA %s", R8_LO_Name(prevPC));
    return -1;
}
int SRA_run()
{
    return 0;
}


int RLC_pHL_dec()
{
    sprintf(MNEMONIC, "RLC [HL]");
    return -1;
}
int RLC_pHL_run()
{
    return RLC(Operand_pHL);
}


#include "FuncSecondCB.h"


int RunCommandCB()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelCB[index][1] == 0)
    {
        return 0;
    }

    return secondLevelCB[index][1]();
}


int DecodeCommandCB()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelCB[index][0] == 0)
    {
        return 0;
    }

    return  secondLevelCB[index][0]();
}
