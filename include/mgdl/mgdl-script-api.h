#pragma once
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-controller.h>

#ifdef __cplusplus
extern "C" {
#endif

void mgdl_InitScriptApi();

// TEXTURES
// -----------------
TextureHandle mgdl_LoadTexture(const zstr& filename);
void mgdl_SetTextureFilter(Handle texture, TextureFilterModes mode);
void mgdl_SetTextureWrap(Handle texture, TextureWrapModes mode);

// DRAWING
// ----------------------------
void mgdl_DrawRectangle(s16 x, s16 y, s16 w, s16 h, u8 paletteIndex);
void mgdl_DrawTexture(TextureHandle handle, s16 x, s16 y);
void mgdl_DrawTextureV(TextureHandle handle, vec2 position);

// SOUNDS
// -----------------------------
SoundHandle mgdl_LoadSound(const zstr& filename);
void mgdl_PlaySound(SoundHandle handle);

// PALETTES
// ---------------------------------------
Handle mgdl_LoadPalette(const zstr& image);
Handle mgdl_GetDefaultPalette();
Handle mgdl_GetDebugPalette();
void mgdl_SetPalette(Handle palette);

// INPUT
// ------------------------------------

bool mgdl_IsButtonDown(int controller, WiiButtons button);
bool mgdl_IsButtonPressed(int controller, WiiButtons button);

#ifdef __cplusplus
}
#endif
