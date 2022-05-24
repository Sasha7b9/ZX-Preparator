// 2022/05/24 15:12:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class TimerMS
{
public:

    TimerMS() { Reset(); }

    void Reset();

    double ElapsedTime();

private:

    double time_reset = 0.0;
};