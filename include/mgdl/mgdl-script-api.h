#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-color.h>

// Everything that takes a string parameter needs to be
// a C++ polymorphic function
// LOGGING
/////////

void mgdl_LogText(const zstr& text);
void mgdl_LogTextInt(const zstr& text, int number);
void mgdl_LogTextFloat(const zstr& text, float number);

PaletteHandle mgdl_LoadPalette(const zstr& image);
PaletteHandle mgdl_LoadPalette(const char* image);
TextureHandle mgdl_LoadTexture(const zstr& filename, bool generateMipMaps);
TextureHandle mgdl_LoadTexture(const char* filename, bool generateMipMaps);
SoundHandle mgdl_LoadSound(const zstr& filename);
SoundHandle mgdl_LoadSound(const char* filename);
SceneHandle mgdl_LoadScene(const zstr& filename);
ModelHandle mgdl_LoadModel(const char* filename);
ModelHandle mgdl_LoadModel(const zstr& filename);
/**
 * @brief Draws text using the default font.
 *
 * @param x Upper left x of first letter.
 * @param y Upper left y of first letter.
 * @param text Text to be drawn.
 * @param color Color of the text.
 */
void mgdl_DrawText(const char* text, float x, float y, float fontSize, color32 color);
/**
 * @brief Draws text using the default font.
 *
 * @param position Upper left corner of first letter
 * @param text Text to be drawn.
 * @param color Color of the text.
 */
void mgdl_DrawTextV(const char* text, const Vector2& topleft, float fontSize, color32 color);
void mgdl_DrawTextEx(TextureHandle font, const char* text, float x, float y,  float fontSize, color32 color);
void mgdl_DrawTextVEx(TextureHandle font, const char* text, const Vector2 & topleft,  float fontSize, color32 color);

void mgdl_DrawText(const zstr& text, float x, float y, float fontSize, color32 color);
void mgdl_DrawTextV(const zstr& text, const Vector2& topleft, float fontSize, color32 color);
void mgdl_DrawTextEx(TextureHandle font, const zstr& text, float x, float y,  float fontSize, color32 color);
void mgdl_DrawTextVEx(TextureHandle font, const zstr& text, const Vector2 & topleft,  float fontSize, color32 color);

void mgdl_DrawTextInt(const zstr& text, int number, float x, float y, float fontSize, color32 color);
void mgdl_DrawTextFloat(const zstr& text, float number, float x, float y, float fontSize, color32 color);
void mgdl_DrawInt( int number, float x, float y, float fontSize, color32 color);
void mgdl_DrawFloat( float number, float x, float y, float fontSize, color32 color);

void mgdl_DrawTextureV(TextureHandle texture, const Vector2& topLeft);
void mgdl_DrawSceneV(SceneHandle scene, const Vector3& position, float scale, color32 color);

void mgdl_DrawModelV(ModelHandle handle, const Vector3& position, float scale, color32 color);
void mgdl_DrawModelRotatedV(ModelHandle handle, const Vector3& position, const Vector3& anglesDeg, float scale, color32 color);

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Draws a box border.
 *
 * @details The box is drawn using GL_LINES so the width is 1 pixel by default
 *
 * @param x Top left corner x in screen units.
 * @param y Top left corner y in screen units.
 * @param w Width
 * @param h Height
 * @param color Color of the border
 */
void mgdl_DrawRectangle(float x, float y, float w, float h, color32 color);
void mgdl_DrawRectangleV(Vector2 topleft, Vector2 size, color32 color);
void mgdl_DrawRectangleLines(float x, float y, float w, float h, color32 color);
void mgdl_DrawRectangleLinesEx(RectF rec, float lineThickness, color32 color);

/**
 * @brief Draws a line.
 *
 * @param x Start point x
 * @param y Start point y
 * @param x2 End point x
 * @param y2 End point y
 * @param color Color of the line.
 */
void mgdl_DrawLine( float x, float y, float x2, float y2, color32 color );
void mgdl_DrawLineGradient( float x1, float y1, float x2, float y2, color32 color1 , color32 color2);

/**
 * @brief Draws a line.
 *
 * @param start Start point
 * @param end Start point
 * @param color Color of the line.
 */
void mgdl_DrawLineV(Vector2 start, Vector2 end, color32 color );
void mgdl_DrawLineEx(Vector2 start, Vector2 end, float lineThickness, color32 color );


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
void mgdl_SetTextureFilterMag(TextureHandle texture, TextureFilterModes mode);

/**
	* @brief Sets the filtering mode of the texture when minifying
	* @note If the texture has mipmaps, the Nearest and Linear modes do not use. Be sure to use a mipmap filter. (You will get a warning if you don't). Warning also comes from using mipmap filter on texture without mipmaps.
	* @param texture Handle of the texture
	* @param mode The mode to use
	*/
void mgdl_SetTextureFilterMin(TextureHandle texture, TextureFilterModes mode);
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
int mgdl_GetTextureWidth(TextureHandle handle);
int mgdl_GetTextureHeight(TextureHandle handle);
int mgdl_GetSpriteWidth(TextureHandle handle);
int mgdl_GetSpriteHeight(TextureHandle handle);
float mgdl_GetSpriteAspect(TextureHandle handle);


// SOUNDS
// -----------------------------
void mgdl_PlaySound(SoundHandle handle);
void mgdl_PauseSound(SoundHandle handle);
void mgdl_StopSound(SoundHandle handle);

mgdlAudioStateEnum mgdl_GetSoundStatus(SoundHandle handle);
int mgdl_GetSoundElapsedMs(SoundHandle handle);

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
Vector2 mgdl_GetJoystick(int controller, WiiJoystick joystick);
float mgdl_GetJoystickX(int controller, WiiJoystick joystick);
float mgdl_GetJoystickY(int controller, WiiJoystick joystick);


#ifdef __cplusplus
}
#endif
