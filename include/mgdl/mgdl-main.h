#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-sprite.h>
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-main.h
 * @ingroup core
 * @brief Main functions to start and use the library
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initializes the systems and creates a window.
 * @details Call this function to open the window and start the main loop. Don't call other mgdl functions before this except
 * SetSplashScreenColors and SetSplashScreenDuration
 * @param name Name of the window. Does not have any meaning on Wii.
 * @param screenAspect Aspect ratio of the screen. Auto means to use the platform default. On Wii this is what is set in the console settings.
 * @param initCallback This is called once after the system is initialized. Use it to load all assets.
 * @param frameCallback This is called once each frame. The framebuffer is swapped automatically after it returns.
 * @param quitCallback This is called when the HOME button is pressed on controller or window is closed for some other reason. Can be NULL.
 * @param initFlags Combination of initialization flags to set full screen, wait for A button hold or to show splash screen.
 *
 * @note On Wii it is possible to set frameCallback to nullptr. This means the main loop will not engage and you have
 * to run the loop yourself.
 * @code
 * // In your main loop:
 * Platform_FrameStart();
 * Platform_ReadControllers();
 * Audio_Update(); // If you use mgdl functions to play sounds
 * Platform_RenderStart();
 * // Your game code here
 * Platform_RenderEnd();
 * Platform_FrameEnd();
 * @endcode
 */
void mgdl_InitSystem(const char* name,
				ScreenAspect screenAspect,
				CallbackFunction initCallback,
				CallbackFunction frameCallback,
				CallbackFunction quitCallback,
				u32 initFlags);

/**
 * @brief Gets a controller by number
 * @param controllerNumber Number of controller 0 - 3
 * @returns Pointer to controller. Will return the first controller on invalid controller number.
 */
WiiController* mgdl_GetController(int controllerNumber);
/**
 * @brief Gets the full viewport
 * @returns Viewport representing the full rendering area. On PC platforms this might be different than window or screen area.
 */
Viewport mgdl_GetViewport(void);
/**
 * @brief Gets the viewport width
 * @returns Width of the viewport
 */
int mgdl_GetScreenWidth(void);
/**
 * @brief Gets the viewport height
 * @returns Height of the viewport
 */
int mgdl_GetScreenHeight(void);
/**
 * @brief Gets the viewport aspect ratio
 * @returns Aspect ratio of the viewport
 */
float mgdl_GetAspectRatio(void);
/**
 * @brief Returns how many seconds have passed since the first frame
 * @returns Seconds since first frame callback
 */
float mgdl_GetElapsedSeconds(void);
/**
 * @brief Returns how many seconds have passed since previous frame
 * @returns Seconds since previous frame callback
 */
float mgdl_GetDeltaTime(void);
/**
 * @brief Returns how many frames have been rendered
 * @returns Amount of frames rendered
 */
u32 mgdl_GetElapsedFrames(void);

/**
 * @brief Quits the program
 */
void mgdl_DoProgramExit(void);

#ifdef __cplusplus
}
#endif
