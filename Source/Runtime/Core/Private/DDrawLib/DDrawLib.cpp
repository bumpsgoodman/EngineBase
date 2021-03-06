#include "CorePCH.h"
#include "Util/RegionCode.h"

DDraw::~DDraw()
{
    Cleanup();
}

Bool DDraw::Initialize(const HWND hWnd)
{
    HRESULT hr;
    DDSURFACEDESC2 ddsd = {};

    mhWnd = hWnd;

    UpdateWindowPos();
    mWidth = mWindowRect.right - mWindowRect.left;
    mHeight = mWindowRect.bottom - mWindowRect.top;

    hr = DirectDrawCreate(nullptr, &mDD, nullptr);
    if (FAILED(hr))
    {
        AssertW(false, L"Failed to Create DDrawObject");
        goto EXIT;
    }

    hr = mDD->QueryInterface(IID_IDirectDraw7, (LPVOID*)&mDD7);
    if (FAILED(hr))
    {
        goto EXIT;
    }

    hr = mDD7->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
    if (FAILED(hr))
    {
        AssertW(false, L"Failed to SetCooperativeLevel");
        goto EXIT;
    }

    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    hr = mDD7->CreateSurface(&ddsd, &mPrimary, nullptr);
    if (FAILED(hr))
    {
        AssertW(false, L"Failed to CreateSurface");
        goto EXIT;
    }

    if (!CreateBackBuffer(mWidth, mHeight))
    {
        AssertW(false, L"Failed to CreateBackBuffer");
        goto EXIT;
    }

    hr = mDD7->CreateClipper(0, &mClipper, nullptr);
    if (FAILED(hr))
    {
        AssertW(false, L"Failed to CreateClipper");
        goto EXIT;
    }
    mClipper->SetHWnd(0, hWnd);
    mPrimary->SetClipper(mClipper);

    return true;

EXIT:
    Cleanup();
    return false;
}

void DDraw::Cleanup()
{
    SAFE_RELEASE(mClipper);
    SAFE_RELEASE(mBack);
    SAFE_RELEASE(mPrimary);
    SAFE_RELEASE(mDD7);
    SAFE_RELEASE(mDD);
}

void DDraw::UpdateWindowPos()
{
    GetClientRect(mhWnd, &mWindowRect);
    ClientToScreen(mhWnd, (POINT*)&mWindowRect.left);
    ClientToScreen(mhWnd, (POINT*)&mWindowRect.right);
}

void DDraw::UpdateWindowSize()
{
    SAFE_RELEASE(mBack);

    UpdateWindowPos();

    const Uint32 width = mWindowRect.right - mWindowRect.left;
    const Uint32 height = mWindowRect.bottom - mWindowRect.top;
    CreateBackBuffer(width, height);
}

Bool DDraw::CreateBackBuffer(const Uint32 width, const Uint32 height)
{
    AssertW(mDD7 != nullptr, L"DDraw object is nullptr");

    HRESULT hr;
    DDSURFACEDESC2 ddsd = {};

    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwHeight = height;
    ddsd.dwWidth = width;

    hr = mDD7->CreateSurface(&ddsd, &mBack, nullptr);
    if (FAILED(hr))
    {
        goto EXIT;
    }

    mWidth = width;
    mHeight = height;

    return true;

EXIT:
    return false;
}

Bool DDraw::BeginDraw()
{
    AssertW(mBack != nullptr, L"th back buffer is nullptr");

    HRESULT hr;
    DDSURFACEDESC2 ddsd = {};

    ddsd.dwSize = sizeof(DDSURFACEDESC2);

    hr = mBack->Lock(nullptr, &ddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);
    if (FAILED(hr))
    {
        goto EXIT;
    }
    
    AssertW(ddsd.dwWidth == mWidth, L"Mismatch width");
    AssertW(ddsd.dwHeight == mHeight, L"Mismatch height");

    mLockedBackBuffer = (char*)ddsd.lpSurface;
    mLockedBackBufferPitch = ddsd.lPitch;

    return true;

EXIT:
    return false;
}

