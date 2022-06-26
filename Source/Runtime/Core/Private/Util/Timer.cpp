#include "CorePCH.h"

Timer::Timer(const Float interval)
    : mInterval(interval)
{
    QueryPerformanceFrequency(&mFrequency);
    QueryPerformanceCounter(&mPrevCounter);
}

void Timer::Update()
{
    static LARGE_INTEGER curCounter;

    QueryPerformanceCounter(&curCounter);
    Double dElapsedTick = (((Double)curCounter.QuadPart - (Double)mPrevCounter.QuadPart) / (Double)mFrequency.QuadPart * 1000.0);
    Float fElpasedTick = (Float)dElapsedTick;

    if (fElpasedTick >= mInterval)
    {
        mbOnTick = true;
        mPrevCounter = curCounter;
    }
    else
    {
        mbOnTick = false;
    }
}