#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-sound.h>

#include <functional>
#include <string>

void InitSystem(const char* name,
				gdl::ScreenAspect screenAspect,
				std::function<void()> initCallback,
				std::function<void()> updateCallback,
				std::function<void()> drawCallback,
				u32 initFlags = 0 );

// TODO Move all loading to AssetManager to get memory status and other stats
PNGFile* LoadPNG(const char* filename);
gdl::Image* LoadImageFile(const char* filename, gdl::TextureFilterModes filterMode);
gdl::Image* LoadImagePNG(PNGFile* png, gdl::TextureFilterModes filterMode);

gdl::Sound* LoadSound(const char* filename);
gdl::Sound* LoadOgg(const char* filename);


Font* LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter);
Font* LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
Font* LoadFontSelective(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters);

void DoProgramExit();

WiiController* GetController(int controllerNumber);

u16 GetScreenWidth();
u16 GetScreenHeight();
float GetAspectRatio();
float GetElapsedSeconds();
float GetDeltaTime();
