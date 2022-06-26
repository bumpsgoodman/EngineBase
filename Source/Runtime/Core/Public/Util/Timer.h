#pragma once

class Timer final
{
public:
    Timer(const Float interval);
    Timer(const Timer&) = default;
    Timer& operator=(const Timer&) = default;
    Timer(Timer&&) = default;
    Timer& operator=(Timer&&) = default;

    void Update();

    inline Float GetInterval() const { return mInterval; }
    inline bool IsOnTick() const { return mbOnTick; }

private:
    Float mInterval = 0;
    bool mbOnTick = false;

    LARGE_INTEGER mFrequency = {};
    LARGE_INTEGER mPrevCounter = {};
};