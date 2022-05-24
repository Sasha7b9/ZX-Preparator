// 2022/05/24 15:13:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"
#include <ctime>


void TimerMS::Reset()
{
    time_reset = (double)std::time(0) * 1000.0;
}


double TimerMS::ElapsedTime()
{
    return (double)std::time(0) * 1000 - time_reset;
}
