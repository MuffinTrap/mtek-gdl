#pragma once

/**
 * @file mgdl-joystick.h
 * @ingroup input win linux macos
 * @brief Interface to read joystick input on different PC platforms
 *
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
    bool isConnected;
    WiiController controller;

#ifdef MGDL_PLATFORM_LINUX
    int linux_device_index;
#endif
#ifdef MGDL_PLATFORM_WINDOWS
    int lastPacketNumber; // From XInput
#endif
    struct axis_state* axes;
};
typedef struct Joystick Joystick;

#define MGDL_JOY_AXIS_LSTICK 0
#define MGDL_JOY_AXIS_RSTICK_X 1
#define MGDL_JOY_AXIS_RSTICK_Y 2
#define MGDL_JOY_AXIS_DPAD 3

Joystick* Joystick_Create(short index);
WiiController* Joystick_GetController(Joystick* stick);
bool Joystick_IsConnected(Joystick* stick);
void Joystick_Init(Joystick* joystick);
void Joystick_ReadInputs(Joystick* joystick);
void Joystick_ZeroInputs(Joystick* joystick);

void Joystick_Disconnect(Joystick* joystick);

float Joystick_NormalizeAxis(short value);
