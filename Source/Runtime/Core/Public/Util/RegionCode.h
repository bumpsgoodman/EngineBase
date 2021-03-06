#pragma once

namespace regioncode
{
    Uint8 MakeRegionCode(const IntVector2& pos, const IntVector2& regionLeftTopPos, const IntVector2& regionRightBottom);
}

Uint8 regioncode::MakeRegionCode(const IntVector2& pos, const IntVector2& regionLeftTopPos, const IntVector2& regionRightBottom)
{
    Uint8 regionCode = 0;

    if (pos.X < regionLeftTopPos.X)
    {
        regionCode |= 0b0001;
    }
    else if (pos.X > regionRightBottom.X)
    {
        regionCode |= 0b0010;
    }

    if (pos.Y < regionLeftTopPos.Y)
    {
        regionCode |= 0b1000;
    }
    else if (pos.Y > regionRightBottom.Y)
    {
        regionCode |= 0b0100;
    }

    return regionCode;
}