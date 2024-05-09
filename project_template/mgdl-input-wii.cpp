#include "mgdl-input-wii.h"
#include "mgdl-wii.h"
#include <wiiuse/wpad.h>
#include <math.h>

static gdl::WiimoteState mote1;

static s32 initStatus;

bool gdl::WiiInput::ButtonPress(int buttonEnum) {
  return (mote1.pressedButtons & buttonEnum) != 0;
}

bool gdl::WiiInput::ButtonRelease(int buttonEnum) {
  return (mote1.releasedButtons & buttonEnum) != 0;
}

bool gdl::WiiInput::ButtonHeld(int buttonEnum) {
  return (mote1.heldButtons & buttonEnum) != 0;
}

gdl::vec2 gdl::WiiInput::GetCursorPosition() {
  return gdl::vec2(mote1.cursorX, mote1.cursorY);
}

gdl::vec2 gdl::WiiInput::GetNunchukJoystickDirection(float deadzone)
{
  mote1.nunchukJoystickDeadzone = deadzone;
  return gdl::vec2(mote1.nunchukJoystickDirectionX, mote1.nunchukJoystickDirectionY);
}

void gdl::WiiInput::Init() {
	initStatus = WPAD_Init();
  WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
  mote1 = {0};
}

void gdl::WiiInput::StartFrame() {
  WPAD_ScanPads();  // Scan the Wiimotes

  ir_t ir;
  WPAD_IR(WPAD_CHAN_0, &ir);
  mote1.cursorX = ir.x;
  mote1.cursorY = ir.y;

  mote1.pressedButtons = WPAD_ButtonsDown(0);
  mote1.releasedButtons = WPAD_ButtonsUp(0);
  mote1.heldButtons = WPAD_ButtonsHeld(0);

  mote1.nunchukJoystickDirectionX=0.0f;
  mote1.nunchukJoystickDirectionY=0.0f;
  expansion_t ex;
  WPAD_Expansion(WPAD_CHAN_0, &ex);
  if (ex.type == WPAD_EXP_NUNCHUK)
  {
      joystick_t n = ex.nunchuk.js;
      if (n.mag > mote1.nunchukJoystickDeadzone)
      {
          // Angle of 0 means up.
          // 90 right, 180 down, 270 left

          float rad = n.ang * PI / 180.0f;
          float x = 0;
          float y = -1.0f;
          float dirx = cos(rad) * x - sin(rad) * y;
          float diry = sin(rad) * x + cos(rad) * y;
          mote1.nunchukJoystickDirectionX = dirx * n.mag;
          mote1.nunchukJoystickDirectionY = diry * n.mag;
      }
  }
}

