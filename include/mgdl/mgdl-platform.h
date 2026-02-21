#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
/**
 * @file mgdl-platform.h
 * @ingroup core
 * @brief Virtual functions to be defined by each platform
 */

#define MGDL_MAX_CONTROLLERS 4

/**
 * @class Platform
 * @brief Information about platform
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

	Viewport viewport; /**< The active viewport inside window */

	u32 deltaTimeMs;
	u32 elapsedTimeMs;
	u32 elapsedFrames;
	u32 applicationStartMs;

	WiiController controllers[MGDL_MAX_CONTROLLERS];
	WiiController kbmcontroller;
	// Joystick mapping to controllers
	// index : controller number
	int joysticIndexToControllerMapping[MGDL_MAX_CONTROLLERS];

	// Splash screen variables
	int waitElapsedMS;
	bool showHoldAMessage;
	float splashProgress;

	// For holding until a is held for 1 second
	float aHoldTimer;

	u32 initFlags;
};
typedef struct Platform Platform;


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

void Platform_SetWindowNameAndAspect(const char* windowName, ScreenAspect aspect);
void Platform_InitAudio(void);
void Platform_FrameStart(void);
void Platform_RenderStart(void);
void Platform_RenderEnd(void);
void Platform_FrameEnd(void);
void Platform_DoProgramExit(void);

// Controller functions
void Platform_InitControllers(void);
void Platform_MapJoystickToController(int joystickIndex, int controllerIndex);

/**
 * @brief Tells if controller is connected.
 */
bool Platform_IsControllerConnected(int controllerIndex);

void Platform_ReadControllers(void);
void Platform_StartNextFrameControllers(void);

/**
* @brief Returns the controller at given index.
* @param controllerNumber Number of controller. 0-3 are valid. If available, mouse and keyboard and first gamepad are combined to controller 0
* @returns The controller if it is connected, controller 0 otherwise
*/
WiiController* Platform_GetController(int controllerNumber);

// Timing functions

float Platform_GetDeltaTime(void);
float Platform_GetElapsedSeconds(void);
u32 Platform_GetElapsedFrames(void);


void Platform_RenderAHold();
void Platform_RenderSplash();
void Platform_UpdateDeltaTime(u32 elapsedMilliseconds);

bool Platform_IncreaseAHoldAndTest();
void Platform_ResetTime();

void Platform_ResizeWindow(int newWidth, int newHeight);
void Platform_SetFullscreen(bool enabled);

Platform* Platform_GetSingleton(void);

Viewport Platform_GetViewport(void);
float Platform_GetAspectRatio(void);

#ifdef __cplusplus
}
#endif