void DDraw::EndDraw()
{
    AssertW(mBack != nullptr, L"The back buffer is nullptr");
    mBack->Unlock(nullptr);

    mLockedBackBuffer = nullptr;
    mLockedBackBufferPitch = 0;
}

void DDraw::Clear(const Uint32 color)
{
    AssertW(mLockedBackBuffer != nullptr, L"The back buffer is not locked");

    for (Uint32 y = 0; y < mHeight; ++y)
    {
        for (Uint32 x = 0; x < mWidth; ++x)
        {
            Uint32* dest = (Uint32*)(mLockedBackBuffer + y * mLockedBackBufferPitch + x * 4);
            *dest = color;
        }
    }
}

void DDraw::OnDraw()
{
    mPrimary->Blt(&mWindowRect, mBack, nullptr, DDBLT_WAIT, nullptr);
}

Bool DDraw::BeginGDI(HDC* outHdc)
{
    HRESULT hr;
    HDC hdc;

    hr = mBack->GetDC(&hdc);
    if (FAILED(hr))
    {
        AssertW(false, L"Failed to GetDC");
        return false;
    }

    *outHdc = hdc;

    return true;
}

void DDraw::EndGDI(const HDC hdc)
{
    mBack->ReleaseDC(hdc);
}

void DDraw::PrintText(const HDC hdc, const wchar_t* const text, const Int32 destX, const Int32 destY, const Uint32 length, const Uint32 color)
{
    const Uint32 r = (color & 0x00ff0000) >> 16;
    const Uint32 g = (color & 0x0000ff00) >> 8;
    const Uint32 b = (color & 0x000000ff);

    SetTextColor(hdc, RGB(r, g, b));
    SetBkMode(hdc, TRANSPARENT);

    TextOut(hdc, destX, destY, text, length);
}

void DDraw::DrawPixel(const IntVector2& dest, const Uint32 color)
{
    AssertW(mLockedBackBuffer != nullptr, L"The back buffer is not locked");
    //AssertW(destX <= mWidth && destY <= mHeight, L"Out of range x, y");

    if ((Uint32)dest.X >= mWidth || (Uint32)dest.Y >= mHeight)
    {
        return;
    }

    //Uint32* dest = (Uint32*)(mLockedBackBuffer + destY * mLockedBackBufferPitch + destX * 4);
    //*dest = color;
    *(Uint32*)(mLockedBackBuffer + dest.Y * mLockedBackBufferPitch + dest.X * 4) = color;
}

void DDraw::DrawLineDDA(const IntVector2& start, const IntVector2& end, const Uint32 color)
{
    Float x = (Float)start.X;
    Float y = (Float)start.Y;

    Int32 dx = end.X - start.X;
    Int32 dy = end.Y - start.Y;
    Uint32 steps = (ABS(dx) > (ABS(dy))) ? ABS(dx) : ABS(dy);

    Float xIncreament = dx / (Float)steps;
    Float yIncreament = dy / (Float)steps;

    DrawPixel(start, color);

    for (Uint32 i = 0; i < steps; ++i)
    {
        x += xIncreament;
        y += yIncreament;

        DrawPixel({ ROUND(x), ROUND(y) }, color);
    }
}

