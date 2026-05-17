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

	/**
	 * @brief Returns the default mgdl palette: Blessing
	 * @returns Default palette
	 */
	Palette* Palette_GetDefault(void);
	/**
	 * @brief Returns the debug palette with 8 basic colors
	 * @returns Debug palette
	 */
	Palette* Palette_GetDebug(void);

	/**
	 * @brief Creates and returns a palette made of array of colors
	 * @param colorsArray Array containing the colors
	 * @param size Size of the array
	 * @returns The created palette
	 */
	Palette* Palette_Create(u32* colorsArray, u8 size);
	/**
	 * @brief Creates and returns an empty palette
	 * @param size Size of the array
	 * @returns The created palette
	 */
	Palette* Palette_CreateEmpty(u8 size);

	/**
	 * @brief Creates and returns a palette created from an image.
	 * @note The image must have a height of 1 pixel and one color for each pixel
	 * @param filename The path to the png file
	 * @returns The created palette
	 */
	Palette* Palette_FromPNG(const char* filename);

	/**
	 * @brief Sets a color in a given palette
	 * @param palette The palette
	 * @param index The index for the new color
	 * @param color The color
	 */
	void Palette_SetColor(Palette* palette, u8 index, color32 color);

	/**
	 * @brief Gets a color from a palette
	 * @param palette The palette
	 * @param index The index of the color
	 */
	color32 Palette_GetColor(Palette* palette, u8 index);

	/**
	 * @brief Gets amount of colors in a palette
	 * @param palette The palette
	 * @param index The amount of colors
	 */
	u8 Palette_GetColorAmount(Palette* palette);

	/**
	 * @brief Deletes the palette and frees the memory
	 * @param palette The palette to delete
	 */
	void Palette_Delete(Palette* palette);
#ifdef __cplusplus
}
#endif
