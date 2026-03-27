#pragma once

#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-dynamic_array.h>

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
	sizetype m_memoryInUse;
	struct DynamicArray* m_textureAssets;
	struct DynamicArray* m_soundAssets;
	struct DynamicArray* m_imageAssets;
};
typedef struct AssetManager AssetManager;

struct TextureAsset
{
	Texture* data;
	zstr filename;
};
typedef struct TextureAsset TextureAsset;

DYNAMIC_ARRAY(TextureAsset)

struct SoundAsset
{
	Sound* data;
	zstr filename;
};
typedef struct SoundAsset SoundAsset;

DYNAMIC_ARRAY(SoundAsset)

struct ImageAsset
{
	PNGFile* data;
	zstr filename;
};
typedef struct ImageAsset ImageAsset;

DYNAMIC_ARRAY(ImageAsset)


#ifdef __cplusplus
extern "C"
{
#endif

void AssetManager_Init();
TextureHandle AssetManager_LoadTexture(const char* filename);
Texture* AssetManager_GetTexture(TextureHandle handle);
ImageHandle AssetManager_LoadPNG(const char* filename);
SoundHandle AssetManager_LoadSound(const char* filename, SoundFileType fileType);
Sound* AssetManager_GetSound(SoundHandle handle);



#ifdef __cplusplus
}
#endif