void DDraw::DrawLineBresenham(const IntVector2& start, const IntVector2& end, const Uint32 color)
{
    Int32 width = end.X - start.X;
    Int32 height = end.Y - start.Y;
    Bool bGradualSlope = (ABS(width) >= ABS(height)) ? true : false;

    Int32 dx = (width >= 0) ? 1 : -1;
    Int32 dy = (height >= 0) ? 1 : -1;
    Int32 w = dx * width;
    Int32 h = dy * height;

    Int32 p = (bGradualSlope) ? 2 * h - w : 2 * w - h;
    Int32 p1 = (bGradualSlope) ? 2 * h : 2 * w;
    Int32 p2 = (bGradualSlope) ? 2 * h - 2 * w : 2 * w - 2 * h;

    IntVector2 pos(start);

    if (bGradualSlope)
    {
        while (pos.X != end.X)
        {
            DrawPixel(pos, color);

            if (p < 0)
            {
                p += p1;
            }
            else
            {
                pos.Y += dy;
                p += p2;
            }

            pos.X += dx;
        }
    }
    else
    {
        while (pos.Y != end.Y)
        {
            DrawPixel(pos, color);

            if (p < 0)
            {
                p += p1;
            }
            else
            {
                pos.X += dx;
                p += p2;
            }

            pos.Y += dy;
        }
    }

    DrawPixel(pos, color);
}

void DDraw::DrawCircle(const IntVector2& center, const Uint32 radius, const Uint32 color)
{
    Int32 x = 0;
    Int32 y = radius;
    Int32 p = 1 - radius;

    DrawPixel({ center.X + (Int32)radius, center.Y }, color);
    DrawPixel({ center.X - (Int32)radius, center.Y }, color);
    DrawPixel({ center.X, center.Y + (Int32)radius }, color);
    DrawPixel({ center.X, center.Y - (Int32)radius }, color);

    while (x < y)
    {
        ++x;
        if (p < 0)
        {
            p = p + 2 * (x + 1);
        }
        else
        {
            --y;
            p = p + 2 * (x - y) + 1;
        }

        DrawPixel({ center.X + x, center.Y + y }, color);
        DrawPixel({ center.X + y, center.Y + x }, color);
        DrawPixel({ center.X + y, center.Y - x }, color);
        DrawPixel({ center.X + x, center.Y - y }, color);
        DrawPixel({ center.X - x, center.Y - y }, color);
        DrawPixel({ center.X - y, center.Y - x }, color);
        DrawPixel({ center.X - y, center.Y + x }, color);
        DrawPixel({ center.X - x, center.Y + y }, color);
    }
}

void DDraw::DrawRectangle(const IntVector2& dest, const Uint32 width, const Uint32 height, const Uint32 color)
{
    AssertW(mLockedBackBuffer != nullptr, L"The back buffer is not locked");

    const Uint32 startX = MAX((Uint32)dest.X, 0);
    const Uint32 startY = MAX((Uint32)dest.Y, 0);
    const Uint32 endX = MIN((Uint32)dest.X + width, mWidth);
    const Uint32 endY = MIN((Uint32)dest.Y + height, mHeight);

    for (Uint32 y = startY; y < endY; ++y)
    {
        for (Uint32 x = startX; x < endX; ++x)
        {
            DrawPixel({ x, y }, color);
        }
    }
}

void DDraw::DrawBitmap(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const char* const bitmap)
{
    AssertW(mLockedBackBuffer != nullptr, L"The back buffer is not locked");
    AssertW(bitmap != nullptr, L"bitmap is nullptr");

    const Uint32 srcX = (destX >= 0) ? 0 : -destX;
    const Uint32 srcY = (destY >= 0) ? 0 : -destY;

    const Uint32 startX = MAX(destX, 0);
    const Uint32 startY = MAX(destY, 0);
    const Uint32 endX = MIN(destX + (Int32)width, (Int32)mWidth);
    const Uint32 endY = MIN(destY + (Int32)height, (Int32)mHeight);

    const Uint32 destWidth = endX - startX;
    const Uint32 destHeight = endY - startY;
    if (destWidth > mWidth || destHeight > mHeight)
    {
        return;
    }

    const char* src = bitmap + (srcY * width + srcX) * 4;
    char* dest = mLockedBackBuffer + startY * mLockedBackBufferPitch + startX * 4;

    for (Uint32 y = startY; y < endY; ++y)
    {
        for (Uint32 x = startX; x < endX; ++x)
        {
            *(Uint32*)dest = *(Uint32*)src;

            src += 4;
            dest += 4;
        }

        src -= destWidth * 4;
        src += width * 4;
        dest -= destWidth * 4;
        dest += mLockedBackBufferPitch;
    }
}

