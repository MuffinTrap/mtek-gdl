#pragma once

namespace gdl
{
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

        char ir_message[60];
        char position_message[60];
        char correction_message[60];
    };
};