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

float WiiController_GetPitch(WiiController* controller) {
	return controller->_pitch;
}

float WiiController_GetYaw(WiiController* controller) {
	return controller->_yaw;
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

const char* WiiController_GetButtonSymbol(int buttonEnum)
{
	static const char* names[] ={ "?", "2", "1", "B", "A", "-", "H", "L", "R", "D", "U", "+", "Z", "C" };
	switch (buttonEnum)
	{
		case Button2 :  return names[1];break;
		case Button1 : return names[2];break;
		case ButtonB : return names[3];break;
		case ButtonA :  return names[4];break;
		case ButtonMinus :  return names[5];break;
		case ButtonHome : return names[6];break;
		case ButtonLeft : return names[7];break;
		case ButtonRight : return names[8];break;
		case ButtonDown :  return names[9];break;
		case ButtonUp :  return names[10];break;
		case ButtonPlus : return names[11];break;
		case ButtonZ : return names[12];break;
		case ButtonC : return names[13];break;
		default: return names[0]; break;
	};
}

