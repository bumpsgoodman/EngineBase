#pragma once

class DDraw final
{
public:
    DDraw() = default;
    DDraw(const DDraw&) = delete;
    DDraw& operator=(const DDraw&) = delete;
    DDraw(DDraw&&) = delete;
    DDraw& operator=(DDraw&&) = delete;
    ~DDraw();

    Bool Initialize(const HWND hWnd);
    void Cleanup();

    void UpdateWindowPos();
    void UpdateWindowSize();
    Bool CreateBackBuffer(const Uint32 width, const Uint32 height);

    inline HWND GetHWnd() const { return mhWnd; }
    inline Uint32 GetWidth() const { return mWidth; }
    inline Uint32 GetHeight() const { return mHeight; }

    Bool BeginDraw();
    void EndDraw();
    void Clear(const Uint32 color);
    void Blt();

    Bool BeginGDI(HDC* outHdc);
    void EndGDI(const HDC hdc);
    void PrintText(const HDC hdc, const wchar_t* const text, const Int32 destX, const Int32 destY, const Uint32 length, const Uint32 color);

    void DrawPixel(const Uint32 destX, const Uint32 destY, const Uint32 color);
    void DrawLineDDA(const Int32 x0, const Int32 y0, const Int32 x1, const Int32 y1, const Uint32 color);
    void DrawLineBresenham(const Int32 x0, const Int32 y0, const Int32 x1, const Int32 y1, const Uint32 color);
    void DrawCircle(const Int32 centerX, const Int32 centerY, const Uint32 radius, const Uint32 color);
    void DrawRectangle(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const Uint32 color);
    void DrawBitmap(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const char* const bitmap);
    void DrawBitmapWithColorKey(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const char* const bitmap, const Uint32 colorKey);

private:
    HWND mhWnd = 0;
    RECT mWindowRect = {};
    Uint32 mWidth = 0;
    Uint32 mHeight = 0;

    char* mLockedBackBuffer = nullptr;
    Uint32 mLockedBackBufferPitch = 0;

    IDirectDraw* mDD = nullptr;
    IDirectDraw7* mDD7 = nullptr;
    IDirectDrawSurface7* mPrimary = nullptr;
    IDirectDrawSurface7* mBack = nullptr;
    IDirectDrawClipper* mClipper = nullptr;
};