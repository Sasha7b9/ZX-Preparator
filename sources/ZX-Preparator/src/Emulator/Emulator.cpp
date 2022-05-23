// 2022/05/23 17:19:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define WIN32_LEAN_AND_MEAN
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)
#include "CPU/registers.h"
#include "defines.h"
#include "Emulator/Emulator.h"
#include "CPU/instr.h"


OutStruct *out = 0;

uint8 *RAM = 0;



void Emulator::Init(OutStruct *out_)
{
    out = out_;
    out->regs = &regs;
    out->regsAlt = &regsAlt;
    RAM = out->MEMORY;
}



int Emulator::Decode(int address)
{
    out->numAddresses = 0;
    out->numOpCodes = 0;
    TRANSCRIPT[0] = 0;
    out->flags[0] = 0;
    out->tackts = 0;
    out->comment[0] = 0;

    rPC = (uint16)address;

    // If RunCommand() return >= 0, decoding fail
    return DecodeCommand();
}



int Emulator::Run(int address)
{
    rPC = (uint16)address;

    return RunCommand();
}



int Emulator::RunNext(void)
{
    return RunCommand();
}
