#include "PlayerPCH.h"
#include "Window.h"

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    constexpr Uint32 WINDOW_WIDTH = 800;
    constexpr Uint32 WINDOW_HEIGHT = 600;

    if (!window::Create(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        AssertW(false, L"Failed to Create Window");
        goto EXIT;
    }

    window::util::ShowWindow(window::ghWnd);
    window::util::CenterWindow(window::ghWnd);

    while (window::Tick())
    {

    }

EXIT:
    window::Destroy();

    return 0;
}