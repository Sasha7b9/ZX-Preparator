// 2022/05/23 16:56:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "DataBase.h"
#include "DLLmain.h"
#include "RAM48.h"



DataBase base;

OutStruct params;



int decompiler()
{
    params.RAM = RAM48;

    InitEMU(&params);

    int address = base.NextAddress();

    while(address >= 0)
    {
        bool res = Decode((uint16)address) != 0;

        base.AddNewData(res, address, &params);

        address = base.NextAddress();
    }

    base.CreateReport();

    return 0;
}