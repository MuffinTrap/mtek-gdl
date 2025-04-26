#pragma once

/**
 * @file mgdl-font.h
 * @brief Class for loading fonts and drawing text with them.
 */

#include "mgdl-types.h"
#include "mgdl-image.h"
#include <string>

/**
 * @brief Represents a font that can be used to draw text.
 */
struct Font
{
	short cw; /**< Character width in pixels.*/
	short ch; /**< Character height in pixels.*/


	Image* _fontImage;

	vec2		*_tList;
	float _uvWidth;
	float _uvHeight;
	char _firstIndex; /**< Needed to remember the first index to calculate offsets into vertex and uv arrays */

	float _spacingX = 0.0f;
	float _spacingY = 0.0f;
	float _aspect = 1.0f;

};
/**
 * @brief Sets the spacing of the font for the next Print call.
 *
 * @param x Spacing between letters horizontally.
 * @param y Spacing between rows vertically.
 */
void Font_SetSpacingOnce(Font* font, float x, float y);

/**
 * @brief Loads a font from an image.
 *
 * The image must be PNG image. Grayscale or color. The amount of characters on every line is expected to be image width / character width. The characters should fill the image evenly.
 *
 * @param fontImage Image object.
 * @param charw Width of a character in pixels.
 * @param charw Height of a character in pixels.
 * @param firstCharacter The first character in the image.
 *
 * @return Loaded font or nullptr if failed to load.
 */
Font* Font_Load(Image* fontImage, short charw, short charh, char firstCharacter);

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
Font* Font_LoadPadded(Image* fontImage, short charw, short charh, char firstCharacter, short charactersPerRow);

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
Font* Font_LoadSelective(Image* fontImage, short charw, short charh, short charactersPerRow, std::string characters );

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
void Font_Printf(Font* font, gdl::rgba8 color, float x, float y, float textHeight, const char* format, ... );

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
void Font_Print(Font* font, gdl::rgba8 color, float x, float y, float textHeight, const char* text);


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
void Font_PrintfAligned(Font* font, gdl::rgba8 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );

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
void Font_PrintAligned(Font* font, gdl::rgba8 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);

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
void Font_PrintfOrigo(Font* font, gdl::rgba8 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );

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
void Font_PrintOrigo(Font* font, gdl::rgba8 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);

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
void Font_Icon(Font* font, u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, gdl::IconSymbol glyph);

void _Font_Bind(Font* font, short charw, short charh, char firstCharacter);
void _Font_BindPadded(Font* font, short charw, short charh, char firstCharacter, short charactersPerRow);
void _Font_BindSelective(Font* font, short charw, short charh, std::string characters, short charactersPerRow);
void _Font_CreateTextureCoordList(Font* font, short rows, short charactersPerRow, short texW, short texH);
void _Font_CreateTextureCoordListSelective(Font* font, short rows, short charactersPerRow, short texW, short texH, std::string characters);

void _Font_CreateCoordinatesForGlyph(Font* font, u32 textureIndex, short cx, short cy, short texW, short texH);
vec2 _Font_GetTextureCoordinate(Font* font, char character);
vec2 _Font_GetTextureCoordinateGlyph(Font* font, gdl::IconSymbol glyph);
