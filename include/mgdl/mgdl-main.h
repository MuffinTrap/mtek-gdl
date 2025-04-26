#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-sound.h>

#include <functional>
#include <string>

namespace gdl
{
	void InitSystem(const char* name,
					gdl::ScreenAspect screenAspect,
				 std::function<void()> initCallback,
					std::function<void()> updateCallback,
					std::function<void()> drawCallback,
					u32 initFlags = 0 );

	// TODO Move all loading to AssetManager to get memory status and other stats
	gdl::Image* LoadImage(std::string filename, gdl::TextureFilterModes filterMode);
	gdl::Image* LoadImage(gdl::PNGFile* png, gdl::TextureFilterModes filterMode);
	gdl::Image* LoadImage(const u8* buffer, size_t size, gdl::TextureFilterModes filterMode);

	gdl::Sound* LoadSound(std::string filename);
	gdl::Sound* LoadSound(const u8* buffer, size_t size);
	gdl::Sound* LoadOgg(std::string filename);

	gdl::PNGFile* LoadPNG(std::string filename);

	gdl::Font* LoadFont(std::string filename, short characterWidth, short characterHeight, char firstCharacter);
	gdl::Font* LoadFontCustom(std::string filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow);
	gdl::Font* LoadFontCustom(std::string filename, short characterWidth, short characterHeight, short charactersPerRow, std::string characters);
	gdl::Font* LoadFont(const u8* buffer, size_t size, short characterWidth, short characterHeight, char firstCharacter);

	void DoProgramExit();

	WiiController* GetController(int controllerNumber);

	u16 GetScreenWidth();
	u16 GetScreenHeight();
	float GetAspectRatio();
	float GetElapsedSeconds();
	float GetDeltaTime();
};
