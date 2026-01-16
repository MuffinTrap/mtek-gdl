

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)

#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-logger.h>

#include <stdio.h>



WiiController kbmController;
#if defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)
	WiiController gamepadController;
	bool gamepadDetected = false;
#endif

#define ascii_1  49
#define ascii_2  50
#define ascii_plus  43
#define ascii_minus  45
// D pad
#define ascii_w  119
#define ascii_a  97
#define ascii_s  115
#define ascii_d  100

// Nunchuck buttons
#define ascii_c  99
#define ascii_z  122

// Roll
#define ascii_u  117
#define ascii_o  111
// Pitch
#define ascii_i  105
#define ascii_k  107
// Yaw
#define ascii_j  106
#define ascii_l  108

#define ascii_ESC 27

// Others

#define ascii_q  113
#define ascii_e  101


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef MGDL_PLATFORM_MAC
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

void InitPCInput()
{
    // Input callbacks and init
    glutKeyboardFunc(keyboardDown); // Register the keyboard callback
    glutKeyboardUpFunc(keyboardUp); // Register the keyboard release

    glutSpecialFunc(specialKeyDown); // Register the keyboard callback
    glutSpecialUpFunc(specialKeyUp); // Register the keyboard release

    glutMouseFunc(mouseKey);        // Register mouse buttons and movement
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);

#if defined(MGDL_PLATFORM_MSYS2) || defined(MGDL_PLATFORM_MAC)
	glutJoystickFunc(joystick, 15);
#endif
}

void mouseKey(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonA);
		}
		else {
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonA);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonB);
		}
		else {
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonB);
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		// Nop
	}
}

void mouseMove(int x, int y) {
	kbmController.m_cursorX = x;

	// In OpenGL the Y increases up
	// but these coordinates the y increses down
    y = mgdl_GetScreenHeight() - y;
	kbmController.m_cursorY = y;
}

void keyboardDown(unsigned char key, int x, int y) {
	switch(key)
	{
		case ascii_ESC: // ASCII code for 'Escape'
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonHome);
			break;
		case ascii_w:
			kbmController.m_nunchukJoystickDirectionY = -1.0f;
			break;
		case ascii_s:
			kbmController.m_nunchukJoystickDirectionY = 1.0f;
			break;
		case ascii_a:
			kbmController.m_nunchukJoystickDirectionX = -1.0f;
			break;
		case ascii_d:
			kbmController.m_nunchukJoystickDirectionX = 1.0f;
			break;
		case ascii_1:
			WiiController_SetButtonDown(&kbmController, WiiButtons::Button1);
			break;
		case ascii_2:
			WiiController_SetButtonDown(&kbmController, WiiButtons::Button2);
			break;
		case ascii_minus:
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonMinus);
			break;
		case ascii_plus:
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonPlus);
			break;

		case ascii_z:
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonZ);
			break;
		case ascii_c:
			WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonC);
			break;

		case ascii_u:
			kbmController.m_roll = -M_PI;
			break;
		case ascii_o:
			kbmController.m_roll = M_PI;
			break;
		case ascii_i:
			kbmController.m_pitch = -M_PI;
			break;
		case ascii_k:
			kbmController.m_pitch = M_PI;
			break;
		case ascii_j:
			kbmController.m_yaw = -M_PI;
			break;
		case ascii_l:
			kbmController.m_yaw = M_PI;
			break;
		};
	}

void keyboardUp(unsigned char key, int x, int y) {
	switch(key)
	{
		case ascii_ESC: // ASCII code for 'Escape'
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonHome);
			break;
		case ascii_w:
			kbmController.m_nunchukJoystickDirectionY = 0.0f;
			break;
		case ascii_s:
			kbmController.m_nunchukJoystickDirectionY = 0.0f;
			break;
		case ascii_a:
			kbmController.m_nunchukJoystickDirectionX = 0.0f;
			break;
		case ascii_d:
			kbmController.m_nunchukJoystickDirectionX = 0.0f;
			break;
		case ascii_1:
			WiiController_SetButtonUp(&kbmController, WiiButtons::Button1);
			break;
		case ascii_2:
			WiiController_SetButtonUp(&kbmController, WiiButtons::Button2);
			break;
		case ascii_minus:
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonMinus);
			break;
		case ascii_plus:
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonPlus);
			break;
		case ascii_z:
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonZ);
			break;
		case ascii_c:
			WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonC);
			break;
		case ascii_u:
			kbmController.m_roll = 0.0f;
			break;
		case ascii_o:
			kbmController.m_roll = 0.0f;
			break;
		case ascii_i:
			kbmController.m_pitch = 0.0f;
			break;
		case ascii_k:
			kbmController.m_pitch = 0.0f;
			break;
		case ascii_j:
			kbmController.m_yaw = 0.0f;
			break;
		case ascii_l:
			kbmController.m_yaw = 0.0f;
			break;
	};
}

void specialKeyDown(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonUp);
	}
    else if (key == GLUT_KEY_DOWN) {
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonDown);
	}
    else if (key == GLUT_KEY_LEFT) {
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonLeft);
	}
    else if (key == GLUT_KEY_RIGHT) {
		WiiController_SetButtonDown(&kbmController, WiiButtons::ButtonRight);
	}
}

void specialKeyUp(int key, int x, int y) {

    if (key == GLUT_KEY_UP) {
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonUp);
	}
    else if (key == GLUT_KEY_DOWN) {
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonDown);
	}
    else if (key == GLUT_KEY_LEFT) {
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonLeft);
	}
    else if (key == GLUT_KEY_RIGHT) {
		WiiController_SetButtonUp(&kbmController, WiiButtons::ButtonRight);
	}
}

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

// The buttonmask does not stay on when buttons are
// kept down.
// Only send up event if mask does not include the button for 2 frames

void joystick(unsigned int buttonmask, int x, int y, int z)
{
	gamepadDetected = true;
	//Log_InfoF("Glut Joystick. Mask: %d (%d, %d, %d)\n", buttonmask, x, y, z);

	// Axis input is from -1000 to 1000
	gamepadController.m_nunchukJoystickDirectionX = (float)x / 1000.0f;
	gamepadController.m_nunchukJoystickDirectionY = (float)y / 1000.0f;

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
				buttons[b] = 2; // Wait for 2 frames
				WiiController_SetButtonDown(&gamepadController, ButtonToWiiButton(b));
				Log_InfoF("Button %d down %s\n", b, WiiController_GetButtonSymbol(ButtonToWiiButton(b)));
			}
		}
		else if (buttons[b] > 0)
		{
			// This button was down in the past
			buttons[b] -= 1;
			if (buttons[b] == 0)
			{
				// This button went up
				WiiController_SetButtonUp(&gamepadController, ButtonToWiiButton(b + 1));
				Log_InfoF("Button %d up\n", b);
			}
		}
	}
}


#pragma GCC diagnostic pop
#ifdef MGDL_PLATFORM_MAC
#pragma clang diagnostic pop
#endif

#endif
