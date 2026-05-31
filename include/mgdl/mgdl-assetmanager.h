#pragma once

#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-scene.h>

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
	struct DynamicArray* m_sceneAssets = nullptr;
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

struct SceneAsset
{
	Scene* data;
	zstr filename;
};
typedef struct SceneAsset SceneAsset;

DYNAMIC_ARRAY(SceneAsset)


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Initializes the asset manager and reservers space for assets. Loads the default assets. Must be called before loading anything.
	 */
void AssetManager_Init();

/**
 * @brief Loads a texture from file. Will not load again if a texture with same name is already loaded.
 * @param filename Path to the texture relative to the executable
 * @returns Handle to the loaded texture
 */
TextureHandle AssetManager_LoadTexture(const char* filename);

/**
 * @brief Returns the texture asset associated with the given handle.
 * @param handle Handle to the texture. If handle is invalid, the default texture is returned.
 * @returns Pointer to the texture.
 */
Texture* AssetManager_GetTexture(TextureHandle handle);

ImageHandle AssetManager_LoadPNG(const char* filename);

SoundHandle AssetManager_LoadSound(const char* filename, SoundFileType fileType);
Sound* AssetManager_GetSound(SoundHandle handle);

PaletteHandle AssetManager_LoadPalette(const char* filename);
PaletteHandle AssetManager_LoadDefaultPalette();
PaletteHandle AssetManager_LoadDebugPalette();
Palette* AssetManager_GetPalette(PaletteHandle handle);
bool AssetManager_HasPalette(const char* filename);

SceneHandle AssetManager_LoadScene(const char* filename);
Scene* AssetManager_GetScene(SceneHandle handle);

TextureAsset AssetManager_CreateTextureAsset(Texture* data, const char* filename);
SoundAsset AssetManager_CreateSoundAsset(Sound* data, const char* filename);
ImageAsset AssetManager_CreateImageAsset(PNGFile* data, const char* filename);
PaletteAsset AssetManager_CreatePaletteAsset(Palette* data, const char* filename);
SceneAsset AssetManager_CreateSceneAsset(Scene* data, const char* filename);


void AssetManager_PrintLoadedTextures();

void AssetManager_UnloadAll();


#ifdef __cplusplus
}
#endif
