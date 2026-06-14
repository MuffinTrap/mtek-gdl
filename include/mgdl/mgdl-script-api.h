#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-color.h>

// Everything that takes a string parameter needs to be
// a C++ polymorphic function
PaletteHandle mgdl_LoadPalette(const zstr& image);
PaletteHandle mgdl_LoadPalette(const char* image);
TextureHandle mgdl_LoadTexture(const zstr& filename);
TextureHandle mgdl_LoadTexture(const char* filename);
SoundHandle mgdl_LoadSound(const zstr& filename);
SoundHandle mgdl_LoadSound(const char* filename);
SceneHandle mgdl_LoadScene(const zstr& filename);
ModelHandle mgdl_LoadModel(const char* filename);
ModelHandle mgdl_LoadModel(const zstr& filename);

void mgdl_DrawText(const char* text, float x, float y, float fontSize, color32 color);
void mgdl_DrawTextEx(TextureHandle font, const char* text, float x, float y,  float fontSize, color32 color);
void mgdl_DrawTextVEx(TextureHandle font, const char* text, const Vector2 & topleft,  float fontSize, color32 color);

void mgdl_DrawText(const zstr& text, float x, float y, float fontSize, color32 color);
void mgdl_DrawTextV(const zstr& text, const Vector2& topleft, float fontSize, color32 color);
void mgdl_DrawTextEx(TextureHandle font, const zstr& text, float x, float y,  float fontSize, color32 color);
void mgdl_DrawTextVEx(TextureHandle font, const zstr& text, const Vector2 & topleft,  float fontSize, color32 color);

void mgdl_DrawTextureV(TextureHandle texture, const Vector2& topLeft);
void mgdl_DrawSceneV(SceneHandle scene, const Vector3& position, float scale, color32 color);

void mgdl_DrawModelV(ModelHandle handle, const Vector3& position, float scale, color32 color);
void mgdl_DrawModelRotatedV(ModelHandle handle, const Vector3& position, const Vector3& anglesDeg, float scale, color32 color);

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Initializes the scripting api so that the functions can be called
 */
void mgdl_InitScriptApi();


// TEXTURES
// -----------------
/**
 * @brief Set the texture filter used for a texture
 * @param texture Handle of the texture
 * @param mode Filtering mode
 */
void mgdl_SetTextureFilter(TextureHandle texture, TextureFilterModes mode);
/**
 * @brief Set the wrapping mode used for a texture
 * @param texture Handle of the texture
 * @param mode Wrapping mode
 */
void mgdl_SetTextureWrap(TextureHandle texture, TextureWrapModes mode);
/**
 * @brief Creates the uv coordinates to use a texture as a font
 * @param texture Handle of the texture
 * @param characterWidth Width of character in pixels
 * @param characterHeight Height of character in pixels
 * @param firstCharacter First character in the font
 */
void mgdl_CreateFont(TextureHandle texture, s16 characterWidth, s16 characterHeight, char firstCharacter);
/**
 * @brief Creates the uv coordinates to use texture as a sprite atlas
 * @param texture Handle of the texture
 * @param spriteWidth Width of sprite in pixels
 * @param spriteHeight Height of sprite in pixels
 */
void mgdl_CreateSpriteAtlas(TextureHandle texture, s16 spriteWidth, s16 spriteHeight);

// DRAWING
// ----------------------------
void mgdl_DrawTexture(TextureHandle handle, float x, float y);
void mgdl_DrawTextureEx(TextureHandle handle, float x, float y, float rotationDeg, float scale);
void mgdl_DrawSprite(TextureHandle handle, u16 spriteIndex, float x, float y, color32 color);
void mgdl_DrawSpriteEx(TextureHandle handle, u16 spriteIndex, float x, float y, float scale, color32 color);
void mgdl_DrawScene(SceneHandle handle, float x, float y, float z, float scale, color32 color);
void mgdl_DrawModel(ModelHandle handle, float x, float y, float z, float scale, color32 color);
void mgdl_DrawModelRotated(ModelHandle handle, const float x, float y, float z, float xdeg, float ydeg, float zdeg, float scale, color32 color);


// SOUNDS
// -----------------------------
void mgdl_PlaySound(SoundHandle handle);
void mgdl_PauseSound(SoundHandle handle);
void mgdl_StopSound(SoundHandle handle);

// PALETTES
// ---------------------------------------
PaletteHandle mgdl_GetDefaultPalette();
PaletteHandle mgdl_GetDebugPalette();
void mgdl_SetPalette(PaletteHandle palette);
color32 mgdl_GetPaletteColor(PaletteHandle palette, u8 colorIndex);

// INPUT
// ------------------------------------

bool mgdl_IsButtonDown(int controller, WiiButtons button);
bool mgdl_IsButtonPressed(int controller, WiiButtons button);

// ABSTRACT
// //////////////////////////////////
enum mgdlParameter
{
	MGDL_SOUND_LOOPING,
	MGDL_SOUND_PAUSED,
	MGDL_SOUND_ELAPSED_MS,
	MGDL_SOUND_STATUS_ENUM,

	MGDL_TEXTURE_WIDTH,
	MGDL_TEXTURE_HEIGHT,
	MGDL_TEXTURE_ASPECT,
	MGDL_SPRITE_WIDTH,
	MGDL_SPRITE_HEIGHT,
	MGDL_SPRITE_ASPECT,

	MGDL_FONT_WIDTH= MGDL_SPRITE_WIDTH,
	MGDL_FONT_HEIGHT = MGDL_SPRITE_HEIGHT,
	MGDL_FONT_ASPECT = MGDL_SPRITE_ASPECT
};
bool mgdl_GetBool(Handle handle, mgdlParameter parameter);
void mgdl_SetBool(Handle handle, mgdlParameter parameter, bool value);

float mgdl_GetFloat(Handle handle, mgdlParameter parameter);
void mgdl_SetFloat(Handle handle, mgdlParameter parameter, float value);

int mgdl_GetInt(Handle handle, mgdlParameter parameter);
void mgdl_SetInt(Handle handle, mgdlParameter parameter, int value);

#ifdef __cplusplus
}
#endif

bool mgdl_GetVector2(Handle handle, mgdlParameter parameter, Vector2& vOut);
void mgdl_SetVector2(Handle handle, mgdlParameter parameter, const Vector2& value);
