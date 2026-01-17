
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

/**
* @file pc-joystick.cpp
* @brief Shared joystick functions
*/

Joystick* Joystick_Create(short index)
{
    Joystick* stick = (Joystick*)malloc(sizeof(Joystick));
    stick->index = index;
    stick->axisCount = 0;
    stick->buttonCount = 0;
    stick->isConnected = false;
    return stick;
}

float Joystick_NormalizeAxis(short value)
{
    return (float)value / 32767.0f;
}

void Joystick_AddToController(WiiController* controller, int index)
{
	Joystick* joystick = joysticks[index];
    WiiController_AddStateFrom(controller, &joystick->controller);
}

void Joystick_ReplaceController(WiiController* controller, int index)
{
	Joystick* joystick = joysticks[index];
    WiiController_ReplaceWith(controller, &joystick->controller);
}

void Joystick_ZeroInputs()
{
    for (int i = 0; i < 4; i++)
    {
        Joystick* joystick = joysticks[i];
        WiiController_ZeroAllInputs(&joystick->controller);
    }
}

bool Joystick_IsConnected(int index)
{
	return joysticks[index]->isConnected;
}
