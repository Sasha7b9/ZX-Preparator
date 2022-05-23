// 2022/05/23 17:29:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CPU/registers.h"
#include "defines.h"
#include "CPU/instr.h"
#include "Computer/RAM.h"
#include "CPU/DAA.h"
#include "Computer/ports.h"
#include "CPU/instrCB.h"
#include "CPU/instrDD.h"
#include "CPU/instrED.h"
#include "CPU/instrFD.h"
#include "CPU/instrShift.h"
#include <cstring>
#include <cstdio>


// 0 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int NOP_dec(void)
{
    AddAddress(PC);
    TACKTS = 4;
    strcpy(MNEMONIC, "NOP");
    return -1;
}

// 0 ------------------------------------------------------------------------------------------------------------------------------------------------------
static int NOP_run(void)
{
    return 4;
}



// 1 ------------------------------------------------------------------------------------------------------------------------------------------------------
static int LD_DD_NN_dec(void)
{
    uint8 valDD = prevPC;
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    AddAddress(PC + 2);
    uint16 NN = PC16andInc();

    sprintf(MNEMONIC, "LD %s,%04X", DD_45_Name(valDD), NN);

    return -1;
}

// 1 ------------------------------------------------------------------------------------------------------------------------------------------------------
static int LD_DD_NN_run(void)
{
    DD_45(prevPC) = PC16andInc();

    return 10;
}


// 2 ------------------------------------------------------------------------------------------------------------------------------------------------------
static int LD_pBC_A_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "LD [BC],A");
    return -1;
}


// 2 ------------------------------------------------------------------------------------------------------------------------------------------------------
int LD_pBC_A_run(void)
{
    RAM[BC] = rA;

    return 7;
}


// 3 ------------------------------------------------------------------------------------------------------------------------------------------------------
int INC_SS_dec(void)
{
    TACKTS = 6;
    AddAddress(PC);
    sprintf(MNEMONIC, "INC %s", SS_45_Name(prevPC));
    sprintf(TRANSCRIPT, "%s<-%s+1", SS_45_Name(prevPC), SS_45_Name(prevPC));
    return -1;
}


// 3 ------------------------------------------------------------------------------------------------------------------------------------------------------
int INC_SS_run(void)
{
    SS_45(prevPC) += 1;
    return 6;
}


// 4 ------------------------------------------------------------------------------------------------------------------------------------------------------
int INC_R_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "INC %s", R8_HI_Name(prevPC));
    return -1;
}


// 4 ------------------------------------------------------------------------------------------------------------------------------------------------------
int INC_R_run(void)
{
    uint8 before = R8_HI(prevPC);

    R8_HI(prevPC) += 1;

    uint8 after = R8_HI(prevPC);
    
    // S Z   H  PV N C
    // + + x + x v 0 .

    CALC_S(after);
    CALC_Z(after);
    CALC_H(before, after);
    LOAD_V(after == 0);
    RES_N;

    return 4;
}


// 5 ------------------------------------------------------------------------------------------------------------------------------------------------------
int DEC_R_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "DEC %s", R8_HI_Name(prevPC));
    return -1;
}


// 5 ------------------------------------------------------------------------------------------------------------------------------------------------------
int DEC_R_run(void)
{
    uint8 before = R8_HI(prevPC);

    R8_HI(prevPC) -= 1;

    uint8 after = R8_HI(prevPC);

    // S Z   H  PV N C
    // + + x + x v 1 .

    CALC_S(after);
    CALC_Z(after);
    CALC_H(before, after);
    LOAD_V(after == 0);
    SET_N;

    return 4;
}


// 6 ------------------------------------------------------------------------------------------------------------------------------------------------------
int LD_R_N_dec(void)
{
    AddAddress(PC + 1);
    AddOpcode(RAM8(PC));
    TACKTS = 7;

    uint8 valR = prevPC;
    uint8 N = PCandInc();

    sprintf(MNEMONIC, "LD %s,%02X", R8_HI_Name(valR), N);

    return -1;
}


