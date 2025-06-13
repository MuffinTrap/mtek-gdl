#pragma once

#include "mgdl-types.h"

// A palette that can be filled with custom colors

struct Palette
{
	u32* _colors; // TODO save as vec3
	u8 size;
};
typedef struct Palette Palette;

#ifdef __cplusplus
extern "C"
{
#endif
	Palette* Palette_GetDefault(void);
	Palette* Palette_Create(u32* colorsArray, u8 size);
	Palette* Palette_CreateEmpty(u8 size);
	//TODO Palette* Palette_FromImage(const char* filename);
	void Palette_SetColor(Palette* palette, u8 index, u32 color);

	u32 Palette_GetColor(Palette* palette, u8 index);
	Color4f Palette_GetColor4f(Palette* palette, u8 index);
#ifdef __cplusplus
}
#endif
