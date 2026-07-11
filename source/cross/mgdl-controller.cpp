#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-assert.h>

void WiiController_Init(WiiController* controller, u8 channel)
{
	controller->m_channel = channel;
	WiiController_ZeroAllInputs(controller);
}

void WiiController_ZeroAllInputs(WiiController* controller)
{
	controller->m_pressedButtons = 0;
	controller->m_releasedButtons = 0;
	controller->m_heldButtons = 0;
	controller->m_nunchukJoystickDirectionX = 0.0f;
	controller->m_nunchukJoystickDirectionY = 0.0f;
	controller->m_cursorX = 0.0f;
	controller->m_cursorY = 0.0f;
	controller->m_roll = 0.0f;
	controller->m_yaw = 0.0f;
	controller->m_pitch = 0.0f;
}

void WiiController_AddStateFrom(WiiController* dest, WiiController* source)
{
	ASSERT_DEBUG(dest != nullptr && source != nullptr);
	dest->m_pressedButtons = (dest->m_pressedButtons | source->m_pressedButtons);
	dest->m_releasedButtons = (dest->m_releasedButtons | source->m_releasedButtons);
	dest->m_heldButtons = (dest->m_heldButtons | source->m_heldButtons);

	if (fabsf(dest->m_nunchukJoystickDirectionX) < fabsf(source->m_nunchukJoystickDirectionX))
	{
		dest->m_nunchukJoystickDirectionX = source->m_nunchukJoystickDirectionX;
	}
	if (fabsf(dest->m_nunchukJoystickDirectionY) < fabsf(source->m_nunchukJoystickDirectionY))
	{
		dest->m_nunchukJoystickDirectionY = source->m_nunchukJoystickDirectionY;
	}

	if (fabsf(dest->m_roll) < fabsf(source->m_roll))
	{
		dest->m_roll = source->m_roll;
	}
	if (fabsf(dest->m_yaw) < fabsf(source->m_yaw))
	{
		dest->m_yaw = source->m_yaw;
	}
	if (fabsf(dest->m_pitch) < fabsf(source->m_pitch))
	{
		dest->m_pitch = source->m_pitch;
	}
	dest->m_cursorX += source->m_cursorX;
	dest->m_cursorY += source->m_cursorY;
}

void WiiController_ReplaceWith(WiiController* dest, WiiController* source)
{
	ASSERT_DEBUG(dest != nullptr && source != nullptr);
	dest->m_pressedButtons = source->m_pressedButtons;
	dest->m_releasedButtons = source->m_releasedButtons;
	dest->m_heldButtons = source->m_heldButtons;
	dest->m_nunchukJoystickDirectionX = source->m_nunchukJoystickDirectionX;
	dest->m_nunchukJoystickDirectionY = source->m_nunchukJoystickDirectionY;
	dest->m_roll = source->m_roll;
	dest->m_yaw = source->m_yaw;
	dest->m_pitch = source->m_pitch;
	dest->m_cursorX = source->m_cursorX;
	dest->m_cursorY = source->m_cursorY;
}

void WiiController_StartFrame(WiiController* controller)
{
	ASSERT_DEBUG(controller != nullptr);
	controller->m_pressedButtons = 0;
	controller->m_releasedButtons = 0;
}

bool WiiController_ButtonPress(WiiController* controller, u32 buttonEnum) {
	ASSERT_DEBUG(controller != nullptr);
	return Flag_IsSetAny(controller->m_pressedButtons, buttonEnum);
}

bool WiiController_ButtonRelease(WiiController* controller, u32 buttonEnum) {
	ASSERT_DEBUG(controller != nullptr);
	return Flag_IsSetAny(controller->m_releasedButtons, buttonEnum);
}

bool WiiController_ButtonHeld(WiiController* controller, u32 buttonEnum) {
	ASSERT_DEBUG(controller != nullptr);
	return Flag_IsSetAny(controller->m_heldButtons, buttonEnum);
}

