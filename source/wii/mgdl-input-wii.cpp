#include "mgdl-wii/mgdl-input-wii.h"
#include "mgdl-wii.h"
#include <wiiuse/wpad.h>
#include <math.h>

static gdl::WiimoteState mote1;
static s32 initStatus;
static s32 scanStatus;
static s32 expansionStatus;

bool gdl::WiiInput::ButtonPress(int buttonEnum) {
  return (mote1.pressed & buttonEnum) != 0;
}

bool gdl::WiiInput::ButtonRelease(int buttonEnum) {
  return (mote1.released & buttonEnum) != 0;
}

bool gdl::WiiInput::ButtonHeld(int buttonEnum) {
  return (mote1.held & buttonEnum) != 0;
}

gdl::vec2 gdl::WiiInput::GetCursorPosition() {
  // Pointer
  return gdl::vec2(mote1.cursorX, mote1.cursorY);
}

gdl::vec2 gdl::WiiInput::GetNunchukJoystickDirection(float deadzone)
{
  mote1.nunchukDeadzone = deadzone;
  return gdl::vec2(mote1.nunchukX, mote1.nunchukY);
}

static const char* StatusToText(gdl::WiiInputStatus s)
{
    switch(s)
    {
        case gdl::WiiInputStatus::AllOk:
            return "Input OK";
            break;
        case gdl::WiiInputStatus::NotReady:
            return "Input not ready";
            break;
        case gdl::WiiInputStatus::NoController:
            return "No Controller";
            break;
        case gdl::WiiInputStatus::TransferError:
            return "Transfer error";
            break;
        case gdl::WiiInputStatus::NoneRegistered:
            return "None registered";
            break;
        case gdl::WiiInputStatus::UnknownError:
            return "Unknown Error";
            break;
        case gdl::WiiInputStatus::BadChannel:
            return "Bad Channel";
            break;
        case gdl::WiiInputStatus::QueueEmpty:
            return "Queue empty";
            break;
        case gdl::WiiInputStatus::BadValue:
            return "Bad Value";
            break;
        case gdl::WiiInputStatus::BadConfig:
            return "Bad Config";
            break;
        default:
          return "INVALID WPAD ERROR FLAG";
        break;
    }
}

static gdl::WiiInputStatus FlagToEnum(s32 status)
{
    if (status == WPAD_ERR_NONE) {
      return gdl::WiiInputStatus::AllOk;
    }
    else if (status == WPAD_ERR_NOT_READY) {
      return gdl::WiiInputStatus::NotReady;
    }
    else if (status == WPAD_ERR_NO_CONTROLLER) {
      return gdl::WiiInputStatus::NoController;
    }
    else if (status == WPAD_ERR_TRANSFER) {
      return gdl::WiiInputStatus::TransferError;
    }
    else if (status == WPAD_ERR_NONEREGISTERED) {
      return gdl::WiiInputStatus::NoneRegistered;
    }
    else if (status == WPAD_ERR_UNKNOWN) {
      return gdl::WiiInputStatus::UnknownError;
    }
    else if (status == WPAD_ERR_BAD_CHANNEL) {
      return gdl::WiiInputStatus::BadChannel;
    }
    else if (status == WPAD_ERR_QUEUE_EMPTY) {
      return gdl::WiiInputStatus::QueueEmpty;
    }
    else if (status == WPAD_ERR_BADVALUE) {
      return gdl::WiiInputStatus::BadValue;
    }
    else if (status == WPAD_ERR_BADCONF) {
      return gdl::WiiInputStatus::BadConfig;
    }
    else {
    // ???
      return gdl::WiiInputStatus::AllOk;
    }
}

gdl::WiiInputStatus gdl::WiiInput::GetInitStatus() 
{ return FlagToEnum(initStatus);}
gdl::WiiInputStatus gdl::WiiInput::GetScanStatus()
{ return FlagToEnum(scanStatus);}
gdl::WiiInputStatus gdl::WiiInput::GetExpansionStatus()
{ return FlagToEnum(expansionStatus);}

void gdl::WiiInput::Init() {
	initStatus = WPAD_Init();
  WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
  mote1 = {0};
}

void gdl::WiiInput::StartFrame() {
  expansionStatus = WPAD_GetStatus();
  scanStatus = WPAD_ScanPads();  // Scan the Wiimotes

  ir_t ir;
  WPAD_IR(WPAD_CHAN_0, &ir);
  mote1.cursorX = ir.x;
  mote1.cursorY = ir.y;

  mote1.pressed = WPAD_ButtonsDown(0);
  mote1.released = WPAD_ButtonsUp(0);
  mote1.held = WPAD_ButtonsHeld(0);

  mote1.nunchukX=0.0f;
  mote1.nunchukY=0.0f;
  expansion_t ex;
  WPAD_Expansion(WPAD_CHAN_0, &ex);
  if (ex.type == WPAD_EXP_NUNCHUK)
  {
      joystick_t n = ex.nunchuk.js;
      if (n.mag > mote1.nunchukDeadzone)
      {
          // Angle of 0 means up.
          // 90 right, 180 down, 270 left

          float rad = n.ang * PI / 180.0f;
          float x = 0;
          float y = -1.0f;
          float dirx = cos(rad) * x - sin(rad) * y;
          float diry = sin(rad) * x + cos(rad) * y;
          mote1.nunchukX = dirx * n.mag;
          mote1.nunchukY = diry * n.mag;
      }
  }
}

