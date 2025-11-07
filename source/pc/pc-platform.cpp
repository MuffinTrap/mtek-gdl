#include <mgdl/mgdl-platform.h>

#include <mgdl/mgdl-splash.h>

/**
* @file mgdl-platform.cpp
* @brief Functions shared by all platforms
*/

void Platform_SetWindowNameAndSize(Platform* platform, const char* windowName, ScreenAspect aspect)
{
    platform->windowName = windowName;
    platform->screenWidth = MGDL_WII_WIDTH;
    platform->screenHeight = MGDL_WII_HEIGHT;
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
}

void Platform_RenderSplash(Platform* platform)
{
    platform->splashProgress = DrawSplashScreen(platform->deltaTimeS, platform->showHoldAMessage, platform->aHoldTimer);
    Platform_RenderEnd(platform);
}


// Nothing is shown but program waits for A button to be held down
void Platform_RenderAHold(Platform* platform)
{
    Platform_RenderEnd(platform);
}

bool Platform_IncreaseAHoldAndTest(Platform* platform)
{
    if (WiiController_ButtonHeld(Platform_GetController(0), WiiButtons::ButtonA))
    {
        platform->aHoldTimer += platform->deltaTimeS;
        if (platform->aHoldTimer >= 1.0f)
        {
            return true;
        }
    }
    else
    {
        platform->aHoldTimer = 0.0f;
    }
    return false;
}
