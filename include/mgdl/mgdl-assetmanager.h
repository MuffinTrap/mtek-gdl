#pragma once

#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-palette.h>

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
	sizetype m_memoryInUse = 0;
	struct DynamicArray* m_textureAssets = nullptr;
	struct DynamicArray* m_soundAssets = nullptr;
	struct DynamicArray* m_imageAssets = nullptr;
	struct DynamicArray* m_paletteAssets = nullptr;
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

struct PaletteAsset
{
	Palette* data;
	zstr filename;
};
typedef struct PaletteAsset PaletteAsset;

DYNAMIC_ARRAY(PaletteAsset)


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
PaletteHandle AssetManager_LoadPalette(const char* filename);
PaletteHandle AssetManager_LoadDefaultPalette();
PaletteHandle AssetManager_LoadDebugPalette();
Palette* AssetManager_GetPalette(PaletteHandle handle);
bool AssetManager_HasPalette(const char* filename);

TextureAsset AssetManager_CreateTextureAsset(Texture* data, const char* filename);
SoundAsset AssetManager_CreateSoundAsset(Sound* data, const char* filename);
ImageAsset AssetManager_CreateImageAsset(PNGFile* data, const char* filename);
PaletteAsset AssetManager_CreatePaletteAsset(Palette* data, const char* filename);


void AssetManager_PrintLoadedTextures();

void AssetManager_UnloadAll();


#ifdef __cplusplus
}
#endif
