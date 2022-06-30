#include "PlayerPCH.h"

namespace event
{
    namespace mouse
    {
        static Bool sbClicked = false;
        static Uint32 sClickedX;
        static Uint32 sClickedY;

        void OnClick(const Uint32 x, const Uint32 y)
        {
            sbClicked = true;
            sClickedX = x;
            sClickedY = y;
        }

        Bool IsClicked()
        {
            return sbClicked;
        }

        void Release()
        {
            sbClicked = false;
            sClickedX = -1;
            sClickedY = -1;
        }

        Uint32 GetX()
        {
            return sClickedX;
        }

        Uint32 GetY()
        {
            return sClickedY;
        }
    }

    namespace keyboard
    {
        static Bool sbKeyMap[256] = {};

        void KeyDown(const Uint8 keyCode)
        {
            sbKeyMap[keyCode] = true;
        }

        void KeyUp(const Uint8 keyCode)
        {
            sbKeyMap[keyCode] = false;
        }

        void Release()
        {
            memset(sbKeyMap, false, sizeof(sbKeyMap));
        }

        Bool IsKeyPressed(const Uint8 keyCode)
        {
            return sbKeyMap[keyCode];
        }
    }
}