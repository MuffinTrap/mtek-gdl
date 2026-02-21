#include <mgdl/mgdl-platform.h>

void Platform_SetWindowNameAndAspect(const char* windowName, ScreenAspect aspect)
{
	Platform* platform = Platform_GetSingleton();
    platform->windowName = windowName;
    platform->screenWidth = MGDL_WII_WIDTH;
    platform->screenHeight = MGDL_WII_HEIGHT;
	platform->aspect = aspect;
    switch(aspect)
    {
        case ScreenAuto:
            platform->windowWidth = 854;
            platform->windowHeight = 480;
            platform->aspectRatio = 16.0f/9.0f;
            break;
        case Screen4x3:
            platform->windowWidth = 640;
            platform->windowHeight = 480;
            platform->aspectRatio = 4.0f/3.0f;
            break;
        case Screen16x9:
            // Wii only outputs 640x480, but in this format it is shown wider
            platform->windowWidth = 854;
            platform->windowHeight = 480;
            platform->aspectRatio = 16.0f/9.0f;
            break;
    };
    platform->viewport.left = 0;
    platform->viewport.top = platform->windowHeight;
    platform->viewport.width = platform->windowWidth;
    platform->viewport.height = platform->windowHeight;
}

void Platform_UpdateDeltaTime(u32 elapsedTimeMilliseconds)
{
	Platform* platform = Platform_GetSingleton();
	u32 lastElapsed = platform->elapsedTimeMs;
	platform->elapsedTimeMs = elapsedTimeMilliseconds - platform->applicationStartMs;
	platform->deltaTimeMs = platform->elapsedTimeMs - lastElapsed;
}

float Platform_GetDeltaTime(void)
{
	Platform* platform = Platform_GetSingleton();
    return (float)platform->deltaTimeMs / 1000.0f;
}

float Platform_GetElapsedSeconds(void)
{
	Platform* platform = Platform_GetSingleton();
    return (float)platform->elapsedTimeMs / 1000.0f;
}

u32 Platform_GetElapsedFrames(void)
{
	Platform* platform = Platform_GetSingleton();
    return platform->elapsedFrames;
}

Viewport Platform_GetViewport(void)
{
	Platform* platform = Platform_GetSingleton();
    return platform->viewport;
}
float Platform_GetAspectRatio(void)
{
	Platform* platform = Platform_GetSingleton();
    return platform->aspectRatio;
}

WiiController* Platform_GetController(int controllerNumber)
{
	if (controllerNumber >= 0 && controllerNumber < MGDL_MAX_CONTROLLERS)
	{
		Platform* platform = Platform_GetSingleton();
		return &platform->controllers[controllerNumber];
	}
	return nullptr;
}
