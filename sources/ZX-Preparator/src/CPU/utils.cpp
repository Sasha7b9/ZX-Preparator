// 2022/05/23 17:34:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CPU/utils.h"



bool IsEven(uint8 value)
{
    int numOnes = 0;

    for (int i = 0; i < 8; i++)
    {
        if (value & 1)
        {
            numOnes++;
        }
        value >>= 1;
    }

    return (numOnes & 1) == 0;
}
