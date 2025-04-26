#pragma once

/**
 * @file mgdl-font.h
 * @brief Class for loading fonts and drawing text with them.
 */

#include "mgdl-types.h"
#include "mgdl-image.h"
#include <string>

namespace gdl
{
	/**
	 * @brief Represents a font that can be used to draw text.
	 */
	class Font
	{
	public:
		/**
		 * @brief Loads a font from an image stored in memory buffer.
		 *
		 * This is only useful if the image has been included in the code as a header.
		 *
		 * @param buffer Memory buffer containing the image data.
		 * @param size Size of the buffer in bytes.
		 * @param charw Width of a character in pixels.
		 * @param charw Height of a character in pixels.
		 * @param firstCharacter The first character in the image.
		 *
		 * @return True if the loading succeeded.
		 */
		bool LoadFromBuffer(const u8* buffer, size_t size, short charw, short charh, char firstCharacter);


		/**
		 * @brief Loads a font from an image.
		 *
		 * The image must be PNG image. Grayscale or color. The amount of characters on every line is expected to be image width / character width. The characters should fill the image evenly.
		 *
		 * @param filename Name of the image file.
		 * @param charw Width of a character in pixels.
		 * @param charw Height of a character in pixels.
		 * @param firstCharacter The first character in the image.
		 *
		 * @return True if the loading succeeded.
		 */
		bool LoadFromImage(const char* filename, short charw, short charh, char firstCharacter);

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
		bool LoadFromImage(const char* filename, short charw, short charh, char firstCharacter, short charactersPerRow);

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
		bool LoadFromImage(const char* filename, short charw, short charh, short charactersPerRow, std::string characters );

		/**
		 * @brief Loads a font from an image object.
		 *
		 * The image must be PNG image. Grayscale or color. The characters should start from the top left corner, but don't need to fill the image horizontally.
		 *
		 * @param image Reference to an image object.
		 * @param charw Width of a character in pixels.
		 * @param charw Height of a character in pixels.
		 * @param firstCharacter The first character in the image.
		 *
		 * @return True if the loading succeeded.
		 */
		bool LoadFromImage(gdl::Image& image, short charw, short charh, char firstCharacter);

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
		void Printf(rgba8 color, float x, float y, float textHeight, const char* format, ... );

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
		void Print(rgba8 color, float x, float y, float textHeight, const char* text);


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
		void Printf(rgba8 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );

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
		void Print(rgba8 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);

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
		void Printf(rgba8 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... );

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
		void Print(rgba8 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text);

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
		void Icon(u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, gdl::IconSymbol glyph);

		/**
		 * @brief Sets the spacing of the font for the next Print call.
		 *
		 * @param x Spacing between letters horizontally.
		 * @param y Spacing between rows vertically.
		 */
		void SetSpacingOnce(float x, float y);

		/**
		 * @brief Returns the width of the character in pixels.
		 * @return Width of the character in pixels.
		 */
		short GetCharacterWidth();
		/**
		 * @brief Returns the height of the character in pixels.
		 * @return Height of the character in pixels.
		 */
		short GetCharacterHeight();

	protected: // gdl::SpriteSet derives from this class

		gdl::Image fontImage;

		GLuint textureName;
		short		cw,ch;
		vec2		*tList;
		char		firstIndex; /**< Needed to remember the first index to calculate offsets into vertex and uv arrays */

		float spacingX = 0.0f;
		float spacingY = 0.0f;
		float aspect = 1.0f;

		void Bind(short charw, short charh, char firstCharacter);
		void Bind(short charw, short charh, char firstCharacter, short charactersPerRow);
		void Bind(short charw, short charh, std::string characters, short charactersPerRow);

		void DrawSheet();

		void CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH);
		void CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH, std::string characters);
		void CreateCoordinatesForGlyph(u32 textureIndex, short cx, short cy, short texW, short texH);
		vec2 GetTextureCoordinate(char character, char subIndex);
		vec2 GetTextureCoordinate(gdl::IconSymbol glyph, char subIndex);
	};
};
