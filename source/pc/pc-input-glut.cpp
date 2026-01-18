#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)

#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-logger.h>

#include <stdio.h>

WiiController kbmController;

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
	Log_Info("Init Glut keyboard and mouse input\n");
    // Input callbacks and init
    glutKeyboardFunc(keyboardDown); // Register the keyboard callback
    glutKeyboardUpFunc(keyboardUp); // Register the keyboard release

    glutSpecialFunc(specialKeyDown); // Register the keyboard callback
    glutSpecialUpFunc(specialKeyUp); // Register the keyboard release

    glutMouseFunc(mouseKey);        // Register mouse buttons and movement
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);
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



	#pragma GCC diagnostic pop
#ifdef MGDL_PLATFORM_MAC
	#pragma clang diagnostic pop
#endif

#endif
