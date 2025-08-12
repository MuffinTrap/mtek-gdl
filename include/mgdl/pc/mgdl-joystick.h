#pragma once

/**
 * @file mgdl-joystick.h
 * @brief Interface to read joystick input on different PC platforms
 */

#include <mgdl/mgdl-controller.h>

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

struct Joystick
{
    short index;
    sizetype axisCount;
    sizetype buttonCount;
    WiiController controller;
#ifdef MGDL_PLATFORM_LINUX
    int linux_device_index;
#endif
    struct axis_state* axes;
};
typedef struct Joystick Joystick;

Joystick* Joystick_Create(short index);
WiiController* Joystick_GetController(Joystick* stick);
bool Joystick_IsConnected(Joystick* stick);
void Joystick_Init(Joystick* joystick);
void Joystick_ReadInputs(Joystick* joystick);
void Joystick_ZeroInputs(Joystick* joystick);

void Joystick_Disconnect(Joystick* joystick);
