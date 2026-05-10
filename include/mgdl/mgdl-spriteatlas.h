#pragma once

/**
 * @file mgdl-spriteatlas.h
 * @ingroup draw2d draw3d asset
 * @brief Struct for info about fonts and spritesheets and functions for drawing them.
 */

#include "mgdl-types.h"
#include "mgdl-color.h"

struct Texture;

/**
 * @brief Stores where letters or sprites are located within a texture
 */
struct SpriteAtlas
{
	Vector2 *uvList;
	float uvWidth;
	float uvHeight;
	union
	{
	short characterWidth; /**< Character width in pixels.*/
	short spriteWidth;
	};
	union
	{
		short characterHeight; /**< Character height in pixels.*/
		short spriteHeight; /**< Character height in pixels.*/
	};
	char firstIndex; /**< Needed to remember the first index to calculate offsets into vertex and uv arrays */
	union
	{
		short characterCount;
		short spriteCount;
	};
	float aspectRatio;
};
typedef struct SpriteAtlas SpriteAtlas;

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Allocates a new empty SpriteAtlas
	 * @returns Allocated SpriteAtlas struct
	 */
SpriteAtlas* SpriteAtlas_Create(void);

s16 Texture_GetSpriteWidth(Texture* texture);
s16 Texture_GetSpriteHeight(Texture* texture);
s16 Texture_GetCharacterWidth(Texture* texture);
s16 Texture_GetCharacterHeight(Texture* texture);

/**
 * @brief Creates font UVs for given texture
 *
 * The amount of characters/sprites on every line is expected to be image width / character width. The characters should fill the image evenly.
 *
 * @param fontTexture Texture object.
 * @param charw Width of a character in pixels.
 * @param charw Height of a character in pixels.
 * @param firstCharacter The first character in the image.
 *
 */
void SpriteAtlas_MapSimple(Texture* fontTexture, short charw, short charh, char firstCharacter);

/**
 * @brief Creates font UVs for given texture with predefined set of characters.
 * @note Only usable for loading fonts.
 *
 * @details The characters should start from the top left corner and be in the default order
 *
 * @param filename Name of the image file.
 * @param charw Width of a character in pixels.
 * @param charw Height of a character in pixels.
 * @param firstCharacter The first character in the image.
 * @param charactersPerRow How many characters are on a row.
 * @param characters The characters to be loaded. Other characters are skipped.

 */
void SpriteAtlas_MapSelective(Texture* fontTexture, short charw, short charh, const char* characters );

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * @param texture Texture to use for drawing. Is expected to have a valid SpriteAtlas struct.
 * @param color Color of the text. SpriteAtlas color is multiplied by this.
 * @param x Upper left corner x of first letter.
 * @param y Upper left corner y of first letter.
 * @param textHeight Height of the text.
 * @param format Text, has to containg formatting markers.
 * @param __VA_ARGS__ Values to the formatting markers.
 */
void Texture_DrawTextF(Texture* texture, color32 color, float x, float y, float textHeight, const char* format, ... );

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * @param texture Texture to use for drawing. Is expected to have a valid SpriteAtlas struct.
 * @param color Color of the text. SpriteAtlas color is multiplied by this.
 * @param x Upper left corner x of first letter.
 * @param y Upper left corner y of first letter.
 * @param textHeight Height of the text.
 * @param text Text to be drawn.
 */
void Texture_DrawText(Texture* texture, color32 color, float x, float y, float textHeight, const char* text);


/**
 * @brief Draws an icon
 * @note Only compatible with the debug font.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * Starts drawing from origo.
 *
 * @param color Color of the text. SpriteAtlas color is multiplied by this.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param glybh The symbol/icon/glyph to be drawn.
 */
void DefaultFont_DrawIcon(color32 color, float x, float y, float textHeight, IconSymbol glyph);

/**
 * @brief Draws an icon rotated
 * @note Only combatible with the Default SpriteAtlas. Other fonts may support the icons too but they are not
 * a standard
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * Starts drawing from origo.
 *
 * @param color Color of the text. SpriteAtlas color is multiplied by this.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param rotation How many clockwise 90 degree turns to do
 * @param glybh The symbol/icon/glyph to be drawn.
 */
void DefaultFont_DrawIconRotated(color32 color, float x, float y, float textHeight, u8 rotation, IconSymbol glyph);

/**
 * @brief Get the texture coordinate corners of a letter
 * @param font SpriteAtlas to read coordinates from
 * @param letter The letter
 * @return The rectangle for uv coordinates.
 */
RectF SpriteAtlas_GetUVRect(SpriteAtlas* font, char letter);

/**
 * @brief Get the texture coordinate corners of an IconSymbol
 * @note Only combatible with the Default SpriteAtlas. Other fonts may support the icons too but they are not
 * @param font SpriteAtlas to read coordinates from
 * @param glyph The icon
 * @return The rectangle for uv coordinates.
 */
RectF DefaultFont_GetUVRectIcon(IconSymbol glyph);

void SpriteAtlas_Bind(Texture* font, short charw, short charh, char firstCharacter);
void SpriteAtlas_BindSelective(Texture* font, short charw, short charh, const char* characters);
void SpriteAtlas_CreateTextureCoordList(SpriteAtlas* font, short rows, short charactersPerRow, short texW, short texH);
void SpriteAtlas_CreateTextureCoordListSelective(SpriteAtlas* font, short rows, short charactersPerRow, short texW, short texH, const char* characters);

void SpriteAtlas_CreateCoordinatesForGlyph(SpriteAtlas* font, u32 textureIndex, short cx, short cy, short texW, short texH);
Vector2 SpriteAtlas_GetTextureCoordinateChar(SpriteAtlas* font, char character);
Vector2 SpriteAtlas_GetTextureCoordinateSprite(SpriteAtlas* font, short sprite);
Vector2 DefaultFont_GetTextureCoordinateGlyph(IconSymbol glyph);

#ifdef __cplusplus
}
#endif
