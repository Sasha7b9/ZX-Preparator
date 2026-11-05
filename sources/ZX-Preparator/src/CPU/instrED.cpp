// 2022/05/23 17:32:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CPU/registers.h"
#include "defines.h"
#include "CPU/instrED.h"
#include "Computer/ports.h"
#include "Computer/RAM.h"
#include <cstring>
#include <cstdio>



int LD_A_I()
{
    return 9;   // WARN
}


int LD_I_A_dec()
{
    TACKTS = 9;
    AddAddress(rPC);

    strcpy(MNEMONIC, "LD I,A");
    strcpy(TRANSCRIPT, "I<-A");
    strcpy(COMMENT, "I - Interrupt Control Vector Register");

    return -1;
}


int LD_I_A_run()
{
    return 0;   // WARN
}


int LD_A_R_dec()
{
    TACKTS = 9;
    AddAddress(rPC);
    strcpy(MNEMONIC, "LD A, R");
    strcpy(TRANSCRIPT, "A<-R");
    strcpy(COMMENT, "Load Refresh Register in accumulator");
    return -1;
}


int LD_A_R_run()
{
    return 0;   // WARN
}

int LD_R_A()
{
    return 0;   // WARN
}


int LD_DD_pNN_dec()
{
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));

    uint8 retValue = prevPC;

    sprintf(MNEMONIC, "LD %s, [#%04X]", DD_45_Name(retValue), PC16andInc());

    AddAddress(rPC);

    return -1;
}

int LD_DD_pNN_run()
{
    uint8 valReg = prevPC;
    DD_45(valReg) = RAM16(PC16andInc());
    return 20;
}


int LD_pNN_DD_dec()
{
    TACKTS = 20;

    uint8 valDD = prevPC;
    AddOpcode(RAM8(rPC));
    AddOpcode(RAM8(rPC + 1));
    uint16 address = PC16andInc();
    AddAddress(rPC);

    sprintf(MNEMONIC, "LD [%4X],%s", address, DD_45_Name(valDD));
    sprintf(TRANSCRIPT, "[%4X]<-%s", address, DD_45_Name(prevPC));

    return -1;
}

int LD_pNN_DD_run()
{
    uint valDD = prevPC;
    RAM16(PC16andInc()) = DD_45(valDD);
    return 20;
}


int LDI()
{
    RAM16(rDE) = RAM16(rHL);
    rDE += 1;
    rHL += 1;
    rBC -= 1;
    return 16;
}


int LDIR_dec()
{
    AddAddress(rPC);
    strcpy(MNEMONIC, "LDIR");
    return -1;
}


int LDIR_run()
{
    int time = 0;
    do
    {
        LDI();
        time += 21;
    } while(rBC != 0);

    return time + 16;
}


int LDD()
{
    RAM16(rDE) = RAM16(rHL);
    rDE--;
    rHL--;
    rBC--;
    return 16;
}


int LDDR_dec()
{
    AddAddress(rPC);
    sprintf(MNEMONIC, "LDDR");
    return -1;
}


int LDDR_run()
{
    int time = 0;

    do
    {
        time += 21;
        LDD();
    } while(rBC != 0);

    time += 16;

    return time;
}


int CPI()
{
    rHL++;
    rBC--;

    // + + x + x + 1 .

    // S WARN
    // Z WARN
    // H WARN
    // PV WARN
    SET_N;

    return 16;
}


int CPIR()
{
    int time = 0;

    do
    {
        time += 21;
        CPI();
    } while(rBC != 0 && rA != RAM8(rHL));

    return time + 16;
}


int CPD()
{
    rHL--;
    rBC--;

    // + + x + x + 1 .
    // S WARN
    // Z WARN
    // H WARN
    // PV WARN
    SET_N;

    return 16;
}


int CPDR()
{
    int time = 0;

    do
    {
        time += 21;
        CPD();
    } while(rBC != 0 && rA != RAM8(rHL));

    return time + 16;
}


int NEG_dec()
{
    AddAddress(rPC);
    strcpy(MNEMONIC, "NEG");
    return -1;
}


int NEG_run()
{
    rA = (uint8)((~rA) + 1);

    // + + x + x V 1 +
    // S WARN
    // Z
    // H
    // V
    SET_N;
    // C

    return 8;
}


int IM0()
{
    return 0;   // WARN
}


int IM1_dec()
{
    AddAddress(rPC);
    strcpy(MNEMONIC, "IM1");
    return -1;
}


int IM1_run()
{
    return 0;   // WARN
}


int IM2_dec()
{
    AddAddress(rPC);
    strcpy(MNEMONIC, "IM2");
    return -1;
}


int IM2_run()
{
    return 0;   // WARN
}


int ADC_HL_SS()
{
    rHL += SS_45(prevPC) + CF;

    // + + x x x V 0 +
    // S
    // Z    WARN
    // V
    RES_N;
    // C

    return 15;
}


int SBC_HL_SS_dec()
{
    TACKTS = 15;
    AddAddress(rPC);
    strcpy(FLAGS, "++XXXV1+");
    sprintf(MNEMONIC, "SBC HL,%s", SS_45_Name(prevPC));
    sprintf(TRANSCRIPT, "HL<-HL-%s-CY", SS_45_Name(prevPC));
    return -1;
}

