#include "PlayerPCH.h"
#include "Renderer.h"
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

    Renderer* renderer = nullptr;

    if (!window::Create(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        AssertW(false, L"Failed to Create Window");
        goto EXIT;
    }

    window::util::ShowWindow(window::ghWnd);
    window::util::CenterWindow(window::ghWnd);

    renderer = new Renderer();
    AssertW(renderer != nullptr, L"Failed to allocate renderer");

    if (!renderer->Initialize(window::ghWnd, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        AssertW(false, L"Failed to initialize renderer");
        goto EXIT;
    }

    window::gUpdateWindowPosFunc = [&renderer]()
    {
        renderer->UpdateWindowPos();
    };

    while (window::Tick())
    {
        if (renderer->IsUpdateFPS())
        {
            Float fps = renderer->GetFPS();
            swprintf(window::gExtendedAppName, L"FPS: %.1f", fps);
            window::SetTitle();
        }

        renderer->Tick();
    }

EXIT:
    renderer->Shutdown();
    window::Destroy();

    return 0;
}