Vector2 WiiController_GetCursorPosition(WiiController* controller) {
	ASSERT_DEBUG(controller != nullptr);

	Vector2 d;
	d.x = controller->m_cursorX;
	d.y = controller->m_cursorY;
	return d;
}

Vector2 WiiController_GetNunchukJoystickDirection(WiiController* controller)
{
	ASSERT_DEBUG(controller != nullptr);
	Vector2 d;
	d.x = controller->m_nunchukJoystickDirectionX;
	d.y = controller->m_nunchukJoystickDirectionY;
	return d;
}

Vector2 WiiController_GetLeftStickDirection(WiiController* controller)
{
	return WiiController_GetNunchukJoystickDirection(controller);
}

Vector2 WiiController_GetRightStickDirection(WiiController* controller)
{
	ASSERT_DEBUG(controller != nullptr);
	return Vector2New(
		controller->m_rightStickDirectionX,
		controller->m_rightStickDirectionY);
}

float WiiController_GetPitch(WiiController* controller) {
	ASSERT_DEBUG(controller != nullptr);
	return controller->m_pitch;
}

float WiiController_GetYaw(WiiController* controller) {
	ASSERT_DEBUG(controller != nullptr);
	return controller->m_yaw;
}

float WiiController_GetRoll(WiiController* controller) {
	ASSERT_DEBUG(controller != nullptr);
	return controller->m_roll;
}

void WiiController_SetButtonDown (WiiController* controller, u32 buttonEnum )
{
	ASSERT_DEBUG(controller != nullptr);
	controller->m_pressedButtons = Flag_SetAll(controller->m_pressedButtons, buttonEnum);
	controller->m_heldButtons = Flag_SetAll(controller->m_heldButtons, buttonEnum);
}
void WiiController_SetButtonUp (WiiController* controller, u32 buttonEnum )
{
	ASSERT_DEBUG(controller != nullptr);
	controller->m_heldButtons = Flag_UnsetAll(controller->m_heldButtons,buttonEnum);
	controller->m_releasedButtons = Flag_SetAll(controller->m_releasedButtons, buttonEnum);
}

const char* WiiController_GetButtonSymbol(WiiController* controller, u32 buttonEnum)
{
	static const char* names[] ={ "?", "2", "1", "B", "A", "-", "H", "L", "R", "D", "U", "+", "Z", "C", "X", "Y", "LZ", "RZ", "L2", "R2" };
	if(controller->m_type == Controller_Wiimote || controller->m_type == Controller_Nunchuk)
	{
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
		}
	}
	else
	{
		switch (buttonEnum)
		{
		case ButtonClassicUp: return names[10];
		case ButtonClassicDown: return names[9];
		case ButtonClassicLeft: return names[7];
		case ButtonClassicRight: return names[8];

		case ButtonClassicMinus: return names[4];
		case ButtonClassicPlus: return names[11];
		case ButtonClassicHome: return names[6];

		case ButtonClassicA: return names[4];
		case ButtonClassicB: return names[3];
		case ButtonClassicX: return names[14]; break;
		case ButtonClassicY: return names[15]; break;

		case ButtonClassicLeftShoulder: return names[16]; break;
		case ButtonClassicRightShoulder: return names[17]; break;
		case ButtonClassicLeftTrigger: return names[18]; break;
		case ButtonClassicRightTrigger: return names[19]; break;
		default: return names[0]; break;
		}
	};
}

bool WiiController_HasNunchuk(WiiController* controller)
{
	return  controller->m_type == Controller_Nunchuk || controller->m_type == Controller_Xbox360Pad;
}

bool WiiController_HasClassicController(WiiController* controller)
{
	return  controller->m_type == Controller_ClassicController || controller->m_type == Controller_Xbox360Pad;
}

void WiiController_SetCursorPosition(WiiController* controller, float x, float y)
{
	controller->m_cursorX = x;
	controller->m_cursorX = y;
}