// 6 ------------------------------------------------------------------------------------------------------------------------------------------------------
int LD_R_N_run(void)
{
    R8_HI(prevPC) = PCandInc();

    return 7;
}


// 7 ------------------------------------------------------------------------------------------------------------------------------------------------------
int RLCA_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "RLCA");
    return -1;
}


// 7 ------------------------------------------------------------------------------------------------------------------------------------------------------
int RLCA_run(void)
{
    return RLC(Operand_A);
}


int EX_AF_AFalt_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "EX AF,AF\'");
    return -1;
}


int EX_AF_AFlat_run(void)
{
    uint8 temp;

    EXCH(rA, Aalt);
    EXCH(rF, RFalt);

    return 4;

}


int ADD_HL_SS_dec(void)
{
    AddAddress(PC);
    TACKTS = 11;
    sprintf(MNEMONIC, "ADD HL,%s", SS_45_Name(prevPC));
    sprintf(TRANSCRIPT, "HL<-HL+%s", SS_45_Name(prevPC));
    strcat(FLAGS, "++XXXV0+");
    return -1;
}


int ADD_HL_SS_run(void)
{
    HL += SS_45(prevPC);

    // . . x x x . 0 +

    RES_N;

    // C WARN

    return 11;
}


int LD_A_pBC_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "LD A,[BC]");
    return -1;
}


int LD_A_pBC_run(void)
{
    rA = RAM[BC];

    return 7;
}


int DEC_SS_dec(void)
{
    TACKTS = 6;
    AddAddress(PC);

    sprintf(MNEMONIC, "DEC %s", SS_45_Name(prevPC));
    sprintf(TRANSCRIPT, "%s<-%s-1", SS_45_Name(prevPC), SS_45_Name(prevPC));

    return -1;
}


int DEC_SS_run(void)
{
    SS_45(prevPC) -= 1;

    return 6;
}


int RRCA_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "RRCA");
    return -1;
}


int RRCA_run(void)
{
    uint8 loBit = GET_nBIT(rA, 0);

    rA >>= 1;
    LOAD_nBIT(rA, 7, loBit);
    LOAD_C(loBit);

    // . . x 0 x . 0 +
    RES_H;
    RES_N;

    return 4;
}


int DJNZ_E_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    uint8 value = PCandInc();
    int shift = ((int)((int8)value)) + 2;
    uint newAddress = PC + shift - 2;
    sprintf(MNEMONIC, "DJNZ %dd (%04X)", shift, newAddress);
    AddAddress(newAddress);
    return -1;
}


int DJNZ_E_run(void)
{
    rB -= 1;

    uint8 delta = PCandInc();

    if(rB)
    {
        AddPC(delta);
        return 13;
    }

    return 8;
}


int LD_pDE_A_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "LD [DE],A");
    return -1;
}


int LD_pDE_A_run(void)
{
    RAM[DE] = rA;

    return 7;
}


int RLA_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "RLA");
    return -1;
}


int RLA_run(void)
{
    uint8 hiBit = GET_nBIT(rA, 7);
    uint8 oldCY = CF;

    rA <<= 1;
    LOAD_C(hiBit);
    LOAD_nBIT(rA, 0, oldCY);

    // . . x 0 x . 0 +
    RES_H;
    RES_N;

    return 4;
}


static void WriteE(char *name)
{
    AddOpcode(RAM8(PC));
    if(name[0])     // For JR E not write address
    {
        AddAddress(PC + 1);
    }

    uint8 value = PCandInc();

    int shift = ((int)((int8)value)) + 2;
    uint newAddress = PC + shift - 2;

    sprintf(MNEMONIC, "JR %s%dd (%04X)", name, shift, newAddress);

    AddAddress(newAddress);
}


int JR_E_dec(void)
{
    WriteE("");

    return -1;
}


int JR_E_run(void)
{
    AddPC(PCandInc());

    return 12;
}


int LD_A_pDE_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "LD A,[DE]");
    return -1;
}


int LD_A_pDE_run(void)
{
    rA = RAM[DE];

    return 7;
}


int RRA_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "RRA");
    return -1;
}


