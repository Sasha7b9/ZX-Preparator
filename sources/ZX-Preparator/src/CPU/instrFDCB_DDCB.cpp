// 2022/05/23 17:32:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CPU/instrFDCB_DDCB.h"
#include "CPU/registers.h"
#include "Computer/RAM.h"
#include "CPU/instrShift.h"
#include <cstring>
#include <cstdio>



static TypeIR typeIR = IR_IX;


#define OPERAND (typeIR == IR_IX ? Operand_IX : Operand_IY)



int RLC_pIR_D_R(void)
{
    return RLC(OPERAND);
}


int RLC_pIR_D(void)
{
    return RLC(OPERAND);
}


int RRC_pIR_D_R(void)
{
    return 0;
}


int RRC_pIR_D(void)
{
    return 0;
}


int RL_pIR_D_R(void)
{
    return 0;
}


int RL_pIR_D(void)
{
    return 0;
}


int RR_pIR_D_R(void)
{
    return 0;
}


int RR_pIR_D(void)
{
    return 0;
}


int SLA_pIR_D_R(void)
{
    return 0;
}


int SLA_pIR_D(void)
{
    return 0;
}


int SRA_pIY_D_R(void)
{
    return 0;
}


int SRA_pIR_D(void)
{
    return 0;
}


int SLS_pIY_D_R(void)
{
    return 0;
}


int SLS_pIR_D(void)
{
    return 0;
}


int SRL_pIY_D_R(void)
{
    return 0;
}


int SRL_pIR_D(void)
{
    return 0;
}


int BIT_B_pIR_D(void)
{
    return 0;
}


int BIT_B_pIY(void)
{
    return 0;
}


int RES_B_pIY_D_R(void)
{
    return 0;
}


int RES_B_pIR_D(void)
{
    return 0;
}


int SET_B_pIR_D_R(void)
{
    return 0;
}


int SET_B_pIR_D(void)
{
    return 0;
}


#include "FuncThird.h"


int RunThridLevel(TypeIR type)
{
    typeIR = type;

    AddOpcode(RAM8(PC));
    PCandInc();
    AddOpcode(RAM8(PC));

    int index = PCandInc();

    if (thirdLevel[index][0] == 0)
    {
        return 0;
    }

    return thirdLevel[index][0]();
}
