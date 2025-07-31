#pragma once

/**
 * @file mgdl-font.h
 * @brief Class for loading fonts and drawing text with them.
 */

#include "mgdl-types.h"
#include "mgdl-color.h"
#include "mgdl-texture.h"

/**
 * @brief Represents a font that can be used to draw text.
 */
struct Font
{
	short characterWidth; /**< Character width in pixels.*/
	short characterHeight; /**< Character height in pixels.*/


	Texture* _fontTexture;

	vec2 *_tList;
	float _uvWidth;
	float _uvHeight;
	char _firstIndex; /**< Needed to remember the first index to calculate offsets into vertex and uv arrays */
	short _characterCount;

	float _spacingX;
	float _spacingY;
	float _aspect;

};
typedef struct Font Font;

#ifdef __cplusplus
extern "C"
{
#endif

Font* Font_Create(void);

/**
 * @brief Sets the spacing of the font for the next Print call.
 *
 * @param x Spacing between letters horizontally.
 * @param y Spacing between rows vertically.
 */
void Font_SetSpacingOnce(Font* font, float x, float y);

/**
 * @brief Sets the max amount of letters printed for the next Print* call.
 *
 * @param limit Maximum amount of letters drawn;
 */
void Font_SetLineLimitOnce(short limit);

/**
 * @brief Loads a font from an image.
 *
 * The image must be PNG image. Grayscale or color. The amount of characters on every line is expected to be image width / character width. The characters should fill the image evenly.
 *
 * @param fontTexture Texture object.
 * @param charw Width of a character in pixels.
 * @param charw Height of a character in pixels.
 * @param firstCharacter The first character in the image.
 *
 * @return Loaded font or nullptr if failed to load.
 */
Font* Font_Load(Texture* fontTexture, short charw, short charh, char firstCharacter);

/**
 * @brief Loads a font from an image.
 *
 * The image must be PNG image. Grayscale or color. The characters should start from the top left corner, but don't need to fill the image horizontally.
 *
 * @param filename Name of the image file.
 * @param charw Width of a character in pixels.
 * @param charw Height of a character in pixels.
 * @param firstCharacter The first character in the image.
 * @param charactersPerRow How many characters are on a row.
 *
 * @return True if the loading succeeded.
 */
Font* Font_LoadPadded(Texture* fontTexture, short charw, short charh, char firstCharacter, short charactersPerRow);

/**
 * @brief Loads a font from an image.
 *
 * The image must be PNG image. Grayscale or color. The characters should start from the top left corner, but don't need to fill the image horizontally.
 *
 * @param filename Name of the image file.
 * @param charw Width of a character in pixels.
 * @param charw Height of a character in pixels.
 * @param firstCharacter The first character in the image.
 * @param charactersPerRow How many characters are on a row.
 * @param characters The characters to be loaded. Other characters are skipped.
 *
 * @return True if the loading succeeded.
 */
Font* Font_LoadSelective(Texture* fontTexture, short charw, short charh, short charactersPerRow, const char* characters );

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * @param color Color of the text. Font color is multiplied by this.
 * @param x Upper left corner x of first letter.
 * @param y Upper left corner y of first letter.
 * @param textHeight Height of the text.
 * @param format Text, has to containg formatting markers.
 * @param __VA_ARGS__ Values to the formatting markers.
 */
void Font_Printf(Font* font, Color4f* color, float x, float y, float textHeight, const char* format, ... );

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * @param color Color of the text. Font color is multiplied by this.
 * @param x Upper left corner x of first letter.
 * @param y Upper left corner y of first letter.
 * @param textHeight Height of the text.
 * @param text Text to be drawn.
 */
void Font_Print(Font* font, Color4f* color, float x, float y, float textHeight, const char* text);


/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * @param color Color of the text. Font color is multiplied by this.
 * @param x Upper left corner x of first letter.
 * @param y Upper left corner y of first letter.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param format Text, has to containg formatting markers.
 * @param __VA_ARGS__ Values to the formatting markers.
 */
void Font_PrintfAligned(Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* format, ... );

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 *
 * @param color Color of the text. Font color is multiplied by this.
 * @param x Upper left corner x of first letter.
 * @param y Upper left corner y of first letter.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param text Text to be drawn.
 */
void Font_PrintAligned(Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* text);

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * Starts drawing from origo.
 *
 * @param color Color of the text. Font color is multiplied by this.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param format Text, has to containg formatting markers.
 * @param __VA_ARGS__ Values to the formatting markers.
 */
void Font_PrintfOrigo(Font* font, Color4f* color, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* format, ... );

/**
 * @brief Draws text either in 2D or 3D.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * Starts drawing from origo.
 *
 * @param color Color of the text. Font color is multiplied by this.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param text Text to be drawn.
 */
void Font_PrintOrigo(Font* font, Color4f* color, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* text);

/**
 * @brief Draws an icon, only compatible with the debug font.
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * Starts drawing from origo.
 *
 * @param color Color of the text. Font color is multiplied by this.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param glybh The symbol/icon/glyph to be drawn.
 */
void Font_Icon(Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, IconSymbol glyph);

/**
 * @brief Draws an icon rotated
 * @note Only combatible with the Default Font. Other fonts may support the icons too but they are not
 * a standard
 *
 * X, Y and text height are in units when drawing in 3D and in pixels when drawing in 2D.
 * Starts drawing from origo.
 *
 * @param color Color of the text. Font color is multiplied by this.
 * @param textHeight Height of the text.
 * @param alignmentX Alignment of text on the horizontal axis.
 * @param alignmentY Alignment of text on the vertical axis.
 * @param rotation How many clockwise 90 degree turns to do
 * @param glybh The symbol/icon/glyph to be drawn.
 */
void Font_IconRotated(Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, u8 rotation, IconSymbol glyph);

/**
 * @brief Get the texture coordinate corners of a letter
 * @param font Font to read coordinates from
 * @param letter The letter
 * @return The rectangle for uv coordinates.
 */
RectF Font_GetUVRect(Font* font, char letter);

/**
 * @brief Get the texture coordinate corners of an IconSymbol
 * @note Only combatible with the Default Font. Other fonts may support the icons too but they are not
 * @param font Font to read coordinates from
 * @param glyph The icon
 * @return The rectangle for uv coordinates.
 */
RectF Font_GetUVRectIcon(Font* font, IconSymbol glyph);

void _Font_Bind(Font* font, short charw, short charh, char firstCharacter);
void _Font_BindPadded(Font* font, short charw, short charh, char firstCharacter, short charactersPerRow);
void _Font_BindSelective(Font* font, short charw, short charh, const char* characters, short charactersPerRow);
void _Font_CreateTextureCoordList(Font* font, short rows, short charactersPerRow, short texW, short texH);
void _Font_CreateTextureCoordListSelective(Font* font, short rows, short charactersPerRow, short texW, short texH, const char* characters);

void _Font_CreateCoordinatesForGlyph(Font* font, u32 textureIndex, short cx, short cy, short texW, short texH);
vec2 _Font_GetTextureCoordinate(Font* font, char character);
vec2 _Font_GetTextureCoordinateGlyph(Font* font, IconSymbol glyph);

#ifdef __cplusplus
}
#endif
