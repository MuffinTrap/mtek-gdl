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
gdl::PNGFile* LoadPNG(std::string filename);
gdl::Image* LoadImageFile(std::string filename, gdl::TextureFilterModes filterMode);
gdl::Image* LoadImagePNG(gdl::PNGFile* png, gdl::TextureFilterModes filterMode);

gdl::Sound* LoadSound(std::string filename);
gdl::Sound* LoadOgg(std::string filename);


Font* LoadFont(std::string filename, short characterWidth, short characterHeight, char firstCharacter);
Font* LoadFontCustom(std::string filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
Font* LoadFontSelective(std::string filename, short characterWidth, short characterHeight, short charactersPerRow, std::string characters);

void DoProgramExit();

WiiController* GetController(int controllerNumber);

u16 GetScreenWidth();
u16 GetScreenHeight();
float GetAspectRatio();
float GetElapsedSeconds();
float GetDeltaTime();
