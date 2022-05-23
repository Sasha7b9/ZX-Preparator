// 2022/05/23 17:34:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "common.h"
#include "utils.h"



bool IsEven(uint8 value)
{
    int numOnes = 0;

    for (int i = 0; i < 8; i++)
    {
        if (value & 1)
        {
            numOnes++;
        }
        numOnes >>= 1;
    }

    return (numOnes / 2) == 0 ? true : false ;
}
