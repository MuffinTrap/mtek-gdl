#include <mgdl/mgdl-controller.h>

gdl::WiiController::WiiController()
{
	channel = 0;
	ZeroAllInputs();
}


void gdl::WiiController::SetChannelNumber ( int channel )
{
	this->channel = channel;
}

int gdl::WiiController::GetChannel()
{
	return channel;
}


bool gdl::WiiController::ButtonPress(int buttonEnum) {
  return (pressedButtons & buttonEnum) != 0;
}

bool gdl::WiiController::ButtonRelease(int buttonEnum) {
  return (releasedButtons & buttonEnum) != 0;
}

bool gdl::WiiController::ButtonHeld(int buttonEnum) {
  return (heldButtons & buttonEnum) != 0;
}

gdl::ControllerVec2 gdl::WiiController::GetCursorPosition() {

  gdl::ControllerVec2 d;
  d.xAxis = cursorX;
  d.yAxis = cursorY;
  return d;
}

gdl::ControllerVec2 gdl::WiiController::GetNunchukJoystickDirection(float deadzone)
{
  nunchukJoystickDeadzone = deadzone;
  gdl::ControllerVec2 d;
  d.xAxis = nunchukJoystickDirectionX;
  d.yAxis = nunchukJoystickDirectionY;
  return d;
}

float gdl::WiiController::GetRoll() {
  return roll;
}

void gdl::WiiController::ZeroAllInputs()
{
	pressedButtons = 0;
	releasedButtons = 0;
	heldButtons = 0;
	nunchukJoystickDirectionX = 0.0f;
	nunchukJoystickDirectionY = 0.0f;
	nunchukJoystickDeadzone = 0.0f;
	cursorX = 0.0f;
	cursorY = 0.0f;
	roll = 0.0f;
}

void gdl::WiiController::SetButtonDown ( int buttonEnum )
{
	pressedButtons += buttonEnum;
	heldButtons += buttonEnum;
}
void gdl::WiiController::SetButtonUp ( int buttonEnum )
{
	heldButtons -= buttonEnum;
}

