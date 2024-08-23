
#pragma once

#include <mgdl/mgdl-controller.h>

extern "C"
{
	void GetGlutController(gdl::WiiController& controllerInOut);
	void keyboardUp(unsigned char key, int x, int y);
	void keyboardDown(unsigned char key, int x, int y);
	void specialKeyUp(int key, int x, int y);
	void specialKeyDown(int key, int x, int y);
	void mouseKey(int button, int state, int x, int y);
	void mouseMove(int x, int y);
}
