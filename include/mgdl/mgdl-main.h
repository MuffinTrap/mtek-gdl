#pragma once

#include "mgdl-types.h"

#include <functional>
#include <string>

namespace gdl
{
	// Forward declaration
	class Font;
	class Sound;
	class Image;
	class PNGFile;
	class WiiController;

	void InitSystem(gdl::ScreenAspect screenAspect,
								std::function<void()> initCallback,
								std::function<void()> updateCallback,
								std::function<void()> drawCallback,
								u32 initFlags = 0 );

	gdl::Image* LoadImage(std::string filename, gdl::TextureFilterModes filterMode);
	gdl::Image* LoadImage(gdl::PNGFile* png);
	gdl::Sound* LoadSound(std::string filename);
	gdl::PNGFile* LoadPNG(std::string filename);
	gdl::Font* LoadFont(std::string filename, short characterWidth, short characterHeight, char firstCharacter);

	void ReadController(gdl::WiiController& controllerInOut);

	u16 GetScreenWidth();
	u16 GetScreenHeight();
	float GetAspectRatio();
	float GetElapsedSeconds();
	float GetDeltaTime();
};
