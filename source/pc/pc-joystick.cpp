
#include <mgdl/pc/mgdl-joystick.h>
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
    Joystick_Init(stick);
    return stick;
}

void Joystick_ZeroInputs(Joystick* joystick)
{
    WiiController_ZeroAllInputs(&joystick->controller);
}

bool Joystick_IsConnected(Joystick* stick)
{
    return stick->isConnected;
}

WiiController* Joystick_GetController(Joystick* stick)
{
    return &stick->controller;
}

float Joystick_NormalizeAxis(short value)
{
    return (float)value / 32767.0f;
}
