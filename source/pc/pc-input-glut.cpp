

#ifndef GEKKO
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-main.h>

#include <stdio.h>

WiiController glutController;

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

void mouseKey(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonA);
		}
		else {
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonA);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonB);
		}
		else {
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonB);
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		// Nop
	}
}

void mouseMove(int x, int y) {
	glutController.m_cursorX = x;

	// In OpenGL the Y increases up
	// but these coordinates the y increses down
    y = mgdl_GetScreenHeight() - y;
	glutController.m_cursorY = y;
}

void keyboardDown(unsigned char key, int x, int y) {
	switch(key)
	{
		case ascii_ESC: // ASCII code for 'Escape'
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonHome);
			break;
		case ascii_w:
			glutController.m_nunchukJoystickDirectionY = -1.0f;
			break;
		case ascii_s:
			glutController.m_nunchukJoystickDirectionY = 1.0f;
			break;
		case ascii_a:
			glutController.m_nunchukJoystickDirectionX = -1.0f;
			break;
		case ascii_d:
			glutController.m_nunchukJoystickDirectionX = 1.0f;
			break;
		case ascii_1:
			_WiiController_SetButtonDown(&glutController, WiiButtons::Button1);
			break;
		case ascii_2:
			_WiiController_SetButtonDown(&glutController, WiiButtons::Button2);
			break;
		case ascii_minus:
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonMinus);
			break;
		case ascii_plus:
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonPlus);
			break;

		case ascii_z:
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonZ);
			break;
		case ascii_c:
			_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonC);
			break;

		case ascii_u:
			glutController.m_roll = -M_PI;
			break;
		case ascii_o:
			glutController.m_roll = M_PI;
			break;
		case ascii_i:
			glutController.m_pitch = -M_PI;
			break;
		case ascii_k:
			glutController.m_pitch = M_PI;
			break;
		case ascii_j:
			glutController.m_yaw = -M_PI;
			break;
		case ascii_l:
			glutController.m_yaw = M_PI;
			break;
		};
	}

void keyboardUp(unsigned char key, int x, int y) {
	switch(key)
	{
		case ascii_ESC: // ASCII code for 'Escape'
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonHome);
			break;
		case ascii_w:
			glutController.m_nunchukJoystickDirectionY = 0.0f;
			break;
		case ascii_s:
			glutController.m_nunchukJoystickDirectionY = 0.0f;
			break;
		case ascii_a:
			glutController.m_nunchukJoystickDirectionX = 0.0f;
			break;
		case ascii_d:
			glutController.m_nunchukJoystickDirectionX = 0.0f;
			break;
		case ascii_1:
			_WiiController_SetButtonUp(&glutController, WiiButtons::Button1);
			break;
		case ascii_2:
			_WiiController_SetButtonUp(&glutController, WiiButtons::Button2);
			break;
		case ascii_minus:
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonMinus);
			break;
		case ascii_plus:
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonPlus);
			break;
		case ascii_z:
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonZ);
			break;
		case ascii_c:
			_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonC);
			break;
		case ascii_u:
			glutController.m_roll = 0.0f;
			break;
		case ascii_o:
			glutController.m_roll = 0.0f;
			break;
		case ascii_i:
			glutController.m_pitch = 0.0f;
			break;
		case ascii_k:
			glutController.m_pitch = 0.0f;
			break;
		case ascii_j:
			glutController.m_yaw = 0.0f;
			break;
		case ascii_l:
			glutController.m_yaw = 0.0f;
			break;
	};
}

void specialKeyDown(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonUp);
	}
    else if (key == GLUT_KEY_DOWN) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonDown);
	}
    else if (key == GLUT_KEY_LEFT) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonLeft);
	}
    else if (key == GLUT_KEY_RIGHT) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonRight);
	}
	else if (key == GLUT_KEY_F5) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonPlus);
	}
}

void specialKeyUp(int key, int x, int y) {

    if (key == GLUT_KEY_UP) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonUp);
	}
    else if (key == GLUT_KEY_DOWN) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonDown);
	}
    else if (key == GLUT_KEY_LEFT) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonLeft);
	}
    else if (key == GLUT_KEY_RIGHT) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonRight);
	}
	else if (key == GLUT_KEY_F5) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonPlus);
	}
}

#pragma GCC diagnostic pop
#ifdef MGDL_PLATFORM_MAC
#pragma clang diagnostic pop
#endif

#endif