int RRA_run(void)
{
    uint8 loBit = GET_nBIT(rA, 0);
    uint8 oldCY = CF;

    rA >>= 1;
    LOAD_nBIT(rA, 7, oldCY);
    LOAD_C(loBit);

    // . . x 0 x . 0 +
    RES_H;
    RES_N;

    return 4;
}


int JR_NZ_E_dec(void)
{
    WriteE("NZ,");

    return -1;
}


int JR_NZ_E_run(void)
{
    uint8 address = PCandInc();
    if(ZF == 0)
    {
        AddPC(address);
        return 12;
    }
    return 7;
}


int LD_pNN_HL_dec(void)
{
    TACKTS = 16;

    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));

    uint16 NN = PC16andInc();
    AddAddress(PC);

    sprintf(MNEMONIC, "LD [%04X],HL", NN);
    sprintf(TRANSCRIPT, "[%04X]<-HL", NN);

    return -1;
}


int LD_pNN_HL_run(void)
{
    uint16 address = PC16andInc();

    RAM[address] = rL;
    RAM[address + 1] = rH;

    return 16;
}


int JR_Z_E_dec(void)
{
    WriteE("Z,");

    return -1;
}


int JR_Z_E_run(void)
{
    uint8 address = PCandInc();
    if(ZF)
    {
        AddPC(address);
        return 12;
    }

    return 7;
}


int LD_HL_pNN_dec(void)
{
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    uint16 address = PC16andInc();
    AddAddress(PC);
    TACKTS = 16;

    sprintf(MNEMONIC, "LD HL,[%04X]", address);
    sprintf(TRANSCRIPT, "HL<-[%04X]", address);

    return -1;
}


int LD_HL_pNN_run(void)
{
    HL = PC16andInc();

    return 16;
}


int CPL_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "CPL");
    return -1;
}


int CPL_run(void)
{
    rA = ~rA;

    // . . x + x . 1 .

    // H WARN

    SET_N;

    return 4;
}


static int JR_NC_E_dec(void)
{
    WriteE("NC,");

    return -1;
}


static int JR_NC_E_run(void)
{
    uint8 address = PCandInc();
    if(CF == 0)
    {
        AddPC(address);
        return 12;
    }

    return 7;
}


static int LD_pNN_A_dec(void)
{
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    sprintf(MNEMONIC, "LD (%04x),A", PC16andInc());
    AddAddress(PC);
    return -1;
}


static int LD_pNN_A_run(void)
{
    RAM[PC16andInc()] = rA;

    return 13;
}


static int INC_pHL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "INC [HL]");
    return -1;
}


static int INC_pHL_run(void)
{
    pHL += 1;

    // + + x + x v 0 .

    RES_N;

    /*
    */

    return 7;
}


static int DEC_pHL_dec(void)
{
    TACKTS = 11;
    AddAddress(PC);

    strcpy(MNEMONIC, "DEC (HL)");
    strcpy(TRANSCRIPT, "(HL)<-(HL)+1");
    strcpy(FLAGS, "++X+XV0.");

    return -1;
}


static int DEC_pHL_run(void)
{
    pHL += 1;

    // + + x + x v 1 .

    SET_N;

    /**/

    return 7;
}


static int LD_pHL_N_dec(void)
{
    TACKTS = 10;
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    uint8 value = PCandInc();

    sprintf(MNEMONIC, "LD (HL),%2x", value);
    sprintf(TRANSCRIPT, "(HL)<-%2x", value);

    return -1;
}


static int LD_pHL_N_run(void)
{
    pHL = PCandInc();

    return 10;
}


static int SCF_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "SCF");
    return -1;
}


static int SCF_run(void)
{
    SET_C;

    // . . x 0 x . 0 +

    RES_H;
    RES_N;

    return 4;
}


static int JR_C_E_dec(void)
{
    WriteE("C,");

    return -1;
}


static int JR_C_E_run(void)
{
    uint8 address = PCandInc();
    if(CF)
    {
        AddPC(address);
        return 12;
    }

    return 7;
}


static int LD_A_pNN_dec(void)
{
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    AddAddress(PC + 2);

    sprintf(MNEMONIC, "LD A,[%04X]", PC16andInc());

    return -1;
}


