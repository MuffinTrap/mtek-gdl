#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-font.h>
#ifdef GEKKO
#include <mgdl-wii/mgdl-wii-sound.h>
#else
#include <mgdl-pc/mgdl-pc-sound.h>
#endif

#include <functional>
#include <string>

namespace gdl
{
	void InitSystem(gdl::ScreenAspect screenAspect,
								std::function<void()> initCallback,
								std::function<void()> updateCallback,
								std::function<void()> drawCallback,
								u32 initFlags = 0 );

	gdl::Image* LoadImage(std::string filename, gdl::TextureFilterModes filterMode);
	gdl::Image* LoadImage(gdl::PNGFile* png, gdl::TextureFilterModes filterMode);
	gdl::Sound* LoadSound(std::string filename);
	gdl::PNGFile* LoadPNG(std::string filename);
	gdl::Font* LoadFont(std::string filename, short characterWidth, short characterHeight, char firstCharacter);

	void DoProgramExit();

	gdl::WiiController& GetController(int controllerNumber);

	u16 GetScreenWidth();
	u16 GetScreenHeight();
	float GetAspectRatio();
	float GetElapsedSeconds();
	float GetDeltaTime();
};