int SBC_HL_SS_run()
{
    rHL -= SS_45(prevPC) - CF;

    // + + x x x V 1 +
    // S
    // Z    WARN
    // V
    SET_N;
    // C

    return 15;
}


int RLD()
{
    uint8 val_pHL = RAM8(rHL);
    uint8 pHLlow = (uint8)(val_pHL & 0x0f);
    uint8 pHLhi = (uint8)((val_pHL >> 4) & 0x0f);
    uint8 Alow = (uint8)(rA & 0x0f);

    val_pHL = 0;
    rA &= 0x0f;

    val_pHL |= Alow;        // low pHL
    val_pHL |= pHLlow << 4; // hi pHL
    rA |= pHLhi;

    RAM8(rHL) = val_pHL;

    // + + x 0 x P 0 .
    // S    WARN
    // Z
    RES_H;
    // P
    RES_N;

    return 18;
}


int RRD()
{
    uint8 val_pHL = RAM8(rHL);
    uint8 pHLlow = (uint8)(val_pHL & 0x0f);
    uint8 pHLhi = (uint8)((val_pHL >> 4) & 0x0f);
    uint8 Alow = (uint8)(rA & 0x0f);

    val_pHL = 0;
    rA &= 0x0f;

    val_pHL |= pHLhi;
    val_pHL |= Alow << 4;
    rA |= pHLlow;

    RAM8(rHL);

    // + + x 0 x P 0 .
    // S    WARN
    // Z
    RES_H;
    // P
    RES_N;

    return 18;
}


int RETN_dec()
{
    strcpy(MNEMONIC, "RETN");
    return -1;
}

int RETN_run()
{
    return 0;  // WARN
}


int IN_R_pC_dec()
{
    if (((prevPC >> 3) & 7) != 6)
    {
        sprintf(MNEMONIC, "IN %s,(C)", R8_HI_Name(prevPC));
        sprintf(TRANSCRIPT, "%s<-(C)", R8_HI_Name(prevPC));
        strcpy(COMMENT, "C - A0...A7, B - A8...A15");
    }
    else
    {
        strcpy(MNEMONIC, "IN (rHL), (C); R == 110, set flags only");
    }
    strcpy(FLAGS, "+ + X + X P 0 .");
    AddAddress(rPC);

    return -1;
}

int IN_R_pC_run()
{
    if(((prevPC >> 3) & 7) == 6)
    {

    }
    else
    {
        R8_HI(prevPC) = ReadPort(rC);
    }

    // + + x + x P 0 .
    // S
    // Z    WARN
    // H
    // P
    RES_N;

    return 12;
}


int OUT_pC_A_dec()
{
    AddAddress(rPC);
    sprintf(MNEMONIC, "OUT (C), A");
    return -1;
}

int OUT_pC_A_run()
{
    return 0;
}


int INI_dec()
{
    strcpy(MNEMONIC, "INI");
    strcpy(COMMENT, "C - A0...A7, B - A8...A15; if B-1==0, Z set, else reset");
    strcpy(FLAGS, "X + X X X X 1 X");
    strcpy(TRANSCRIPT, "(rHL)<-(C); B--; HL++");
    AddAddress(rPC);
    return 0;
}


int INI_run()
{
    return 0;
}


int INIR_dec()
{
    strcpy(MNEMONIC, "INIR");
    strcpy(COMMENT, "C - A0...A7, B - A8...A15; z set only after finishing instruction");
    strcpy(FLAGS, "X 1 X X X X X 1 ");
    strcpy(TRANSCRIPT, "(rHL)<-(C); B--; HL++; while B!=0");
    AddAddress(rPC);
    return 0;
}

int INIR_run()
{
    return 0;
}


int IND_dec()
{
    strcpy(MNEMONIC, "IND");
    strcpy(TRANSCRIPT, "(rHL)<-(C); B--; HL--");
    AddAddress(rPC);
    return 0;
}


int IND_run()
{
    return 0;
}


int INDR()
{
#ifdef LISTING

    strcpy(MNEMONIC, "INDR");
    strcpy(TRANSCRIPT, "(rHL)<-(C); B--; HL--; while B!=0");
    AddAddress(rPC);
    return 0;

#else

    return 0;

#endif
}


int OUT_pC_R_dec()
{
    sprintf(MNEMONIC, "OUT (C), %s", R8_HI_Name(prevPC));
    sprintf(TRANSCRIPT, "(C)<-%s", R8_HI_Name(prevPC));
    return 0;
}


int OUT_pC_R_run()
{
    return 0;
}


#include "FuncSecondED.h"


int RunCommandED()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelED[index][1] == 0)
    {
        return 0;
    }

    return secondLevelED[index][1]();
}


int DecodeCommandED()
{
    AddOpcode(RAM8(rPC));

    int index = PCandInc();

    if(secondLevelED[index][0] == 0)
    {
        return 0;
    }

    return secondLevelED[index][0]();
}