static int LD_A_pNN_run(void)
{
    rA = RAM[PC16andInc()];

    return 13;
}


static int CCF_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "CCF");
    return -1;
}


static int CCF_run(void)
{
    if(CF)
    {
        RES_C;
    }
    else
    {
        SET_C;
    }

    // . . x x x . 0 +

    RES_N;

    return 4;
}


static int LD_R_R_dec(void)
{
    AddAddress(PC);
    TACKTS = 4;

    sprintf(MNEMONIC, "LD %s,%s", R8_HI_Name(prevPC), R8_LO_Name(prevPC));
    sprintf(TRANSCRIPT, "%s<-%s", R8_HI_Name(prevPC), R8_LO_Name(prevPC));

    return -1;
}


static int LD_R_R_run(void)
{
    R8_HI(prevPC) = R8_LO(prevPC);

    return 4;
}


static int LD_R_pHL_dec(void)
{
    TACKTS = 7;
    AddAddress(PC);

    sprintf(MNEMONIC, "LD %s,[HL]", R8_HI_Name(prevPC));
    sprintf(TRANSCRIPT, "%s<-[HL]", R8_HI_Name(prevPC));

    return -1;
}


static int LD_R_pHL_run(void)
{
    R8_HI(prevPC) = pHL;

    return 7;
}


static int LD_pHL_R_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "LD [HL],%s", R8_LO_Name(prevPC));
    return -1;
}


static int LD_pHL_R_run(void)
{
    pHL = R8_LO(prevPC);

    return 7;
}


static int HALT_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "HALT");
    return -1;
}


static int HALT_run(void)
{
    return 0;   // WARN There halt CPU
}


static int ADD_A_R_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "LD A,%s", R8_LO_Name(prevPC));
    return -1;
}


static int ADD_A_R_run(void)
{
    rA += R8_LO(prevPC);

    return 4;
}


static int ADD_A_pHL_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "ADD A,[HL]");
    return -1;
}


static int ADD_A_pHL_run(void)
{
    rA += pHL;

    return 7;
}


static int ADC_A_S_dec(void)
{
    AddAddress(PC + 1);
    AddOpcode(RAM8(PC));
    sprintf(MNEMONIC, "ADC A,%s", R8_LO_Name(prevPC));
    return -1;
}


static int ADC_A_S_run(void)
{
    rA += R8_LO(prevPC) + CF;

    return 4;
}


static int ADC_A_pHL_run(void)
{
    rA += pHL + CF;

    return 7;
}


static int SUB_S_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "SUB %s", R8_LO_Name(prevPC));
    return -1;
}


static int SUB_S_run(void)
{
    rA -= R8_LO(prevPC);

    return 4;
}


static int SUB_pHL_run(void)
{
    rA -= pHL;

    return 7;
}


static int SBC_A_S_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "SBC A,%s", R8_LO_Name(prevPC));
    return -1;
}


static int SBC_A_S_run(void)
{
    rA -= R8_LO(prevPC) - CF;

    return 4;
}


static int SBC_A_pHL_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "SBC A,[HL]");
    return -1;
}


static int SBC_A_pHL_run(void)
{
    rA -= pHL - CF;

    return 7;
}


static int AND_S_dec(void)
{
    TACKTS = 4;
    AddAddress(PC);

    sprintf(MNEMONIC, "AND %s", R8_LO_Name(prevPC));
    sprintf(TRANSCRIPT, "A<-A&%s", R8_LO_Name(prevPC));
    strcpy(FLAGS, "++X1XP00");

    return -1;
}


static int AND_S_run(void)
{
    rA &= R8_LO(prevPC);

    return 4;
}


static int AND_pHL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "AND [HL]");
    return -1;
}


static int AND_pHL_run(void)
{
    rA &= pHL;

    return 7;
}


static int XOR_S_dec(void)
{
    sprintf(MNEMONIC, "XOR %s", R8_LO_Name(prevPC));
    sprintf(TRANSCRIPT, "A<-A(+)%s", R8_LO_Name(prevPC));
    strcpy(FLAGS, "++X0XP00");
    TACKTS = 4;
    AddAddress(PC);
    return -1;
}


