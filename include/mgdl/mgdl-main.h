#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-sprite.h>

/**
 * @file mgdl-main.h
 * @ingroup core
 * @brief Main functions to start and use the library
 */

struct Font;

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

// TODO Move all loading to AssetManager to get memory status and other stats
PNGFile* mgdl_LoadPNG(const char* filename);
Texture* mgdl_LoadTexture(const char* filename, TextureFilterModes filterMode);
Texture* mgdl_LoadTexturePNG(PNGFile* png, TextureFilterModes filterMode);

Sound* mgdl_LoadSoundWav(const char* filename);
Sound* mgdl_LoadSoundOgg(const char* filename);
Sound* mgdl_LoadSoundMp3(const char* filename);

Scene* mgdl_LoadFBX(const char* filename);
Sprite* mgdl_LoadSprite(const char* filename, short spriteWidth, short spriteHeight);

Font* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter);
Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
Font* mgdl_LoadFontSelective(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters);

WiiController* mgdl_GetController(int controllerNumber);

// TODO mgdl_GetQuitReason(void) HOME pressed, Error or Window closed

Viewport mgdl_GetViewport(void);
int mgdl_GetScreenWidth(void);
int mgdl_GetScreenHeight(void);
float mgdl_GetAspectRatio(void);
float mgdl_GetElapsedSeconds(void);
float mgdl_GetDeltaTime(void);
u32 mgdl_GetElapsedFrames(void);

void mgdl_DoProgramExit(void);

#ifdef __cplusplus
}
#endif
