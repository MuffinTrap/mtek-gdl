#include "mgdl-wii/mgdl-input-wii.h"
#include "mgdl-wii.h"
#include <wiiuse/wpad.h>
#include <math.h>

bool gdl::WiiInput::ButtonPress(int buttonEnum) {
  return (WPAD_ButtonsDown(0) & buttonEnum) != 0;
}

bool gdl::WiiInput::ButtonRelease(int buttonEnum) {
  return (WPAD_ButtonsUp(0) & buttonEnum);
}

bool gdl::WiiInput::ButtonHeld(int buttonEnum) {
  return (WPAD_ButtonsHeld(0) & buttonEnum);
}


void gdl::WiiInput::Init() {
	WPAD_Init();
  WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
}

gdl::vec2 gdl::WiiInput::GetCursorPosition() {
  // Pointer
  ir_t ir;
  WPAD_IR(WPAD_CHAN_0, &ir);
  return gdl::vec2(ir.x, ir.y);
}

gdl::vec2 gdl::WiiInput::GetNunchukJoystickDirection(float deadzone)
{
    expansion_t ex;
    WPAD_Expansion(WPAD_CHAN_0, &ex);
    if (ex.type == WPAD_EXP_NUNCHUK)
    {
        joystick_t n = ex.nunchuk.js;
        if (n.mag > deadzone)
        {
            // Angle of 0 means up.
            // 90 right, 180 down, 270 left

            float rad = n.ang * PI / 180.0f;
            float x = 0;
            float y = -1.0f;
            float dirx = cos(rad) * x - sin(rad) * y;
            float diry = sin(rad) * x + cos(rad) * y;
            return gdl::vec2(dirx * n.mag, diry * n.mag);
        }
    }
    return gdl::vec2(0.0f, 0.0f);
}

void gdl::WiiInput::StartFrame() {
  WPAD_ScanPads();  // Scan the Wiimotes
}

