#pragma once

#include "mgdl-types.h"
#include "mgdl-color.h"

/**
 * @file mgdl-palette.h
 * @ingroup draw2d draw3d
 * @brief A palette that can be filled with custom colors
 */

struct Palette
{
	Color4f* _colors;
	u8 size;
};
typedef struct Palette Palette;

#ifdef __cplusplus
extern "C"
{
#endif
	Palette* Palette_GetDefault(void);

	Palette* Palette_Create(u32* colorsArray, u8 size);
	Palette* Palette_Create4f(u32* colorsArray, u8 size);
	Palette* Palette_CreateEmpty(u8 size);

	//TODO Palette* Palette_FromTexture(const char* filename);
	void Palette_SetColor(Palette* palette, u8 index, u32 color);
	void Palette_SetColor4f(Palette* palette, u8 index, Color4f color);

	u32 Palette_GetColor(Palette* palette, u8 index);
	Color4f Palette_GetColor4f(Palette* palette, u8 index);

	void Palette_Delete(Palette* palette);
#ifdef __cplusplus
}
#endif
