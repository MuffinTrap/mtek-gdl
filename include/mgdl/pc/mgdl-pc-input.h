
#pragma once

#include <mgdl/mgdl-controller.h>

extern WiiController kbmController; /**< Represents keyboard and mouse input as controller */

#if defined(MGDL_PLATFORM_MSYS2) || defined(MGDL_PLATFORM_MAC)
extern WiiController gamepadController; /**< Represents gamepad as controller (needed for glut)*/
extern bool gamepadDetected;
#endif
extern "C"
{
	void InitPCInput();
#if defined(MGDL_PLATFORM_WINDOWS)
	// XInput
#	define MGDL_VK_MOUSE_LEFT 0
#	define MGDL_VK_MOUSE_RIGHT 1

	void keyboardDown(UINT vcode);
	void keyboardUp(UINT vcode);
	void mouseDown(UINT button);
	void mouseUp(UINT button);
	void mouseMove(int x, int y);
#else
	// Glut functions
	void keyboardUp(unsigned char key, int x, int y);
	void keyboardDown(unsigned char key, int x, int y);
	void specialKeyUp(int key, int x, int y);
	void specialKeyDown(int key, int x, int y);
	void mouseKey(int button, int state, int x, int y);
	void mouseMove(int x, int y);
#if defined(MGDL_PLATFORM_MSYS2) || defined(MGDL_PLATFORM_MAC)
	void joystick(unsigned int buttonmask, int x, int y, int z);
#endif
#endif
}
