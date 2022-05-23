// 2022/05/23 17:30:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "instrDD.h"
#include "defines.h"
#include "registers.h"
#include "Computer/RAM.h"
#include "instrFDCB_DDCB.h"
#include <cstring>
#include <cstdio>


int LD_R_pIX_D_run(void)
{
    uint8 valueReg = prevPC;

    R8_HI(valueReg) = RAM[IX + PCandInc()];
    
    return 19;
}



int LD_R_pIX_D_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);

    sprintf(MNEMONIC, "LD %s,[IX+%02d]", R8_HI_Name(RAM8(PC - 1)), RAM8(PC));

    return -1;
}


int LD_pIX_D_R_run(void)
{
    uint8 valueReg = prevPC;

    RAM[IX + PCandInc()] = R8_LO(valueReg);

    return 19;
}



int LD_pIX_D_R_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "LD [IX+%02d],%s", RAM8(PC), R8_LO_Name(RAM8(PC - 1)));
    return -1;
}


int POP_IX_run(void)
{
    IX = RAM16(SP);
    SP += 2;
    return 14;
}



int POP_IX_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "POP IX");
    return -1;
}


int LD_pIX_D_N_run(void)
{
    uint8 valD = PCandInc();
    uint8 valN = PCandInc();
    RAM[IX + valD] = valN;
    return 19;
}



int LD_pIX_D_N_dec(void)
{
    return 0;
}


int LD_IX_NN(void)
{
#ifdef LISTING

    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    AddAddress(PC + 2);
    sprintf(MNEMONIC, "LD IX,%04X", PC16andInc());
    return -1;

#else

    IX = PC16andInc();
    return 14;

#endif
}



int DEC_IX_dec(void)
{
#ifdef LISTING

    AddAddress(PC);
    sprintf(MNEMONIC, "DEC IX");
    return -1;

#else

    IX -= 1;
    return 10;

#endif
}


int LD_IX_pNN(void)
{
    IX = RAM16(PC16andInc());
    return 20;
}


int LD_pNN_IX(void)
{
    RAM16(PC16andInc()) = IX;
    return 20;
}


int LD_SP_IX(void)
{
    SP = IX;
    return 10;
}


int PUSH_IX(void)
{
    RAM16(SP - 2) = IX;
    SP -= 2;
    return 15;
}


int EX_pSP_IX(void)
{
    uint16 temp;
    EXCH(IX, RAM16(SP));
    return 23;
}


int ADD_A_pIX_D(void)
{
    rA += RAM8(IX + PCandInc());
    return 19;
}


int INC_pIX_D(void)
{
    RAM16(IX + PCandInc()) += 1;
    return 23;
}


int ADD_IX_PP(void)
{
#ifdef LISTING

    AddAddress(PC);
    sprintf(MNEMONIC, "ADD IX,%s", PP_45_Name(prevPC));
    return -1;

#else

    IX += PP_45(prevPC);
    return 15;

#endif
}


int INC_IX_run(void)
{
    IX += 1;
    return 10;
}



int INC_IX_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "INC IX");
    return -1;
}


int JP_pIX(void)
{
#ifdef LISTING

    strcpy(MNEMONIC, "JP [IX]");
    return -1;

#else

    PC = IX;
    return 8;

#endif
}


int RLC_pIX_D_and_BIT_B_pIX_D_and_SET_B_pIX_D_RES_B_pIX_D(void)
{
    return RunThridLevel(IR_IX);

    /*
#ifdef LISTING

    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    AddAddress(PC + 2);

    uint8 valD = PCandInc();
    uint8 valBit = PCandInc();
    uint8 bit = (valBit >> 3) & 7;

    if ((valBit & 0xc7) == 0x86)    // RES b, [IX + d]
    {
        sprintf(MNEMONIC, "RES %d,[IX+%02X]", bit, valD);
        return -1;
    }

    return 1;

#else

    uint8 valD = PCandInc();
    uint8 sign = PCandInc();

    if(sign == 0x06)            // RLC (IX + D)
    {
        uint8 hiBit = GET_BIT(RAM8(IX + valD), 7);
        LOAD_C(hiBit);
        RAM8(IX + valD) <<= 1;
        LOAD_BIT(RAM8(IX + valD), 0, hiBit);
        return 23;
    }
    if((sign & 0x67) == 0x46)    // BIT B, (IX + D)
    {
        uint8 numBit = (valD >> 3) & 7;
        uint8 valueZ = GET_BIT(RAM8(IX + D), numBit);
        LOAD_Z(valueZ);
        return 20;
    }
    if((sign & 0xc7) == 0xc6)   // SET B, (IX + D)
    {
        uint8 numBit = (valD >> 3) & 7;
        RAM8(IX + D) |= (1 << numBit);
        return 23;
    }

    LOG_ERROR();

    return 0;

#endif
    */
}


#include "FuncSecondDD.h"


int DecodeCommandDD(void)
{
    AddOpcode(RAM8(PC));

    int index = PCandInc();

    if(secondLevelDD[index][0] == 0)
    {
        return 0;
    }

    return secondLevelDD[index][0]();
}


int RunCommandDD(void)
{
    int index = PCandInc();

    if(secondLevelDD[index][1] == 0)
    {
        return 0;
    }

    return secondLevelDD[index][1]();
}
