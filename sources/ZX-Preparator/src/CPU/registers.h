// 2022/05/23 17:34:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Computer/RAM.h"
#include "CPU/utils.h"
#include "Emulator/Emulator.h"


enum REG_8
{
    RB_ = 0,
    RC_ = 1,
    RD_ = 2,
    RE_ = 3,
    RH_ = 4,
    RL_ = 5,
    pHL_ = 6,
    RA_ = 7
};


enum REG_QQ
{
    RQQ_BC_ = 0,
    RQQ_DE_ = 1,
    RQQ_HL_ = 2,
    RQQ_AF_ = 3
};


// Return value from RAM, pointed to PC, and increment PC
void AddPC(uint8 delta);
uint8 PCandInc(void);
uint16 PC16andInc(void);
uint16 ValuePC(void);


extern uint8 prevPC;
extern pFuncpU8V funcsReg8[8];
extern pFuncpU16V funcsRegDD[4];
extern pFuncpU16V funcsRegPP[4];
extern REGS regs;
extern REGS regsAlt;
extern uint8 regI;
extern uint8 regR;
extern uint8 iff1;
extern uint8 iff2;
extern uint8 imfA;
extern uint8 imfB;


#define BC      regs.r16[0]
#define DE      regs.r16[1]
#define HL      regs.r16[2]
#define AF      regs.r16[3]
#define SP      regs.r16[4]
#define IX      regs.r16[5]
#define IY      regs.r16[6]
#define PC      regs.r16[7]

#define rA      regs.r8[7]
#define rB      regs.r8[1]
#define rC      regs.r8[0]
#define rD      regs.r8[3]
#define rE      regs.r8[2]
#define rL      regs.r8[4]
#define rH      regs.r8[5]
#define pHL     RAM[regs.r16[2]]
#define rF      (regs.r8[6])
#define I       regI
#define R       regR

#define SF      (GET_nBIT(rF, 7))
#define SET_S   (rF |= 0x80)
#define RES_S   (rF &= ~0x80)
#define CALC_S(value)   if(value & 0x80) (rF |= 0x80); else (rF &= ~(0x80));

#define ZF      (GET_nBIT(rF, 6))
#define SET_Z   (rF |= 0x40)
#define RES_Z   (rF &= ~0x40)
#define CALC_Z(value)   if(value) (rF &= ~(0x40)); else (rF |= 0x40);
//#define LOAD_Z(value)   if(value) SET_Z; else RES_Z;

#define HF      (GET_nBIT(rF, 4))
#define SET_H   (rF |= 0x10)
#define RES_H   (rF &= ~0x10)
#define CALC_H(before, after) if(CalculateH(before, after)) (rF |= 0x10); else (rF &= ~(0x10));

#define PF      (GET_nBIT(rF, 2))
#define SET_P   (rF |= 4)
#define RES_P   (RES_nBIT(rF, 2))
#define CALC_P(value)  (IsEven(value))

#define VF      (GET_nBIT(rF, 2))
#define SET_V   (SET_nBIT(rF, 2))
#define RES_V   (RES_nBIT(rF, 2))
#define LOAD_V(load) if(load) SET_V; else RES_V;

#define NF      (GET_nBIT(rF, 1))
#define SET_N   (rF |= 0x02)
#define RES_N   (rF &= ~(0x02))

#define CF      (GET_nBIT(rF, 0))
#define SET_C   (rF |= 0x01)
#define RES_C   (rF &= ~0x01)
#define LOAD_C(value)  if(value) SET_C; else RES_C


#define Aalt    regsAlt.r8[7]
#define Balt    regsAlt.r8[1]
#define RCalt   regsAlt.r8[0]
#define RDalt   regsAlt.r8[3]
#define REalt   regsAlt.r8[2]
#define RLalt   regsAlt.r8[4]
#define RHalt   regsAlt.r8[5]
#define RFalt   regsAlt.r8[6]

#define R8_HI(value)    (*funcsReg8[(value >> 3) & 7]())
#define R8_LO(value)    (*funcsReg8[value & 7]())
#define pR8_LO(value)   (&(*funcsReg8[value & 7]()))

#define DD_45(value)    (*funcsRegDD[(value >> 4) & 3]())
#define SS_45(value)    (*funcsRegDD[(value >> 4) & 3]())
#define QQ_45(value)    (regs.r16[(value >> 4) & 3])
#define PP_45(value)    (*funcsRegPP[(value >> 4) & 3]())


#define CC(value) ((value >> 3) & 7)
#define CC_NZ   0
#define CC_Z    1
#define CC_NC   2
#define CC_C    3
#define CC_PO   4
#define CC_PE   5
#define CC_P    6
#define CC_M    7

#define IFF1    iff1
#define IFF2    iff2

#define EXCH(r1, r2)    \
    temp = r1;          \
    r1 = r2;            \
    r2 = temp;

char* R8_HI_Name(uint8 value);
char* R8_LO_Name(uint8 value);
char* DD_45_Name(uint8 value);
char* SS_45_Name(uint8 value);
char* QQ_45_Name(uint8 value);
char* PP_45_Name(uint8 value);


typedef enum
{
    IR_IX,
    IR_IY
} TypeIR;
