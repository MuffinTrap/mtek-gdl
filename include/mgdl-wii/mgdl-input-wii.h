#ifndef MGDL_WII_INPUT_H
#define MGDL_WII_INPUT_H

#include <gctypes.h>

namespace gdl
{
    typedef struct {
        u32 pressed;
        u32 released;
        u32 held;
        float nunchukDeadzone;
        float nunchukX;
        float nunchukY;
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

    class WiiInput
    {
        public:
        void Init();
        void StartFrame();
        gdl::WiiInputStatus GetInitStatus();
        gdl::WiiInputStatus GetScanStatus();
        gdl::WiiInputStatus GetExpansionStatus();

        // Button values are same as in <wiiuse/wpad.h>
        bool ButtonPress(int buttonEnum);
        bool ButtonRelease(int buttonEnum);
        bool ButtonHeld(int buttonEnum);

        gdl::vec2 GetCursorPosition();
        gdl::vec2 GetNunchukJoystickDirection(float deadzone);

        // TODO:
        // Try how it works if all data is read immediately after
        // scanpads()
    };
}
#endif
