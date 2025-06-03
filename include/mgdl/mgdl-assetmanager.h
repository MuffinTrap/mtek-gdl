#pragma once

#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>

/**
 * @file mgdl-assetmanager.h
 * @brief Keeping track of loaded assets and memory use
*/

/**
 * @brief Contains info about loaded assets and memory use
 */
struct AssetManager
{
	sizetype _memoryInUse;
};

extern "C"
{

void AssetManager_Init(AssetManager* manager);
void AssetManager_LoadFont(AssetManager* manager, Font* font);
void AssetManager_LoadImage(AssetManager* manager, Image* image);
void AssetManager_LoadPNG(AssetManager* manager, PNGFile* png);
void AssetManager_LoadSound(AssetManager* manager, Sound* snd);
void AssetManager_LoadOgg(AssetManager* manager, Music* ogg) {}; // TODO
void AssetManager_LoadWav(AssetManager* manager, Music* wav) {}; // TODO

}
