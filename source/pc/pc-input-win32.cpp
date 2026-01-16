#if defined(MGDL_PLATFORM_WINDOWS)
#include <mgdl/pc/mgdl-pc-input.h>
#include <WinUser.h>

WiiController kbmController;

// NOTE InitPCInput is in pc-joystick-win32.cpp

static const UINT VK_1 = 0x31;
static const UINT VK_2 = 0x32;

static const UINT VK_A = 0x41;
static const UINT VK_D = 0x44;
static const UINT VK_S = 0x53;
static const UINT VK_W = 0x57;

static const UINT VK_I = 0x49;
static const UINT VK_J = 0x4A;
static const UINT VK_K = 0x4B;
static const UINT VK_L = 0x47;

static const UINT VK_C = 0x43;
static const UINT VK_Z = 0x5A;
static const UINT VK_U = 0x55;
static const UINT VK_O = 0x4F;

void keyboardDown(UINT vcode)
{
	switch (vcode)
	{
	case VK_ESCAPE:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonHome);
		break;
	case VK_W:
		kbmController.m_nunchukJoystickDirectionY = -1.0f;
		break;
	case VK_S:
		kbmController.m_nunchukJoystickDirectionY = 1.0f;
		break;
	case VK_A:
		kbmController.m_nunchukJoystickDirectionX = -1.0f;
		break;
	case VK_D:
		kbmController.m_nunchukJoystickDirectionX = 1.0f;
		break;
	case VK_1:
		WiiController_SetButtonDown(&kbmController, WiiButtons::Button1);
		break;
	case VK_2:
		WiiController_SetButtonDown(&kbmController, WiiButtons::Button2);
		break;
	case VK_OEM_MINUS:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonMinus);
		break;
	case VK_OEM_PLUS:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonPlus);
		break;

	case VK_Z:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonZ);
		break;
	case VK_C:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonC);
		break;

	case VK_U:
		kbmController.m_roll = -M_PI;
		break;
	case VK_O:
		kbmController.m_roll = M_PI;
		break;
	case VK_I:
		kbmController.m_pitch = -M_PI;
		break;
	case VK_K:
		kbmController.m_pitch = M_PI;
		break;
	case VK_J:
		kbmController.m_yaw = -M_PI;
		break;
	case VK_L:
		kbmController.m_yaw = M_PI;
		break;

	case VK_UP:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonUp);
		break;
	case VK_DOWN:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonDown);
		break;
	case VK_LEFT:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonLeft);
		break;
	case VK_RIGHT:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonRight);
		break;
	};
}
void keyboardUp(UINT vcode)
{
	switch (vcode)
	{
	case VK_ESCAPE:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonHome);
		break;
	case VK_W:
		kbmController.m_nunchukJoystickDirectionY = -0.0f;
		break;
	case VK_S:
		kbmController.m_nunchukJoystickDirectionY = 0.0f;
		break;
	case VK_A:
		kbmController.m_nunchukJoystickDirectionX = -0.0f;
		break;
	case VK_D:
		kbmController.m_nunchukJoystickDirectionX = 0.0f;
		break;
	case VK_1:
		WiiController_SetButtonUp(&kbmController, WiiButtons::Button1);
		break;
	case VK_2:
		WiiController_SetButtonUp(&kbmController, WiiButtons::Button2);
		break;
	case VK_OEM_MINUS:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonMinus);
		break;
	case VK_OEM_PLUS:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonPlus);
		break;

	case VK_Z:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonZ);
		break;
	case VK_C:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonC);
		break;


	case VK_U:
		kbmController.m_roll = -0;
		break;
	case VK_O:
		kbmController.m_roll = 0;
		break;
	case VK_I:
		kbmController.m_pitch = -0;
		break;
	case VK_K:
		kbmController.m_pitch = 0;
		break;
	case VK_J:
		kbmController.m_yaw = -0;
		break;
	case VK_L:
		kbmController.m_yaw = 0;
		break;
	case VK_UP:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonUp);
		break;
	case VK_DOWN:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonDown);
		break;
	case VK_LEFT:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonLeft);
		break;
	case VK_RIGHT:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonRight);
		break;
	};

}

void mouseDown(UINT button)
{
	switch (button)
	{
	case MGDL_VK_MOUSE_LEFT:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonA);
		break;
	case MGDL_VK_MOUSE_RIGHT:
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonB);
		break;
	}
}
void mouseUp(UINT button)
{
	switch (button)
	{
	case MGDL_VK_MOUSE_LEFT:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonA);
		break;
	case MGDL_VK_MOUSE_RIGHT:
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonB);
		break;
	}
}

void mouseMove(int x, int y)
{
	kbmController.m_cursorX = x;
	kbmController.m_cursorY = y;
}

#endif
