#pragma once

#include <mgdl/mgdl-font.h>

struct AssetManager
{
	u32 _memoryInUse;
};


void AssetManager_Init(AssetManager* manager);
void AssetManager_LoadFont(AssetManager* manager, Font* font);
void AssetManager_LoadImage(AssetManager* manager, gdl::Image* image);
