#if defined(MGDL_PLATFORM_MSYS2) || defined(MGDL_PLATFORM_MAC)
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/mgdl-logger.h>

void Joystick_Init(Joystick* joystick)
{
	int hasJoystick = glutDeviceGet(GLUT_HAS_JOYSTICK);
	if (hasJoystick)
	{
		Log_InfoF("Found glut joystick Buttons: %d Axes %d\n",
			glutDeviceGet(GLUT_JOYSTICK_BUTTONS),
			glutDeviceGet(GLUT_JOYSTICK_AXES));

	}
	joystick->isConnected = (hasJoystick > 0) || gamepadDetected;
}

void Joystick_ReadInputs(Joystick* joystick)
{
	// NOP
}
#endif