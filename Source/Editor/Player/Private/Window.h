#pragma once

namespace window
{
    static HINSTANCE ghInstance;
    static HWND ghWnd;
    static wchar_t gAppName[128] = L"EngineBase - ";
    static wchar_t gExtendedAppName[128] = L"Test";
    static void (*gUpdateWindowPosFunc)(void);

    LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool Create(HINSTANCE hInstance, const Uint32 windowWidth, const Uint32 windowHeight);
    void Destroy();
    bool Tick();

    void SetTitle();

    namespace util
    {
        void ShowWindow(HWND hWnd);
        void CenterWindow(HWND hWnd);
    }
}

LRESULT CALLBACK window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        Uint32 x = GET_X_LPARAM(lParam);
        Uint32 y = GET_Y_LPARAM(lParam);
        //event::mouse::OnClick(x, y);
        break;
    }
    case WM_MOVE:
        if (gUpdateWindowPosFunc)
        {
            gUpdateWindowPosFunc();
        }
        break;
    case WM_KEYDOWN:
        //event::keyboard::KeyDown((uint8_t)wParam);
        break;
    case WM_KEYUP:
        //event::keyboard::KeyUp((uint8_t)wParam);
        break;
    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

bool window::Create(HINSTANCE hInstance, const Uint32 windowWidth, const Uint32 windowHeight)
{
    ghInstance = hInstance;

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = gAppName;
    wc.hIconSm = nullptr;

    RegisterClassExW(&wc);

    RECT windowRect = { 0, 0, (LONG)windowWidth, (LONG)windowHeight };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

    static_assert(sizeof(gAppName) == sizeof(wchar_t) * 128, "Mismatch size");
    static_assert(sizeof(gExtendedAppName) == sizeof(wchar_t) * 128, "Mismatch size");
    wchar_t appName[256];

    swprintf(appName, L"%s%s", gAppName, gExtendedAppName);

    ghWnd = CreateWindowExW(0, gAppName, appName, WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        nullptr, nullptr, hInstance, nullptr);

    if (ghWnd == nullptr)
    {
        return false;
    }

    return true;
}

void window::Destroy()
{
    DestroyWindow(ghWnd);
}

bool window::Tick()
{
    MSG msg = {};

    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

        if (msg.message == WM_QUIT)
        {
            return false;
        }
    }

    return true;
}

void window::SetTitle()
{
    static_assert(sizeof(gAppName) == sizeof(wchar_t) * 128, "Mismatch size");
    static_assert(sizeof(gExtendedAppName) == sizeof(wchar_t) * 128, "Mismatch size");
    wchar_t appName[256];

    swprintf(appName, L"%s%s", gAppName, gExtendedAppName);

    SetWindowTextW(ghWnd, appName);
}

void window::util::ShowWindow(HWND hWnd)
{
    ::ShowWindow(hWnd, SW_SHOW);
}

void window::util::CenterWindow(HWND hWnd)
{
    const Uint32 SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const Uint32 SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    const Uint32 CENTER_SCREEN_X = SCREEN_WIDTH / 2;
    const Uint32 CENTER_SCREEN_Y = SCREEN_HEIGHT / 2;

    RECT windowRect;

    GetWindowRect(hWnd, &windowRect);
    const Uint32 WINDOW_WIDTH = windowRect.right - windowRect.left;
    const Uint32 WINDOW_HEIGHT = windowRect.bottom - windowRect.top;

    MoveWindow(hWnd, CENTER_SCREEN_X - WINDOW_WIDTH / 2, CENTER_SCREEN_Y - WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
}