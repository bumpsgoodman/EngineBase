#pragma once

namespace event
{
    namespace mouse
    {
        void OnClick(const Uint32 x, const Uint32 y);
        Bool IsClicked();
        void Release();
        Uint32 GetX();
        Uint32 GetY();
    }

    namespace keyboard
    {
        void KeyDown(const Uint8 keyCode);
        void KeyUp(const Uint8 keyCode);
        void Release();
        Bool IsKeyPressed(const Uint8 keyCode);
    }
}