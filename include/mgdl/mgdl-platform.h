#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"

#include <cstring>
#include <string.h>
#include <string>
#include <functional>

struct Platform
{
	const char* name;
	short screenWidth;
	short screenHeight;
	float aspectRatio;
	gdl::ScreenAspect aspect;
	float _deltaTimeS;
	float _elapsedTimeS;
	u32 _elapsedUpdates;
};

// These functions are implemented in pc-platform.cpp or wii-platform.cpp

void Platform_Init(const char* name,
						gdl::ScreenAspect screenAspect,
						std::function<void()> initCallback,
						std::function<void()> updateCallback,
						std::function<void()> drawCallback,
						u32 initFlags = 0 );

WiiController* Platform_GetController(int controllerNumber);
void Platform_DoProgramExit();

Platform* Platform_GetSingleton();
float Platform_GetDeltaTime();
float Platform_GetElapsedSeconds();
u32 Platform_GetElapsedUpdates();

