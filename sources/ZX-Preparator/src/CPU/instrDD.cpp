// 2022/05/23 17:30:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "instrDD.h"
#include "defines.h"
#include "registers.h"
#include "Computer/RAM.h"
#include "instrFDCB_DDCB.h"
#include <cstring>
#include <cstdio>



int CP_pIX_D_dec()
{
    AddOpcode(RAM8(rPC));
    AddAddress(rPC + 1);
    sprintf(MNEMONIC, "CP [IX+%02d]", PCandInc());
    return -1;
}


int CP_pIX_D_run()
{
    return 0;
}


int LD_R_pIX_D_dec()
{
    AddOpcode(RAM8(rPC));
    AddAddress(rPC + 1);

    sprintf(MNEMONIC, "LD %s,[IX+%02d]", R8_HI_Name(RAM8(rPC - 1)), RAM8(rPC));

    return -1;
}

int LD_R_pIX_D_run()
{
    uint8 valueReg = prevPC;

    R8_HI(valueReg) = RAM[rIX + PCandInc()];

    return 19;
}


int LD_pIX_D_R_run()
{
    uint8 valueReg = prevPC;

    RAM[rIX + PCandInc()] = R8_LO(valueReg);

    return 19;
}



int LD_pIX_D_R_dec()
{
    AddOpcode(RAM8(rPC));
    AddAddress(rPC + 1);
    sprintf(MNEMONIC, "LD [IX+%02d],%s", RAM8(rPC), R8_LO_Name(RAM8(rPC - 1)));
    return -1;
}


int POP_IX_run()
{
    rIX = RAM16(rSP);
    rSP += 2;
    return 14;
}



int POP_IX_dec()
{
    AddAddress(rPC);
    sprintf(MNEMONIC, "POP IX");
    return -1;
}


int LD_pIX_D_N_run()
{
    volatile uint8 valD = PCandInc();
    volatile uint8 valN = PCandInc(); //-V656
    RAM[rIX + valD] = valN;
    return 19;
}



int LD_pIX_D_N_dec()
{
    return 0;
}


int LD_IX_NN_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    AddAddress(rPC + 2);
    sprintf(MNEMONIC, "LD IX, #%04X", PC16andInc());
    return -1;
}

int LD_IX_NN_run()
{
    rIX = PC16andInc();
    return 14;
}



int DEC_IX_dec()
{
    AddAddress(rPC);
    sprintf(MNEMONIC, "DEC IX");
    return -1;
}

int DEC_IX_run()
{
    rIX -= 1;
    return 10;
}


int LD_IX_pNN_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    AddAddress(rPC + 2);
    sprintf(MNEMONIC, "LD IX,[%d]", PC16andInc());
    return -1;
}

int LD_IX_pNN_run()
{
    rIX = RAM16(PC16andInc());
    return 20;
}


int LD_pNN_IX_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    AddAddress(rPC + 2);
    sprintf(MNEMONIC, "LD [#%04X], IX", PC16andInc());
    return -1;
}


int LD_pNN_IX_run()
{
    RAM16(PC16andInc()) = rIX;
    return 20;
}


int LD_SP_IX()
{
    rSP = rIX;
    return 10;
}


int PUSH_IX_dec()
{
    AddAddress(rPC);
    strcpy(MNEMONIC, "PUSH IX");
    return -1;
}

int PUSH_IX_run()
{
    RAM16(rSP - 2) = rIX;
    rSP -= 2;
    return 15;
}


int EX_pSP_IX()
{
    uint16 temp;
    EXCH(rIX, RAM16(rSP));
    return 23;
}

int ADD_A_pIX_D_dec()
{
    AddAddress(rPC);
    uint8 d = PCandInc();
    AddAddress(rPC);
    AddOpcode(d);
    sprintf(MNEMONIC, "ADD A, [IX + %d]", d);
    return -1;
}

int ADD_A_pIX_D_run()
{
    rA += RAM8(rIX + PCandInc());
    return 19;
}


int INC_pIX_D()
{
    RAM16(rIX + PCandInc()) += 1;
    return 23;
}


int ADD_IX_PP_dec()
{
    AddAddress(rPC);
    sprintf(MNEMONIC, "ADD IX,%s", PP_45_Name(prevPC));
    return -1;
}

int ADD_IX_PP_run()
{
    rIX += PP_45(prevPC);
    return 15;
}


int INC_IX_run()
{
    rIX += 1;
    return 10;
}



int INC_IX_dec()
{
    AddAddress(rPC);
    sprintf(MNEMONIC, "INC IX");
    return -1;
}


int JP_pIX_dec()
{
    AddAddress(rPC);
    strcpy(MNEMONIC, "JP [IX]");
    return -1;
}


int JP_pIX_run()
{
    rPC = rIX;
    return 8;
}


int RLC_pIX_D_and_BIT_B_pIX_D_and_SET_B_pIX_D_RES_B_pIX_D()
{
    return RunThridLevel(TypeIR::IX);

    /*
#ifdef LISTING

    AddOpcode(RAM8(rPC));
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


int DecodeCommandDD()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelDD[index][0] == 0)
    {
        return 0;
    }

    return secondLevelDD[index][0]();
}


int RunCommandDD()
{
    int index = PCandInc();

    if(secondLevelDD[index][1] == 0)
    {
        return 0;
    }

    return secondLevelDD[index][1]();
}
