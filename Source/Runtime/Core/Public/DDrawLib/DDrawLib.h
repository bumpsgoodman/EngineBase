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
    void OnDraw();

    Bool BeginGDI(HDC* outHdc);
    void EndGDI(const HDC hdc);
    void PrintText(const HDC hdc, const wchar_t* const text, const Int32 destX, const Int32 destY, const Uint32 length, const Uint32 color);

    void DrawPixel(const IntVector2& dest, const Uint32 color);
    void DrawLineDDA(const IntVector2& start, const IntVector2& end, const Uint32 color);
    void DrawLineBresenham(const IntVector2& start, const IntVector2& end, const Uint32 color);
    void DrawCircle(const IntVector2& center, const Uint32 radius, const Uint32 color);
    void DrawRectangle(const IntVector2& dest, const Uint32 width, const Uint32 height, const Uint32 color);
    void DrawBitmap(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const char* const bitmap);
    void DrawBitmapWithColorKey(const Int32 destX, const Int32 destY, const Uint32 width, const Uint32 height, const char* const bitmap, const Uint32 colorKey);

    bool ClipLineCoham(IntVector2* inOutStartPos, IntVector2* inOutEndPos, const IntVector2& windowLeftTop, const IntVector2& windowRightBottom);

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