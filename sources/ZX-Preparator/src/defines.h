// 2022/04/29 14:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef const char *pchar;
typedef unsigned char uchar;
typedef unsigned char uint8;
typedef unsigned short uint16;
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

void AddAddress(uint address);
void AddOpcode(uint8 code);
bool CalculateH(uint8 before, uint8 after); // ���������� ����� ��������������� ��������
