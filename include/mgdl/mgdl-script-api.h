#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-controller.h>

// Everything that takes a string parameter needs to be
// C++ polymorphic
PaletteHandle mgdl_LoadPalette(const zstr& image);
PaletteHandle mgdl_LoadPalette(const char* image);
TextureHandle mgdl_LoadTexture(const zstr& filename);
TextureHandle mgdl_LoadTexture(const char* filename);
SoundHandle mgdl_LoadSound(const zstr& filename);
SoundHandle mgdl_LoadSound(const char* filename);

#ifdef __cplusplus
extern "C" {
#endif

void mgdl_InitScriptApi();

// TEXTURES
// -----------------
void mgdl_SetTextureFilter(TextureHandle texture, TextureFilterModes mode);
void mgdl_SetTextureWrap(TextureHandle texture, TextureWrapModes mode);

// DRAWING
// ----------------------------
void mgdl_DrawRectangle(s16 x, s16 y, s16 w, s16 h, u8 paletteIndex);
void mgdl_DrawTexture(TextureHandle handle, s16 x, s16 y);
void mgdl_DrawTextureV(TextureHandle handle, vec2 position);

// SOUNDS
// -----------------------------
void mgdl_PlaySound(SoundHandle handle);

// PALETTES
// ---------------------------------------
PaletteHandle mgdl_GetDefaultPalette();
PaletteHandle mgdl_GetDebugPalette();
void mgdl_SetPalette(PaletteHandle palette);

// INPUT
// ------------------------------------

bool mgdl_IsButtonDown(int controller, WiiButtons button);
bool mgdl_IsButtonPressed(int controller, WiiButtons button);

#ifdef __cplusplus
}
#endif
