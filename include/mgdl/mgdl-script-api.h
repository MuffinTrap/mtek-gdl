#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-color.h>

// Everything that takes a string parameter needs to be
// C++ polymorphic
PaletteHandle mgdl_LoadPalette(const zstr& image);
PaletteHandle mgdl_LoadPalette(const char* image);
TextureHandle mgdl_LoadTexture(const zstr& filename);
TextureHandle mgdl_LoadTexture(const char* filename);
SoundHandle mgdl_LoadSound(const zstr& filename);
SoundHandle mgdl_LoadSound(const char* filename);

void mgdl_DrawText(float x, float y, const zstr& text, color32 color);
void mgdl_DrawTextV2(Vector2 topleft, const zstr& text, color32 color);

#ifdef __cplusplus
extern "C" {
#endif

void mgdl_InitScriptApi();


// TEXTURES
// -----------------
void mgdl_SetTextureFilter(TextureHandle texture, TextureFilterModes mode);
void mgdl_SetTextureWrap(TextureHandle texture, TextureWrapModes mode);
void mgdl_CreateFontUVs(TextureHandle texture, s16 characterWidth, s16 characterHeight, char firstCharacter);

// DRAWING
// ----------------------------
void mgdl_DrawTexture(TextureHandle handle, float x, float y);
void mgdl_DrawTextureV(TextureHandle handle, vec2 position);


// SOUNDS
// -----------------------------
void mgdl_PlaySound(SoundHandle handle);

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

#ifdef __cplusplus
}
#endif
