#pragma once

#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>

struct AssetManager
{
	u32 _memoryInUse;
};


void AssetManager_Init(AssetManager* manager);
void AssetManager_LoadFont(AssetManager* manager, Font* font);
void AssetManager_LoadImage(AssetManager* manager, Image* image);
void AssetManager_LoadPNG(AssetManager* manager, PNGFile* png);
void AssetManager_LoadSound(AssetManager* manager, Sound* snd);
