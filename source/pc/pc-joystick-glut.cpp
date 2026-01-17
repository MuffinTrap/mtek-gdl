#if defined(MGDL_PLATFORM_MSYS2) || defined(MGDL_PLATFORM_MAC)
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-opengl.h>

Joystick* joysticks[4];

static WiiButtons ButtonToWiiButton(int button)
{
	switch (button)
	{
	case 0: return ButtonA; break;
	case 1: return ButtonB; break;
	case 2: return Button1; break;
	case 3: return Button2; break;

	case 4: return ButtonC; break;
	case 5: return ButtonZ; break;
	// NOTE Cannot notice pressing the Xbox logo button
	case 6: return ButtonMinus; break;
	case 7: return ButtonPlus; break;

	case 8: return ButtonUp; break;
	case 9: return ButtonDown; break;
	case 10: return ButtonLeft; break;
	case 11: return ButtonRight; break;
	}
	return ButtonNone;
}

static int buttons[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

// NOTE
// This is not good enough for Xbox Gamepad, only detects 8 buttons
// and one stick
void joystickCallback(unsigned int buttonmask, int x, int y, int z)
{
	Joystick* joystick = joysticks[0];
	WiiController* C = &joystick->controller;
	//Log_InfoF("Glut Joystick. Mask: %d (%d, %d, %d)\n", buttonmask, x, y, z);

	// Axis input is from -1000 to 1000
	C->m_nunchukJoystickDirectionX = (float)x / 1000.0f;
	C->m_nunchukJoystickDirectionY = (float)y / 1000.0f;

	// Go through all buttons
	for (int b = 0; b < 12; b++)
	{
		// If this button is on the mask
		// it is currently down
		if (buttonmask & (0x01 << b))
		{
			if (buttons[b] == 0)
			{
				// This button went down
				buttons[b] = 1;
				WiiController_SetButtonDown(C, ButtonToWiiButton(b));
			//	Log_InfoF("Button %d down %s\n", b, WiiController_GetButtonSymbol(ButtonToWiiButton(b)));
			}
		}
		else if (buttons[b] == 1)
		{
			// This button was down in the past

			// This button went up
			WiiController_SetButtonUp(C, ButtonToWiiButton(b));
			//Log_InfoF("Button %d up\n", b);

			buttons[b] = 0;
		}
	}
}

void Joystick_Init()
{
	for (int i = 0; i < 4; i++)
	{
		joysticks[i] = Joystick_Create(i);
	}

	// Glut can only see 1 joystick
	Joystick* joystick = joysticks[0];

	int hasJoystick = glutDeviceGet(GLUT_HAS_JOYSTICK);
	if (hasJoystick)
	{
		Log_InfoF("Found glut joystick Buttons: %d Axes %d\n",
			glutDeviceGet(GLUT_JOYSTICK_BUTTONS),
			glutDeviceGet(GLUT_JOYSTICK_AXES));

		glutJoystickFunc(joystickCallback, 0);

		joystick->isConnected = true;
	}
	else
	{
		Log_Info("Glut did not find any joysticks\n");
	}
}

void Joystick_Deinit()
{
	glutJoystickFunc(NULL, 0);
	for (int i = 0; i < 4; i++)
	{
		free(joysticks[i]);
	}
}


void Joystick_ReadInputs()
{
	// Tells glut to call the registered callback
	glutForceJoystickFunc();
}

void Joystick_StartFrame()
{
	Joystick* joystick = joysticks[0];
	WiiController* C = &joystick->controller;
	//Log_InfoF("Glut Joystick. Mask: %d (%d, %d, %d)\n", buttonmask, x, y, z);
	WiiController_StartFrame(C);
}

#endif
