#ifdef MGDL_PLATFORM_MAC

#include <mgdl/pc/mgdl-joystick.h>
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
