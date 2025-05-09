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

void InitSystem(const char* name,
				gdl::ScreenAspect screenAspect,
				std::function<void()> initCallback,
				std::function<void()> updateCallback,
				std::function<void()> drawCallback,
				u32 initFlags);

// TODO Move all loading to AssetManager to get memory status and other stats
PNGFile* LoadPNG(const char* filename);
Image* LoadImage(const char* filename, gdl::TextureFilterModes filterMode);
Image* LoadImagePNG(PNGFile* png, gdl::TextureFilterModes filterMode);

Sound* LoadSound(const char* filename);
Music* LoadOgg(const char* filename);

Scene* LoadFBX(const char* filename);
Sprite* LoadSprite(const char* filename, short spriteWidth, short spriteHeight);

Font* LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter);
Font* LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
Font* LoadFontSelective(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters);

WiiController* mgdl_GetController(int controllerNumber);

u16 mgdl_GetScreenWidth();
u16 mgdl_GetScreenHeight();
float mgdl_GetAspectRatio();
float mgdl_GetElapsedSeconds();
float mgdl_GetDeltaTime();

void mgdl_DoProgramExit();

}
