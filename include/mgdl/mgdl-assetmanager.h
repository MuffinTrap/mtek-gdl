#pragma once

#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-model.h>

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
	ObjectArray* m_textureAssets = nullptr;
	ObjectArray* m_soundAssets = nullptr;
	ObjectArray* m_imageAssets = nullptr;
	ObjectArray* m_paletteAssets = nullptr;
	ObjectArray* m_sceneAssets = nullptr;
	ObjectArray* m_modelAssets = nullptr;
};
typedef struct AssetManager AssetManager;

struct TextureAsset
{
	Texture* data;
	zstr filename;
};
typedef struct TextureAsset TextureAsset;

OBJECT_ARRAY_DECLARE(TextureAsset)

struct SoundAsset
{
	Sound* data;
	zstr filename;
};
typedef struct SoundAsset SoundAsset;

OBJECT_ARRAY_DECLARE(SoundAsset)

struct ImageAsset
{
	PNGFile* data;
	zstr filename;
};
typedef struct ImageAsset ImageAsset;

OBJECT_ARRAY_DECLARE(ImageAsset)

struct PaletteAsset
{
	Palette* data;
	zstr filename;
};
typedef struct PaletteAsset PaletteAsset;

OBJECT_ARRAY_DECLARE(PaletteAsset)

struct SceneAsset
{
	Scene* data;
	zstr filename;
};
typedef struct SceneAsset SceneAsset;

OBJECT_ARRAY_DECLARE(SceneAsset)

struct ModelAsset
{
	Model* data;
	zstr filename;
};
typedef struct ModelAsset ModelAsset;

OBJECT_ARRAY_DECLARE(ModelAsset)


#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Initializes the asset manager and reservers space for assets. Loads the default assets. Must be called before loading anything.
	 */
void AssetManager_Init();

/**
 * @brief Loads a texture from png file. Will not load again if a texture with same name is already loaded.
 * @param filename Path to the png file relative to the executable
 * @returns Handle to the loaded texture
 */
TextureHandle AssetManager_LoadTexture(const char* filename, bool generateMipMaps);

/**
 * @brief Returns the texture asset associated with the given handle.
 * @param handle Handle to the texture. If handle is invalid, the default texture is returned.
 * @returns Pointer to the texture.
 */
Texture* AssetManager_GetTexture(TextureHandle handle);

/**
 * @brief Loads image data from file. Will not load again if an image with same name is already loaded.
 * @details Image data can be used to create heightmaps and such.
 * @param filename Path to the png file relative to the executable
 * @returns Handle to the loaded image
 */
ImageHandle AssetManager_LoadPNG(const char* filename);

/**
 * @brief Loads sound data from file. Will not load again if sound from that file is already loaded.
 * @param filename Path to the audio file relative to the executable
 * @param fileType The type of the file.
 * @returns Handle to the loaded sound.
 */
SoundHandle AssetManager_LoadSound(const char* filename, SoundFileType fileType);

/**
 * @brief Returns the sound asset associated with the given handle.
 * @param handle Handle to the sound. If handle is invalid, a nullptr is returned.
 * @returns Pointer to the sound or nullptr
 */
Sound* AssetManager_GetSound(SoundHandle handle);

/**
 * @brief Loads a palette from png image. Every unique color becomes a palette entry.
 * @note Duplicate colors take up extra memory. Use an image where all pixels are unique.
 * @param filename Filename of the png file
 * @returns Handle to the loaded palette.
 */
PaletteHandle AssetManager_LoadPalette(const char* filename);

/**
 * @brief Loads the default Blessing palette. Can be called multiple times.
 * @returns Handle to the default palette
 */
PaletteHandle AssetManager_LoadDefaultPalette();
/**
 * @brief Loads the debug palette, with basic colors. Can be called multiple times.
 * @returns Handle to the Debug palette.
 */
PaletteHandle AssetManager_LoadDebugPalette();

/**
 * @brief Returns the Palette associated with the given handle
 * @param handle Palette handle
 * @returns Palette pointer or Default palette if handle was invalid
 */
Palette* AssetManager_GetPalette(PaletteHandle handle);

/**
 * @brief Tells if a palette with given name is loaded
 * @param paletteName Name of the palette. If palette was loaded from file, the name is the filename.
 * @returns True if the palette is loaded
 */
bool AssetManager_HasPalette(const char* paletteName);

/**
 * @brief Loads a scene from fbx file
 * @param filename Name of the fbx file
 * @returns Handle to the loaded scene
 */
SceneHandle AssetManager_LoadScene(const char* filename);
/**
 * @brief Returns Scene pointer
 * @param handle Handle of the scene
 * @returns Pointer to the scene or nullptr if handle was invalid
 */
Scene* AssetManager_GetScene(SceneHandle handle);

/**
 * @brief Loads the first model from fbx file
 * @param filename Name of the fbx file
 * @returns Handle to the loaded model
 */
ModelHandle AssetManager_LoadModel(const char* filename);

/**
 * @brief Returns a pointer to Model
 * @param handle Model Handle
 * @returns Pointer to model or nullptr if handle was invalid
 */
Model* AssetManager_GetModel(ModelHandle handle);

// Private functions
TextureAsset AssetManager_CreateTextureAsset(Texture* data, const char* filename);
SoundAsset AssetManager_CreateSoundAsset(Sound* data, const char* filename);
ImageAsset AssetManager_CreateImageAsset(PNGFile* data, const char* filename);
PaletteAsset AssetManager_CreatePaletteAsset(Palette* data, const char* filename);
SceneAsset AssetManager_CreateSceneAsset(Scene* data, const char* filename);
ModelAsset AssetManager_CreateModelAsset(Model* data, const char* filename);


void AssetManager_PrintLoadedTextures();

/**
 * @brief Unloads all assets loaded by asset manager
 */
void AssetManager_UnloadAll(void);


#ifdef __cplusplus
}
#endif
