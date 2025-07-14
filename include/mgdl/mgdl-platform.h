#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"

struct Platform
{
	const char* windowName;
	short screenWidth;
	short screenHeight;
	float aspectRatio;
	ScreenAspect aspect;
	float _deltaTimeS;
	float _elapsedTimeS;
	u32 _elapsedUpdates;
};
typedef struct Platform Platform;

// These functions are implemented in pc-platform.cpp or wii-platform.cpp
#ifdef __cplusplus
extern "C"
{
#endif

void Platform_Init(const char* windowName,
						ScreenAspect screenAspect,
						CallbackFunction initCallback,
						CallbackFunction frameCallback,
						u32 initFlags);

struct WiiController* Platform_GetController(int controllerNumber);
void Platform_DoProgramExit(void);

struct Platform* Platform_GetSingleton(void);
float Platform_GetDeltaTime(void);
float Platform_GetElapsedSeconds(void);
u32 Platform_GetElapsedUpdates(void);

#ifdef __cplusplus
}
#endif
