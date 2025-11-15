#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/pc/mgdl-joystick.h>

/**
 * @file mgdl-platform.h
 * @ingroup core
 * @brief Virtual functions to be defined by each platform
 */

struct Platform
{
	const char* windowName;
	// Screen size is the rendering size
	short screenWidth;
	short screenHeight;

	// Window size is the size of the window
	// the contents can be scaled and letterboxed
	short windowWidth;
	short windowHeight;
	float aspectRatio;
	ScreenAspect aspect;
	float deltaTimeS;
	float elapsedTimeS;
	u32 elapsedUpdates;

	// Joysticks and controllers
	Joystick* gamepads[4];

	// Splash screen variables
	int waitElapsedMS;
	bool showHoldAMessage;
	float splashProgress;

	// For holding until a is held for 1 second
	float aHoldTimer;
};
typedef struct Platform Platform;

void Platform_SetWindowNameAndSize(Platform* platform, const char* windowName, ScreenAspect aspect);

const int MGDL_WII_WIDTH = 640;
const int MGDL_WII_HEIGHT = 480;

// These functions are implemented in pc-platform.cpp or wii-platform.cpp
#ifdef __cplusplus
extern "C"
{
#endif

void Platform_Init(const char* windowName,
						ScreenAspect screenAspect,
						CallbackFunction initCallback,
						CallbackFunction frameCallback,
						CallbackFunction quitCallback,
						u32 initFlags);

struct WiiController* Platform_GetController(int controllerNumber);
void Platform_DoProgramExit(void);

struct Platform* Platform_GetSingleton(void);
float Platform_GetDeltaTime(void);
float Platform_GetElapsedSeconds(void);
u32 Platform_GetElapsedUpdates(void);

void Platform_InitControllers();
void Platform_UpdateControllers();
void Platform_UpdateSplash(int value);
void Platform_RenderSplash(Platform* platform);

void Platform_UpdateAHold(int value);
void Platform_RenderAHold(); // Cannot have parameter to comply with glutDisplayFunc

void Platform_RenderEnd();

bool Platform_IncreaseAHoldAndTest(Platform* platform);
void Platform_ResetTime(Platform* platform);

void Platform_ResizeWindow(int newWidth, int newHeight);

#ifdef __cplusplus
}
#endif
