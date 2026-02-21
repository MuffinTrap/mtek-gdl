#ifndef GEKKO
#include <mgdl/mgdl-platform.h>

#include <mgdl/mgdl-splash.h>
#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/mgdl-util.h>

/**
* @file mgdl-platform.cpp
* @brief Functions shared by all PC platforms
*/

static Platform* platformPC = Platform_GetSingleton();

// This is called when window is resized
void Platform_ResizeWindow(int newWidth, int newHeight)
{
    // Keep aspect ratio
    float width = (float)newWidth/(float)platformPC->windowWidth;
    float height = (float)newHeight/(float)platformPC->windowHeight;
    float scale = width < height ? width : height;

    // Use the scale to center the graphics
    float scaledWidth = scale*(float)platformPC->windowWidth;
    float scaledHeight = scale*(float)platformPC->windowHeight;

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
    platformPC->viewport.left = left;
    platformPC->viewport.top = top;
    platformPC->viewport.width = scaledWidth;
    platformPC->viewport.height = scaledHeight;

    // Update window size
    platformPC->windowWidth = newWidth;
    platformPC->windowHeight = newHeight;
}

void Platform_RenderSplash()
{
    platformPC->splashProgress = DrawSplashScreen(Platform_GetDeltaTime(), platformPC->showHoldAMessage, platformPC->aHoldTimer);
    Platform_RenderEnd();
}

// Nothing is shown but program waits for A button to be held down
void Platform_RenderAHold()
{
    Platform_RenderEnd();
}

bool Platform_IncreaseAHoldAndTest()
{
    if (WiiController_ButtonHeld(Platform_GetController(0), WiiButtons::ButtonA))
    {
        platformPC->aHoldTimer += Platform_GetDeltaTime();
        if (platformPC->aHoldTimer >= 1.0f)
        {
            return true;
        }
    }
    else
    {
        platformPC->aHoldTimer = 0.0f;
    }
    return false;
}

void Platform_ResetTime()
{
    platformPC->waitElapsedMS = 0;
    platformPC->aHoldTimer = 0.0f;
    platformPC->splashProgress = 0.0f;
}

void Platform_StartNextFrameControllers()
{
	// Start recording input for next update
	WiiController_StartFrame(&kbmController);
	Joystick_StartFrame();

	for (int i = 0; i < 4; i++)
	{
		WiiController* controller = Platform_GetController(i);
		// Reset controller for next frame
		WiiController_StartFrame(controller);
	}
}

void Platform_InitControllers()
{
	InitPCInput();
	Joystick_Init();
	Joystick_ZeroInputs();

	for (int i = 0; i < MGDL_MAX_CONTROLLERS; i++)
	{
		WiiController* c = &platformPC->controllers[i];
		WiiController_Init(c, i);
		WiiController_ZeroAllInputs(c);
		WiiController_StartFrame(c);
	}

	WiiController_Init(&kbmController, 0);
	WiiController_ZeroAllInputs(&kbmController);
	WiiController_StartFrame(&kbmController);


    for (int ji = 0; ji < MGDL_MAX_CONTROLLERS; ji++)
    {
        platformPC->joysticIndexToControllerMapping[ji] = 0;
    }
}


void Platform_MapJoystickToController(int joystickIndex, int controllerIndex)
{
	if (joystickIndex >= 0 && joystickIndex < MGDL_MAX_CONTROLLERS &&
		controllerIndex >= 0 && controllerIndex < MGDL_MAX_CONTROLLERS)
	{
		platformPC->joysticIndexToControllerMapping[joystickIndex] = controllerIndex;
	}
}

bool Platform_IsControllerConnected(int controllerIndex)
{
	if (controllerIndex == 0)
	{
		return true;
	}

    if (controllerIndex >= 0 && controllerIndex < MGDL_MAX_CONTROLLERS)
    {
        for (int ji = 0; ji < MGDL_MAX_CONTROLLERS; ji++)
        {
            int mappedController = platformPC->joysticIndexToControllerMapping[ji];
            if (mappedController == controllerIndex)
            {
                return Joystick_IsConnected(ji);
            }
        }
    }
    return false;
}


void Platform_ReadControllers()
{
    // First Controller is always mouse and keyboard
    WiiController* firstController = Platform_GetController(0);

    // Read mouse and keyboard into first controller
    WiiController_ReplaceWith(firstController, &kbmController);

    // Update state of all joysticks
    Joystick_ReadInputs();

    // Read if any of the joysticks should be
    // fed into controller 0 in addition to mouse and keyboard
    // If first joystick is connected, add it
    // to controller 0
    int startJoystickIndex = 0;
    if (platformPC->joysticIndexToControllerMapping[0] == 0 && Joystick_IsConnected(0))
    {
        Joystick_AddToController(firstController, 0);
        startJoystickIndex = 1;
    }

    // other controllers reading
    for (int i = startJoystickIndex; i < MGDL_MAX_CONTROLLERS; i++)
    {
        if (Joystick_IsConnected(i))
        {
            Joystick_ReplaceController(Platform_GetController(platformPC->joysticIndexToControllerMapping[i]), i);
        }
    }
}

#endif
