#ifdef MGDL_PLATFORM_WINDOWS
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

void Joystick_Init(Joystick* joystick)
{
    joystick->index = -1;
}
void Joystick_ReadInputs(Joystick* joystick)
{
    // Nop
}
void Joystick_Disconnect(Joystick* joystick)
{
    // nop
}

#endif


