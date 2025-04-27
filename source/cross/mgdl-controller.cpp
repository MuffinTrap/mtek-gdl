#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-util.h>

void WiiController_Init(WiiController* controller, u8 channel)
{
	controller->_channel = channel;
	WiiController_ZeroAllInputs(controller);
}

void WiiController_ZeroAllInputs(WiiController* controller)
{
	controller->_pressedButtons = 0;
	controller->_releasedButtons = 0;
	controller->_heldButtons = 0;
	controller->_nunchukJoystickDirectionX = 0.0f;
	controller->_nunchukJoystickDirectionY = 0.0f;
	controller->_cursorX = 0.0f;
	controller->_cursorY = 0.0f;
	controller->_roll = 0.0f;
}

void WiiController_StartFrame(WiiController* controller)
{
	controller->_pressedButtons = 0;
	controller->_releasedButtons = 0;
}

bool WiiController_ButtonPress(WiiController* controller, int buttonEnum) {
	return (controller->_pressedButtons & buttonEnum) != 0;
}

bool WiiController_ButtonRelease(WiiController* controller, int buttonEnum) {
	return (controller->_releasedButtons & buttonEnum) != 0;
}

bool WiiController_ButtonHeld(WiiController* controller, int buttonEnum) {
	return (controller->_heldButtons & buttonEnum) != 0;
}

vec2 WiiController_GetCursorPosition(WiiController* controller) {

	vec2 d;
	d.x = controller->_cursorX;
	d.y = controller->_cursorY;
	return d;
}

vec2 WiiController_GetNunchukJoystickDirection(WiiController* controller)
{
	vec2 d;
	d.x = controller->_nunchukJoystickDirectionX;
	d.y = controller->_nunchukJoystickDirectionY;
	return d;
}

float WiiController_GetRoll(WiiController* controller) {
	return controller->_roll;
}

void _WiiController_SetButtonDown (WiiController* controller, int buttonEnum )
{
	controller->_pressedButtons += buttonEnum;
	controller->_heldButtons += buttonEnum;
}
void _WiiController_SetButtonUp (WiiController* controller, int buttonEnum )
{
	controller->_heldButtons -= buttonEnum;
	controller->_releasedButtons += buttonEnum;
}