static int XOR_S_run(void)
{
    rA ^= R8_LO(prevPC);

    return 4;
}


static int XOR_pHL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "XOR [HL]");
    return -1;
}


static int XOR_pHL_run(void)
{
    rA ^= pHL;

    return 7;
}


static int OR_S_dec(void)
{
    AddAddress(PC);

    sprintf(MNEMONIC, "OR %s", R8_LO_Name(prevPC));

    return -1;
}


static int OR_S_run(void)
{
    rA |= R8_LO(prevPC);

    return 4;
}


static int OR_pHL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "OR [HL]");
    return -1;
}


static int OR_pHL_run(void)
{
    rA |= pHL;

    return 7;
}


static int CP_S_dec(void)
{
    AddAddress(PC);
    TACKTS = 4;

    strcpy(FLAGS, "++X+XV1+");
    sprintf(MNEMONIC, "CP %s", R8_LO_Name(prevPC));
    sprintf(TRANSCRIPT, "A ~ %s", R8_LO_Name(prevPC));
    sprintf(COMMENT, "compare");

    return -1;
}


static int CP_S_run(void)
{
    uint8 value = R8_LO(prevPC);

    SET_N;

    if(rA == value)
    {
        SET_Z;
    }
    else
    {
        RES_Z;
    }

    if(rA < value)
    {
        SET_C;
        SET_S;
    }
    else
    {
        RES_C;
        RES_S;
    }

    if((value & 0x0f) > (rA & 0x0f))
    {
        SET_H;
    }
    else
    {
        RES_H;
    }

    return 4;
}


static int CP_pHL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "CP [HL]");
    return -1;
}


static int CP_pHL_run(void)
{
    return 7;
}


static char* Cond_Name(uint8 value)
{
    const char* names[8] =
    {
        "NZ",
        "Z",
        "NC",
        "C",
        "PO",
        "PE",
        "P",
        "M"
    };

    return (char*)names[CC(value)];
}


static int RET_CC_dec(void)
{
    AddAddress(PC);

    sprintf(MNEMONIC, "RET %s", Cond_Name(prevPC));

    return -1;
}


static bool Cond_NZ(void)
{
    return (ZF == 0);
}


static bool Cond_Z(void)
{
    return (ZF == 1);
}


static bool Cond_NC(void)
{
    return (CF == 0);
}


static bool Cond_C(void)
{
    return (CF == 1);
}


static bool Cond_PO(void)
{
    return (PF == 0);
}


static bool Cond_PE(void)
{
    return (PF == 1);
}


static bool Cond_P(void)
{
    return (SF == 0);
}


static bool Cond_M(void)
{
    return (SF == 1);
}


static bool Cond(uint8 value)
{
    const pFuncBV funcs[8] =
    {
        Cond_NZ,
        Cond_Z,
        Cond_NC,
        Cond_C,
        Cond_PO,
        Cond_PE,
        Cond_P,
        Cond_M
    };

    uint8 cond = CC(value);

    return funcs[cond]();
}


static int RET_dec(void)
{
    strcpy(MNEMONIC, "RET");
    return -1;
}


static int RET_run(void)
{
    PC = RAM16(SP);
    return 10;
}


static int RET_CC_run(void)
{
    if(Cond(prevPC))
    {
        RET_run();
        return 11;
    }
    return 5;
}


static int POP_QQ_dec(void)
{
    AddAddress(PC);

    sprintf(MNEMONIC, "POP %s", QQ_45_Name(prevPC));

    return -1;
}


static int POP_QQ_run(void)
{
    QQ_45(prevPC) = RAM[SP] + RAM[SP + 1] * 256;
    SP += 2;

    return 10;
}


static int JP_CC_NN_dec(void)
{
    char *nameCC = Cond_Name(prevPC);

    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));

    uint16 NN = PC16andInc();

    sprintf(MNEMONIC, "JP %s,%04x", nameCC, NN);

    AddAddress(PC);
    AddAddress(NN);

    return -1;
}


