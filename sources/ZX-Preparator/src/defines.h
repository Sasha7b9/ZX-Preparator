// 2022/04/29 14:52:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#pragma warning(push)
#pragma warning(disable : 4668 5267)
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/textfile.h>
#include <wx/splitter.h>
#pragma warning(pop)



typedef const char        *pchar;
typedef unsigned char      uchar;
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef signed short       int16;
typedef unsigned int       uint;
typedef signed char        int8;
typedef unsigned long long uint64;
typedef signed long long   int64;


typedef uint8 *(*pFuncpU8V)();
typedef uint16 *(*pFuncpU16V)();
typedef int(*pFuncIV)();
typedef bool(*pFuncBV)();


struct InfoStruct;
extern InfoStruct   *out;
extern wxConfigBase *g_config;
extern wxFileConfig *g_file_config;


#define TACKTS      out->tackts
#define FLAGS       out->flags
#define COMMENT     out->comment
#define MNEMONIC    out->mnemonic
#define TRANSCRIPT  out->transcript


#define GET_nBIT(value, bit) (((value) & (1 << (bit))) >> (bit))
#define SET_nBIT(value, bit) ((value) | (1 << (bit)))
#define RES_nBIT(value, bit) ((value) & (~(1 << (bit))))
#define LOAD_nBIT(value, bit, vBit) if(vBit) ((value) |= (1 << (bit))); else ((value) &= (~(1 << (bit))))


void AddAddress(uint address);
void AddOpcode(uint8 code);
bool CalculateH(uint8 before, uint8 after); // Вычисление флага дополнительного переноса
