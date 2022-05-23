// 2022/05/23 17:33:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef enum
{
    Operand_A,
    Operand_Reg8,
    Operand_pHL,
    Operand_IX,
    Operand_IY
} TypeOperand;


int RLC(TypeOperand type);

int RRC(TypeOperand type);
