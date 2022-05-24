// 2022/05/24 11:52:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


extern uint8 *RAM;

#define RAM16(address) (*((uint16*)&RAM[address]))
#define RAM8(address) (*(&RAM[address]))
