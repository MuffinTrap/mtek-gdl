#ifdef MGDL_PLATFORM_WINDOWS
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

#include <xinput.h>

// HACKS: Load the functions from dll
// into our own function pointers

// Allows to change the signature more easily
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)

typedef X_INPUT_GET_STATE(xinput_get_state);

// Create stub functions in case we cannot load the
// dll
X_INPUT_GET_STATE(XInputGetStateStub)
{
    return (0);
}

// Connect function pointers to stubs
static DWORD (*XInput_GetState_FuncP)(DWORD uid, XINPUT_STATE* statP) = XInputGetStateStub;

// Find the functions from dll
static void Win32LoadXInput(void)
{
    // Try the earliest version
    HMODULE XinputLib = NULL;
	XinputLib = LoadLibraryA("xinput1_3.dll");
    if (XinputLib == NULL)
    {
		XinputLib = LoadLibraryA("xinput1_4.dll");
    }
    if (XinputLib)
    {
        XInput_GetState_FuncP = (xinput_get_state*)GetProcAddress(XinputLib, "XInputGetState");
    }
}

void InitPCInput()
{
    Win32LoadXInput();
}

void Joystick_Init(Joystick* joystick)
{
    XINPUT_STATE stateOut = {};
    DWORD result = XInput_GetState_FuncP(joystick->index, &stateOut);
    if (result == ERROR_SUCCESS)
    {
        joystick->isConnected = true;
        Log_InfoF("Joystick Init index %d\n", joystick->index);
    }
    else if (result == ERROR_DEVICE_NOT_CONNECTED)
    {
        joystick->isConnected = false;
        Log_ErrorF("Could not open joystick %d\n", joystick->index);
    }
}

static WiiButtons ButtonToWiiButton(int XinputButton)
{
    switch (XinputButton)
    {
    case XINPUT_GAMEPAD_A: return ButtonA; break;
    case XINPUT_GAMEPAD_B: return ButtonB; break;
    case XINPUT_GAMEPAD_X: return Button1; break;
    case XINPUT_GAMEPAD_Y: return Button2; break;

    case XINPUT_GAMEPAD_LEFT_SHOULDER: return ButtonC; break;
    case XINPUT_GAMEPAD_RIGHT_SHOULDER: return ButtonZ; break;
	// NOTE Cannot notice pressing the Xbox logo button
    case XINPUT_GAMEPAD_START: return ButtonPlus; break;
    case XINPUT_GAMEPAD_BACK: return ButtonMinus; break;

    case XINPUT_GAMEPAD_DPAD_UP: return ButtonUp; break;
    case XINPUT_GAMEPAD_DPAD_DOWN: return ButtonDown; break;
    case XINPUT_GAMEPAD_DPAD_LEFT: return ButtonLeft; break;
    case XINPUT_GAMEPAD_DPAD_RIGHT: return ButtonRight; break;
    }
    return ButtonNone;

}

static const int ButtonAmount = 12;
static const int GamepadButtons[] = {
    XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_B,
    XINPUT_GAMEPAD_X,
    XINPUT_GAMEPAD_Y,

    XINPUT_GAMEPAD_LEFT_SHOULDER,
    XINPUT_GAMEPAD_RIGHT_SHOULDER,
    XINPUT_GAMEPAD_START,
    XINPUT_GAMEPAD_BACK,

    XINPUT_GAMEPAD_DPAD_UP,
    XINPUT_GAMEPAD_DPAD_DOWN,
    XINPUT_GAMEPAD_DPAD_LEFT,
    XINPUT_GAMEPAD_DPAD_RIGHT
};

void Joystick_ReadInputs(Joystick* joystick)
{
    XINPUT_STATE stateOut = {};
    DWORD result = XInput_GetState_FuncP(joystick->index, &stateOut);
    if (result == ERROR_SUCCESS)
    {
        joystick->isConnected = true;
        // Check if state has changed
        if (stateOut.dwPacketNumber == joystick->lastPacketNumber)
        {
            return;
        }
        joystick->lastPacketNumber = stateOut.dwPacketNumber;
        // Read all buttons
        WiiController* c = &joystick->controller;
        for (int buttonIndex = 0; buttonIndex < ButtonAmount; buttonIndex += 1)
        {
            u32 wiiButton = ButtonToWiiButton(GamepadButtons[buttonIndex]);
            bool isDown = WiiController_ButtonHeld(c, wiiButton);

			if (Flag_IsSet(stateOut.Gamepad.wButtons, GamepadButtons[buttonIndex]))
			{
				// If this button is not down, check the controller state
				if (isDown == false)
				{ 
                    WiiController_SetButtonDown(c, wiiButton);
                }
            }
            else if (isDown == true)
            {
                // Button 
                WiiController_SetButtonUp(c, ButtonToWiiButton(GamepadButtons[buttonIndex]));
            }
        }

        // Read axis
        c->m_nunchukJoystickDirectionX = Joystick_NormalizeAxis(stateOut.Gamepad.sThumbLX);
        // Y axis is inverted
        SHORT leftStickY = stateOut.Gamepad.sThumbLY;
        if (leftStickY < 0)
        {
            // from -32768 to 36767
            leftStickY += 1;
        }
		leftStickY *= -1;
        c->m_nunchukJoystickDirectionY = Joystick_NormalizeAxis(leftStickY);

		c->m_roll = Joystick_NormalizeAxis(stateOut.Gamepad.sThumbRX) * M_PI;
		c->m_pitch = Joystick_NormalizeAxis(stateOut.Gamepad.sThumbRY) * M_PI;

        // TODO Yaw
    }
    else if (result == ERROR_DEVICE_NOT_CONNECTED)
    {
        joystick->isConnected = false;
    }
    // Nop
}
void Joystick_Disconnect(Joystick* joystick)
{
    // nop
}

#endif


