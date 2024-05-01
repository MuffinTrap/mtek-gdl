#ifndef MGDL_WII_INPUT_H
#define MGDL_WII_INPUT_H

#include <gctypes.h>

namespace gdl
{
    typedef struct {
        u32 pressedButtons;
        u32 releasedButtons;
        u32 heldButtons;
        float nunchukJoystickDirectionX;
        float nunchukJoystickDirectionY;
        float nunchukJoystickDeadzone;
        float cursorX;
        float cursorY;
    } WiimoteState;

    enum WiiInputStatus : s32
    {
        AllOk = 0,
        NoController = -1,
        NotReady = -2,
        TransferError = -3,
        NoneRegistered = -4,
        UnknownError = -5,
        BadChannel = -6,
        QueueEmpty = -7,
        BadValue = -8,
        BadConfig = -9 
    };
    // TODO: Replace with glm::vec2
    class vec2
    {
        public:
        float x;
        float y;
        vec2(float x, float y)
        {
            this->x = x;
            this->y = y;
        }
    };

    namespace WiiInput
    {
        void Init();
        void StartFrame();

        // Button values are same as in <wiiuse/wpad.h>
        bool ButtonPress(int buttonEnum);
        bool ButtonRelease(int buttonEnum);
        bool ButtonHeld(int buttonEnum);

        gdl::vec2 GetCursorPosition();
        gdl::vec2 GetNunchukJoystickDirection(float deadzone);
    }
}
#endif
