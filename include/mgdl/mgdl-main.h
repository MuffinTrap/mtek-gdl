#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-sprite.h>


#ifdef __cplusplus
extern "C"
{
#endif


void mgdl_InitSystem(const char* name,
				ScreenAspect screenAspect,
				CallbackFunction initCallback,
				CallbackFunction frameCallback,
				u32 initFlags);

// TODO Move all loading to AssetManager to get memory status and other stats
PNGFile* mgdl_LoadPNG(const char* filename);
Image* mgdl_LoadImage(const char* filename, TextureFilterModes filterMode);
Image* mgdl_LoadImagePNG(PNGFile* png, TextureFilterModes filterMode);

Sound* mgdl_LoadSound(const char* filename);
Music* mgdl_LoadOgg(const char* filename);
Music* mgdl_LoadWav(const char* filename);

Scene* mgdl_LoadFBX(const char* filename);
Sprite* mgdl_LoadSprite(const char* filename, short spriteWidth, short spriteHeight);

Font* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter);
Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
Font* mgdl_LoadFontSelective(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters);

WiiController* mgdl_GetController(int controllerNumber);

u16 mgdl_GetScreenWidth(void);
u16 mgdl_GetScreenHeight(void);
float mgdl_GetAspectRatio(void);
float mgdl_GetElapsedSeconds(void);
float mgdl_GetDeltaTime(void);

void mgdl_DoProgramExit(void);

#ifdef __cplusplus
}
#endif
