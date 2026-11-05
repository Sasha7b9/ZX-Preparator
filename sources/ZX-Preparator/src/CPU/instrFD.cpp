// 2022/05/23 17:32:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CPU/registers.h"
#include "defines.h"
#include "CPU/instrFD.h"
#include "Computer/ports.h"
#include "Computer/RAM.h"
#include "CPU/instrFDCB_DDCB.h"
#include <cstring>
#include <cstdio>



int INC_pIY_D_dec()
{
    AddOpcode(RAM8(rPC));
    sprintf(MNEMONIC, "INC [IY+%02X]", PCandInc());
    return -1;
}
int INC_pIY_D_run()
{
    return 0;
}


int DEC_pIY_D_dec()
{
    AddOpcode(RAM8(rPC));
    sprintf(MNEMONIC, "DEC (IY+%d)", PCandInc());
    return -1;
}
int DEC_pIY_D_run()
{
    return 0;
}


int LD_pIY_D_R_dec()
{
    AddOpcode(RAM8(rPC));
    char *reg = R8_LO_Name(prevPC);
    sprintf(MNEMONIC, "LD (IY+%d), %s", PCandInc(), reg);
    return -1;
}
int LD_pIY_D_R_run()
{
    return 0;
}


int LD_IY_NN_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    uint16 NN = PC16andInc();
    sprintf(MNEMONIC, "LD IY, %04x", NN);
    return -1;
}
int LD_IY_NN_run()
{
    return 0;
}


int LD_IY_pNN_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    uint16 NN = PC16andInc();
    sprintf(MNEMONIC, "LD IY, [%u]", NN);
    return -1;
}
int LD_IY_pNN_run()
{
    return 0;
}


int ADD_IY_SS_dec()
{
    sprintf(MNEMONIC, "ADD IX, %s", SS_45_Name(prevPC));
    sprintf(TRANSCRIPT, "IY<-IY+%s", SS_45_Name(prevPC));
    return -1;
}
int ADD_IY_SS_run()
{
    return 0;
}


int DEC_IY_dec()
{
    strcpy(MNEMONIC, "DEC IY");
    strcpy(TRANSCRIPT, "IY<-IY-1");
    return -1;
}
int DEC_IY_run()
{
    return 0;
}


int INC_IY_dec()
{
    strcpy(MNEMONIC, "INC IY");
    strcpy(TRANSCRIPT, "IY<-IY+1");
    return -1;
}
int INC_IY_run()
{
    return 0;
}


int DecodeCommandFDCB_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    uint8 valD = PCandInc();
    uint8 valBit = PCandInc();
    uint8 bit = (valBit >> 3) & 7;

    if ((valBit & 0xc7) == 0xc6)       // SET B, (IY + D)
    {
        sprintf(MNEMONIC, "SET %d,[IY+%02X]", bit, valD);
        return -1;
    }
    else if ((valBit & 0xc7) == 0x86)  // RES B, (IY + D)
    {
        sprintf(MNEMONIC, "RES %d,[IY+%02X]", bit, valD);
        return -1;
    }
    else if ((valBit & 0xc7) == 0x46)  // BIT B, (IY + D)
    {
        sprintf(MNEMONIC, "BIT %d,[IY+%02X]", bit, valD);
        return -1;
    }

    return 1;
}
int DecodeCommandFDCB_run()
{
    return 0;
}


int LD_pIY_D_N_dec()
{
    uint8 valD = PCandInc();
    uint8 valN = PCandInc();
    AddOpcode(valD);
    AddOpcode(valN);
    sprintf(MNEMONIC, "LD [IY + %02X], #%02X", valD, valN);
    return -1;
}
int LD_pIY_D_N_run()
{
    return 0;
}


int SUB_pIY_D_dec()
{
    AddOpcode(RAM8(rPC));
    sprintf(MNEMONIC, "SUB (IY+0x%02x)", PCandInc());
    return -1;
}
int SUB_pIY_D_run()
{
    return 0;
}


int LD_R_pIY_D_dec()
{
    uint8 valReg = prevPC;
    AddOpcode(RAM8(rPC));
    sprintf(MNEMONIC, "LD %s, [IY + %02X]", R8_HI_Name(valReg), PCandInc());
    return -1;
}
int LD_R_pIY_D_run()
{
    return 0;
}


int ADD_A_pIY_D_dec()
{
    AddOpcode(RAM8(rPC));
    sprintf(MNEMONIC, "ADD A,[IY+%02X]", prevPC);
    return -1;
}
int ADD_A_pIY_D_run()
{
    return 0;
}


int CP_pIY_D_dec()
{
    AddOpcode(RAM8(rPC));
    sprintf(MNEMONIC, "CP [IY+%02X]", PCandInc());
    return -1;
}
int CP_pIY_D_run()
{
    return 0;
}


int XOR_pIY_D_dec()
{
    AddOpcode(RAM8(rPC + 1));
    sprintf(MNEMONIC, "XOR [IY+%02X]", PCandInc());
    return -1;
}
int XOR_pIY_D_run()
{
    return 0;
}


int PUSH_IY_dec()
{
    sprintf(MNEMONIC, "PUSH IY");
    return -1;
}
int PUSH_IY_run()
{
    return 0;
}


int POP_IY_dec()
{
    sprintf(MNEMONIC, "POP IY");
    return -1;
}
int POP_IY_run()
{
    return 0;
}


#include "FuncSecondFD.h"


int RunCommandFD()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelFD[index][1] == 0)
    {
        return 0;
    }

    return secondLevelFD[index][1]();
}


int DecodeCommandFD()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelFD[index][0] == 0)
    {
        return 0;
    }

    return secondLevelFD[index][0]();
}