static int JP_CC_NN_run(void)
{
    bool cond = Cond(prevPC);

    uint16 address = PC16andInc();

    if(cond)
    {
        PC = address;
    }

    return 10;
}


static int JP_NN_dec(void)
{
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));

    uint16 address = PC16andInc();

    AddAddress(address);

    sprintf(MNEMONIC, "JP %04X", address);
    sprintf(TRANSCRIPT, "PC<-%04X", address);

    TACKTS = 10;

    return -1;
}


static int JP_NN_run(void)
{
    PC = PC16andInc();

    return 10;
}


static int CALL_NN_dec(void)
{
    TACKTS = 17;
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));

    uint16 NN = PC16andInc();
    AddAddress(NN);
    AddAddress(PC);

    sprintf(MNEMONIC, "CALL %04X", NN);
    sprintf(TRANSCRIPT, "(SP-2)<-PC, PC<-%04X", NN);

    return -1;
}


static int CALL_NN_run(void)
{
    uint16 valuePC = ValuePC();

    RAM[SP - 1] = (uint8)(valuePC >> 8);
    RAM[SP - 2] = (uint8)(valuePC);
    PC = PC16andInc();

    return 17;
}


static int CALL_CC_NN_dec(void)
{
    AddOpcode(RAM8(PC));
    AddOpcode(RAM8(PC + 1));
    AddAddress(PC + 2);
    uint8 value = prevPC;
    uint16 newAddress = PC16andInc();
    AddAddress(newAddress);
    sprintf(MNEMONIC, "CALL %s,%04X", Cond_Name(value), newAddress);
    return -1;
}


static int CALL_CC_NN_run(void)
{
    if(Cond(prevPC))
    {
        return CALL_NN_run();
    }

    PC16andInc();
    return 10;
}


static int PUSH_QQ_dec(void)
{
    TACKTS = 11;
    AddAddress(PC);

    sprintf(MNEMONIC, "PUSH %s", QQ_45_Name(prevPC));
    sprintf(TRANSCRIPT, "(SP-2)<-%s, SP<-SP-2", QQ_45_Name(prevPC));

    return -1;
}


static int PUSH_QQ_run(void)
{
    RAM[SP - 2] = (uint8)QQ_45(prevPC);
    RAM[SP - 1] = (uint8)(QQ_45(prevPC) >> 8);
    SP -= 2;

    return 11;
}


static int ADD_A_N_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "ADD A,0x%02x", PCandInc());
    return -1;
}


static int ADD_A_N_run(void)
{
    rA += PCandInc();

    return 7;
}


const uint8 rstP[8] =
{
    0x00,
    0x08,
    0x10,
    0x18,
    0x20,
    0x28,
    0x30,
    0x38
};


static int RST_P_dec(void)
{
    int address = rstP[(prevPC >> 3) & 7];
    if(address != 0x28 && address != 0x08) // ��� ������������ ����� ������� ������ �������� �����-�� ������, ������� �� ��������� � ������� ������ �������
    {
        AddAddress(PC);
    }
    AddAddress(address);
    sprintf(MNEMONIC, "RST %02X", address);

    return -1;
}


static int RST_P_run(void)
{
    uint8 T = (prevPC >> 3) & 7;

    uint16 valuePC = ValuePC();

    RAM[SP - 1] = (uint8)(valuePC >> 8);
    RAM[SP - 2] = (uint8)valuePC;

    PC = rstP[T];

    return 11;
}


static int ADC_A_N_dec(void)
{
    AddAddress(PC + 1);
    AddOpcode(RAM8(PC));
    sprintf(MNEMONIC, "ADC A,%02X", PCandInc());
    return -1;
}


static int ADC_A_N_run(void)
{
    rA += PCandInc() + CF;

    return 7;
}


static int OUT_pN_A_dec(void)
{
    TACKTS = 11;
    AddAddress(PC + 1);
    AddOpcode(RAM8(PC));
    uint8 N = PCandInc();

    sprintf(MNEMONIC, "OUT (%02x), A", N);
    sprintf(TRANSCRIPT, "(%02x) <- A", N);
    sprintf(COMMENT, "N - A0...A7, A - A8...A15");

    return -1;
}


