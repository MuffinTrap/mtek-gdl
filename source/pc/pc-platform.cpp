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

// This is called when window is resized
void Platform_ResizeWindow(int newWidth, int newHeight)
{
    Platform* plat = Platform_GetSingleton();
    // Keep aspect ratio
    float width = (float)newWidth/(float)plat->windowWidth;
    float height = (float)newHeight/(float)plat->windowHeight;
    float scale = width < height ? width : height;

    // Use the scale to center the graphics
    float scaledWidth = scale*(float)plat->windowWidth;
    float scaledHeight = scale*(float)plat->windowHeight;

    int left = 0;
    int top = 0;

    if (scaledWidth < newWidth)
    {
        // Black bars on sides
        left = (newWidth - scaledWidth)/2;
    }
    if (scaledHeight < newHeight)
    {
        // Black bars on top and bottom
        top = (newHeight - scaledHeight)/2;
    }

    // But keep showing the internal resolution scaled
    glViewport(left, top, scaledWidth, scaledHeight);

    // Update window size
    plat->windowWidth = newWidth;
    plat->windowHeight = newHeight;
}

void Platform_RenderSplash(Platform* platform)
{
    platform->splashProgress = DrawSplashScreen(platform->deltaTimeS, platform->showHoldAMessage, platform->aHoldTimer);
    Platform_RenderEnd();
}


// Nothing is shown but program waits for A button to be held down
void Platform_RenderAHold()
{
    Platform_RenderEnd();
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
void Platform_ResetTime(Platform* platform)
{
    platform->waitElapsedMS = 0;
    platform->aHoldTimer = 0.0f;
    platform->splashProgress = 0.0f;

}
