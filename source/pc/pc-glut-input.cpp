

#ifndef GEKKO
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-opengl.h>

#include <stdio.h>

gdl::WiiController glutController;

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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-parameter"

void mouseKey(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			glutController.SetButtonDown(gdl::WiiButtons::ButtonA);
		}
		else {
			glutController.SetButtonUp(gdl::WiiButtons::ButtonA);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN) {
			glutController.SetButtonDown(gdl::WiiButtons::ButtonB);
		}
		else {
			glutController.SetButtonUp(gdl::WiiButtons::ButtonB);
		}
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		// Nop
	}
}

void mouseMove(int x, int y) {
	glutController.cursorX = x;

	// In OpenGL the Y increases up
	// but these coordinates the y increses down
	glutController.cursorY = y;
}

void keyboardDown(unsigned char key, int x, int y) {
	printf("Keyboard down %d\n", key);
	if (key == ascii_ESC) { // ASCII code for 'Escape'
		glutController.SetButtonDown(gdl::WiiButtons::ButtonHome);
	}
	else if (key == ascii_w) {
		glutController.nunchukJoystickDirectionY = -1.0f;
	}
	else if (key == ascii_s) {
		glutController.nunchukJoystickDirectionY = 1.0f;
	}
	else if (key == ascii_a) {
		glutController.nunchukJoystickDirectionX = -1.0f;
	}
	else if (key == ascii_d) {
		glutController.nunchukJoystickDirectionX = 1.0f;
	}
	else if (key == ascii_1) {
		glutController.SetButtonDown(gdl::WiiButtons::Button1);
	}
	else if (key == ascii_2) {
		glutController.SetButtonDown(gdl::WiiButtons::Button2);
	}
	else if (key == ascii_q) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonMinus);
	}
	else if (key == ascii_e) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonPlus);
	}
}

void keyboardUp(unsigned char key, int x, int y) {
	printf("Keyboard up %d\n", key);
	if (key == ascii_ESC) { // ASCII code for 'Escape'
		glutController.SetButtonUp(gdl::WiiButtons::ButtonHome);
	}
	else if (key == ascii_w) {
		glutController.nunchukJoystickDirectionY = 0.0f;
	}
	else if (key == ascii_s) {
		glutController.nunchukJoystickDirectionY = 0.0f;
	}
	else if (key == ascii_a) {
		glutController.nunchukJoystickDirectionX = 0.0f;
	}
	else if (key == ascii_d) {
		glutController.nunchukJoystickDirectionX = 0.0f;
	}
	else if (key == ascii_1) {
		glutController.SetButtonUp(gdl::WiiButtons::Button1);
		printf("Button 1 up\n");
	}
	else if (key == ascii_2) {
		glutController.SetButtonUp(gdl::WiiButtons::Button2);
		printf("Button 2 up\n");
	}
	else if (key == ascii_q) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonMinus);
	}
	else if (key == ascii_e) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonPlus);
	}
}

void specialKeyDown(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonUp);
	}
    else if (key == GLUT_KEY_DOWN) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonDown);
	}
    else if (key == GLUT_KEY_LEFT) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonLeft);
	}
    else if (key == GLUT_KEY_RIGHT) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonRight);
	}
	else if (key == GLUT_KEY_F5) {
		glutController.SetButtonDown(gdl::WiiButtons::ButtonPlus);
	}
}

void specialKeyUp(int key, int x, int y) {

    if (key == GLUT_KEY_UP) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonUp);
	}
    else if (key == GLUT_KEY_DOWN) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonDown);
	}
    else if (key == GLUT_KEY_LEFT) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonLeft);
	}
    else if (key == GLUT_KEY_RIGHT) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonRight);
	}
	else if (key == GLUT_KEY_F5) {
		glutController.SetButtonUp(gdl::WiiButtons::ButtonPlus);
	}
}

#pragma GCC diagnostic pop
#pragma clang diagnostic pop

#endif
