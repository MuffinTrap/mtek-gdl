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
	color32* m_colors;
	u8 m_size;
};
typedef struct Palette Palette;

#ifdef __cplusplus
extern "C"
{
#endif
	Palette* Palette_GetDefault(void);
	Palette* Palette_GetDebug(void);

	Palette* Palette_Create(u32* colorsArray, u8 size);
	Palette* Palette_CreateEmpty(u8 size);

	Palette* Palette_FromPNG(const char* filename);
	void Palette_SetColor(Palette* palette, u8 index, color32 color);

	color32 Palette_GetColor(Palette* palette, u8 index);

	u8 Palette_GetColorAmount(Palette* palette);

	void Palette_Delete(Palette* palette);
#ifdef __cplusplus
}
#endif
