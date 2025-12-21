#pragma once

#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-music.h>

/**
 * @file mgdl-assetmanager.h
 * @ingroup asset
 * @brief Keeping track of loaded assets and memory use
*/

/**
 * @brief Contains info about loaded assets and memory use
 */
struct AssetManager
{
	sizetype _memoryInUse;
};
typedef struct AssetManager AssetManager;

#ifdef __cplusplus
extern "C"
{
#endif

void AssetManager_Init(AssetManager* manager);
void AssetManager_LoadFont(AssetManager* manager, Font* font);
void AssetManager_LoadTexture(AssetManager* manager, Texture* image);
void AssetManager_LoadPNG(AssetManager* manager, PNGFile* png);
void AssetManager_LoadSound(AssetManager* manager, Sound* snd);


#ifdef __cplusplus
}
#endif