void DDraw::DrawBitmapWithColorKey(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const char* const bitmap, const Uint32 colorKey)
{
    AssertW(mLockedBackBuffer != nullptr, L"The back buffer is not locked");
    AssertW(bitmap != nullptr, L"bitmap is nullptr");

    const Uint32 srcX = (destX >= 0) ? 0 : -destX;
    const Uint32 srcY = (destY >= 0) ? 0 : -destY;

    const Uint32 startX = MAX(destX, 0);
    const Uint32 startY = MAX(destY, 0);
    const Uint32 endX = MIN(destX + (Int32)width, (Int32)mWidth);
    const Uint32 endY = MIN(destY + (Int32)height, (Int32)mHeight);

    const Uint32 destWidth = endX - startX;
    const Uint32 destHeight = endY - startY;
    if (destWidth > mWidth || destHeight > mHeight)
    {
        return;
    }

    const char* src = bitmap + (srcY * width + srcX) * 4;
    char* dest = mLockedBackBuffer + startY * mLockedBackBufferPitch + startX * 4;

    for (Uint32 y = 0; y < destHeight; ++y)
    {
        for (Uint32 x = 0; x < destWidth; ++x)
        {
            if (*(Uint32*)src != colorKey)
            {
                *(Uint32*)dest = *(Uint32*)src;
            }

            src += 4;
            dest += 4;
        }

        src -= destWidth * 4;
        src += width * 4;
        dest -= destWidth * 4;
        dest += mLockedBackBufferPitch;
    }
}

bool DDraw::ClipLineCoham(IntVector2* inOutStartPos, IntVector2* inOutEndPos, const IntVector2& windowLeftTop, const IntVector2& windowRightBottom)
{
    AssertW(inOutStartPos != nullptr, L"inOutStartPos is nullptr");
    AssertW(inOutEndPos != nullptr, L"inOutEndPos is nullptr");

    Uint8 startCode = regioncode::MakeRegionCode(*inOutStartPos, windowLeftTop, windowRightBottom);
    Uint8 endCode = regioncode::MakeRegionCode(*inOutEndPos, windowLeftTop, windowRightBottom);

    while (true)
    {
        Uint8 outsideCode = (startCode == 0) ? endCode : startCode;

        if ((startCode == 0) && (endCode == 0))
        {
            return true;
        }
        else if (startCode & endCode)
        {
            return false;
        }
        else
        {
            Float width = (Float)(inOutEndPos->X - inOutStartPos->X);
            Float height = (Float)(inOutEndPos->Y - inOutStartPos->Y);
            IntVector2 pos;

            // left or right
            if (outsideCode < 0b0100)
            {
                if (outsideCode & 0b0010)
                {
                    pos.X = windowRightBottom.X;
                }
                else
                {
                    pos.X = windowLeftTop.X;
                }

                pos.Y = ROUND(inOutStartPos->Y + height * (pos.X - inOutStartPos->X) / width);
            }
            else // top or bottom
            {
                if (outsideCode & 0b1000)
                {
                    pos.Y = windowLeftTop.Y;
                }
                else
                {
                    pos.Y = windowRightBottom.Y;
                }

                pos.X = ROUND(inOutStartPos->X + width * (pos.Y - inOutStartPos->Y) / height);
            }

            if (startCode == 0)
            {
                *inOutEndPos = pos;
                endCode = regioncode::MakeRegionCode(*inOutEndPos, windowLeftTop, windowRightBottom);
            }
            else
            {
                *inOutStartPos = pos;
                startCode = regioncode::MakeRegionCode(*inOutStartPos, windowLeftTop, windowRightBottom);
            }
        }
    }
}