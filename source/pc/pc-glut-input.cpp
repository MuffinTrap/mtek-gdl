

#ifndef GEKKO
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-opengl.h>

#include <stdio.h>

WiiController glutController;

#define ascii_1  49
#define ascii_2  50
#define ascii_w  119
#define ascii_a  97
#define ascii_s  115
#define ascii_d  100
#define ascii_q  113
#define ascii_e  101

#define ascii_ESC 27


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
	glutController._cursorX = x;

	// In OpenGL the Y increases up
	// but these coordinates the y increses down
	glutController._cursorY = y;
}

void keyboardDown(unsigned char key, int x, int y) {
	if (key == ascii_ESC) { // ASCII code for 'Escape'
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonHome);
	}
	else if (key == ascii_w) {
		glutController._nunchukJoystickDirectionY = -1.0f;
	}
	else if (key == ascii_s) {
		glutController._nunchukJoystickDirectionY = 1.0f;
	}
	else if (key == ascii_a) {
		glutController._nunchukJoystickDirectionX = -1.0f;
	}
	else if (key == ascii_d) {
		glutController._nunchukJoystickDirectionX = 1.0f;
	}
	else if (key == ascii_1) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::Button1);
	}
	else if (key == ascii_2) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::Button2);
	}
	else if (key == ascii_q) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonMinus);
	}
	else if (key == ascii_e) {
		_WiiController_SetButtonDown(&glutController, WiiButtons::ButtonPlus);
	}
}

void keyboardUp(unsigned char key, int x, int y) {
	if (key == ascii_ESC) { // ASCII code for 'Escape'
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonHome);
	}
	else if (key == ascii_w) {
		glutController._nunchukJoystickDirectionY = 0.0f;
	}
	else if (key == ascii_s) {
		glutController._nunchukJoystickDirectionY = 0.0f;
	}
	else if (key == ascii_a) {
		glutController._nunchukJoystickDirectionX = 0.0f;
	}
	else if (key == ascii_d) {
		glutController._nunchukJoystickDirectionX = 0.0f;
	}
	else if (key == ascii_1) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::Button1);
	}
	else if (key == ascii_2) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::Button2);
	}
	else if (key == ascii_q) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonMinus);
	}
	else if (key == ascii_e) {
		_WiiController_SetButtonUp(&glutController, WiiButtons::ButtonPlus);
	}
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
