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
    mElapsedTick = (Float)dElapsedTick;

    if (mElapsedTick >= mInterval)
    {
        mbOnTick = true;
        mPrevCounter = curCounter;
    }
    else
    {
        mbOnTick = false;
    }
}