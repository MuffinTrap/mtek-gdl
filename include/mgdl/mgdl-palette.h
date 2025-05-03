#pragma once

#include "mgdl-types.h"

// A palette that can be filled with custom colors

struct Palette
{
	u32* _colors; // TODO save as vec3
	u8 size;
};

extern "C"
{
	Palette* Palette_GetDefault();
	Palette* Palette_Create(u32* colorsArray, u8 size);
	Palette* Palette_CreateEmpty(u8 size);
	void Palette_SetColor(Palette* palette, u8 index, u32 color);

	u32 Palette_GetColor(Palette* palette, u8 index);
}
