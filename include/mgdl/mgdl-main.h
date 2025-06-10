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

#include <functional>
#include <string>

extern "C"
{

void mgdl_InitSystem(const char* name,
				gdl::ScreenAspect screenAspect,
				std::function<void()> initCallback,
				std::function<void()> updateCallback,
				std::function<void()> drawCallback,
				u32 initFlags);

// TODO Move all loading to AssetManager to get memory status and other stats
PNGFile* mgdl_LoadPNG(const char* filename);
Image* mgdl_LoadImage(const char* filename, gdl::TextureFilterModes filterMode);
Image* mgdl_LoadImagePNG(PNGFile* png, gdl::TextureFilterModes filterMode);

Sound* mgdl_LoadSound(const char* filename);
Music* mgdl_LoadOgg(const char* filename);
Music* mgdl_LoadWav(const char* filename);

Scene* mgdl_LoadFBX(const char* filename);
Sprite* mgdl_LoadSprite(const char* filename, short spriteWidth, short spriteHeight);

Font* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter);
Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
Font* mgdl_LoadFontSelective(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters);

WiiController* mgdl_GetController(int controllerNumber);

u16 mgdl_GetScreenWidth();
u16 mgdl_GetScreenHeight();
float mgdl_GetAspectRatio();
float mgdl_GetElapsedSeconds();
float mgdl_GetDeltaTime();

void mgdl_DoProgramExit();

}
