// 2022/04/29 14:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef const char *pchar;
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint;
typedef signed char int8;


typedef uint8 *(*pFuncpU8V)();
typedef uint16 *(*pFuncpU16V)();
typedef int(*pFuncIV)();


struct OutStruct;
extern OutStruct *out;


#define TACKTS      out->tackts
#define FLAGS       out->flags
#define COMMENT     out->comment
#define MNEMONIC    out->mnemonic
#define TRANSCRIPT  out->transcript


#define GET_nBIT(value, bit) (((value) & (1 << bit)) >> bit)
#define SET_nBIT(value, bit) ((value) | (1 << bit))
#define RES_nBIT(value, bit) ((value) & (~(1 << bit)))
#define LOAD_nBIT(value, bit, vBit) if(vBit) ((value) |= (1 << bit)); else ((value) &= (~(1 << bit)))


void AddAddress(uint address);
void AddOpcode(uint8 code);
bool CalculateH(uint8 before, uint8 after); // Вычисление флага дополнительного переноса
