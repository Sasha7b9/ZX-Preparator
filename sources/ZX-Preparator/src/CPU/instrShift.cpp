// 2022/05/23 17:33:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CPU/registers.h"
#include "defines.h"
#include "CPU/instrShift.h"
#include <cstring>
#include <cstdio>



static int RLC_RRC(TypeOperand type, uint8 byte)
{
#define RUN_RLC_RRC()                           \
    if(byte == 0x06)                            \
    {                                           \
        uint8 hiBit = GET_nBIT(*pOperand, 7);   \
        (*pOperand) <<= 1;                      \
        LOAD_nBIT(*pOperand, 0, hiBit);         \
        LOAD_C(hiBit);                          \
    }                                           \
    else                                        \
    {                                           \
        uint8 loBit = GET_nBIT(*pOperand, 0);   \
        (*pOperand) >>= 1;                      \
        LOAD_nBIT(*pOperand, 7, loBit);         \
        LOAD_C(loBit);                          \
    }                                           \
    RES_H;                                      \
    RES_N;

    if (type == Operand_A)
    {
        uint8 *pOperand = &rA;

        RUN_RLC_RRC();

        return 4;
    }
    else if (type == Operand_Reg8)
    {
        uint8 *pOperand = pR8_LO(prevPC);

        RUN_RLC_RRC();

        return 8;
    }
    else if (type == Operand_pHL)
    {
        uint8 *pOperand = &pHL;

        RUN_RLC_RRC();

        return 15;
    }
    else if (type == Operand_IX || type == Operand_IY)
    {
        int index = ((type == Operand_IX) ? rIX : rIY) + RAM[rPC - 2];

        uint8 *pOperand = &(RAM[index]);

        RUN_RLC_RRC();

        if (RAM[rPC - 1] != byte)
        {
            R8_LO(RAM[rPC - 1]) = *pOperand;
        }

        return 23;
    }

    return 0;
}


int RLC(TypeOperand type)
{
    return RLC_RRC(type, 0x06);
}


int RRC(TypeOperand type)
{
    return RLC_RRC(type, 0x0e);
}