static int OUT_pN_A_run(void)
{
    WritePort(PCandInc(), rA);

    return 11;
}


static int SUB_N_dec(void)
{
    AddAddress(PC + 1);
    AddOpcode(RAM8(PC));
    sprintf(MNEMONIC, "SUB %02X", PCandInc());
    return -1;
}


static int SUB_N_run(void)
{
    rA -= PCandInc();

    return 7;
}


static int EXX_dec(void)
{
    TACKTS = 4;
    AddAddress(PC);
    strcpy(MNEMONIC, "EXX");
    strcpy(TRANSCRIPT, "BC<->BC', DE<->DE', HL<->HL'");

    return -1;
}


static int EXX_run(void)
{
    uint8 temp;

    EXCH(rB, Balt);
    EXCH(rC, RCalt);
    EXCH(rD, RDalt);
    EXCH(rE, REalt);
    EXCH(rH, RHalt);
    EXCH(rL, RLalt);

    return 4;
}


static int IN_A_pN_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "IN A,(%02X)", PCandInc());
    return -1;
}


static int IN_A_pN_run(void)
{
    rA = ReadPort(PCandInc());

    return 11;
}


static int SBC_A_N_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "SBC A,%02X", PCandInc());
    return -1;
}


static int SBC_A_N_run(void)
{
    rA -= PCandInc() - CF;

    return 7;
}


static int EX_pSP_HL_dec(void)
{
    AddAddress(PC);
    sprintf(MNEMONIC, "EX [SP],HL");
    return -1;
}


static int EX_pSP_HL_run(void)
{
    uint8 temp;

    EXCH(rH, RAM[SP + 1]);
    EXCH(rL, RAM[SP]);

    return 9;
}


static int AND_N_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "AND %02X", PCandInc());
    return -1;
}


static int AND_N_run(void)
{
    rA &= PCandInc();

    return 7;
}


static int JP_pHL_dec(void)
{
    strcpy(MNEMONIC, "JP [HL]");
    return -1;
}


static int JP_pHL_run(void)
{
    PC = HL;

    return 4;
}


static int EX_DE_HL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "EX DE,HL");
    return -1;
}


static int EX_DE_HL_run(void)
{
    uint16 temp;

    EXCH(DE, HL);

    return 4;
}


static int XOR_N_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "XOR %02X", PCandInc());
    return -1;
}


static int XOR_N_run(void)
{
    rA ^= PCandInc();

    return 7;
}


static int DI_dec(void)
{
    strcpy(MNEMONIC, "DI");
    strcpy(TRANSCRIPT, "IFF1<-0, IFF2<-0");
    strcpy(COMMENT, "reset the maskable interrupt");
    TACKTS = 4;

    AddAddress(PC);

    return -1;
}


static int DI_run(void)
{
    IFF1 = 0;
    IFF2 = 0;

    return 4;
}


static int OR_N_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);
    sprintf(MNEMONIC, "OR %02X", PCandInc());
    return -1;
}


static int OR_N_run(void)
{
    rA |= PCandInc();

    return 7;
}


static int LD_SP_HL_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "LD SP,HL");
    return -1;
}


static int LD_SP_HL_run(void)
{
    SP = HL;

    return 6;
}


static int EI_dec(void)
{
    AddAddress(PC);
    strcpy(MNEMONIC, "EI");
    return -1;
}


static int EI_run(void)
{
    IFF1 = 1;
    IFF2 = 1;

    return 4;
}


static int CP_N_dec(void)
{
    AddOpcode(RAM8(PC));
    AddAddress(PC + 1);

    sprintf(MNEMONIC, "CP %02X", PCandInc());
    
    return -1;
}


static int CP_N_run(void)
{
    return 7;
}


#include "FuncFirst.h"


int RunCommand(void)
{
    return firstLevel[PCandInc()][1]();
}


int DecodeCommand(void)
{
    AddOpcode(RAM8(PC));

    return firstLevel[PCandInc()][0]();
}
