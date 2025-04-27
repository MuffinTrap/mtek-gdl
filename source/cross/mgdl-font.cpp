
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-util.h>
#include <string.h>
#include <stdarg.h>

using namespace gdl;

Font* Font_Load(Image* fontImage, short charw, short charh, char firstCharacter )
{
	Font* font = new Font();
	font->_fontImage = fontImage;
	// TODO Log info printf("\tCreating font/sprite coordinates\n");
	_Font_Bind(font, charw, charh, firstCharacter);
	return font;
}

Font* Font_LoadPadded(Image* fontImage, short charw, short charh, char firstCharacter, short charactersPerRow )
{
	Font* font = new Font();
	font->_fontImage = fontImage;
	// TODO Log info printf("\tCreating font/sprite coordinates\n");
	_Font_BindPadded(font, charw, charh, firstCharacter, charactersPerRow);
	return font;
}

Font* Font_LoadSelective(Image* fontImage, short charw, short charh, short charactersPerRow, std::string characters )
{
	Font* font = new Font();
	font->_fontImage = fontImage;
	// TODO Log info printf("\tCreating font/sprite coordinates\n");
	_Font_BindSelective(font, charw, charh, characters, charactersPerRow);
	return font;
}


void _Font_BindPadded(Font* font,short charw, short charh, char firstCharacter, short charactersPerRow )
{
	const short tw = font->_fontImage->width;
	const short th = font->_fontImage->height;
	short rows = th / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)
	font->_firstIndex = firstCharacter;
	font->cw = charw;
	font->ch = charh;
	font->_uvWidth = (float)charw/(float)tw;
	font->_uvHeight = (float)charh/(float)th;
	font->_aspect = (float)charw/(float)charh;

	_Font_CreateTextureCoordList(font, rows, charactersPerRow, tw, th);

	font->_spacingX = 0.0f;
	font->_spacingY = 0.0f;
}

void _Font_Bind(Font* font, short charw, short charh, char firstCharacter )
{
	short charactersPerRow = font->_fontImage->width/ charw;
	_Font_BindPadded(font, charw, charh, firstCharacter, charactersPerRow);
}

void _Font_BindSelective (Font* font, short charw, short charh, std::string characters, short charactersPerRow )
{
	const short tw = font->_fontImage->width;
	const short th = font->_fontImage->height;
	short rows = th / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)

	font->_firstIndex = characters[0];

	font->cw = charw;
	font->ch = charh;
	font->_uvWidth = (float)charw/(float)tw;
	font->_uvHeight = (float)charh/(float)th;
	font->_aspect = (float)charw/(float)charh;

	_Font_CreateTextureCoordListSelective(font, rows, charactersPerRow, tw, th, characters);

	font->_spacingX = 0.0f;
	font->_spacingY = 0.0f;
}


void Font_Icon (Font* font, u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, gdl::IconSymbol glyph )
{
	GLuint textureName = font->_fontImage->textureId;
	float step = font->_aspect * textHeight;
	float dx = x;
	float dy = y;
	float dz = 0.0f;

	const float uvW = font->_uvWidth;
	const float uvH = font->_uvHeight;

	if (alignmentX == RJustify)
	{
		float width = step;
		dx -= width;
	}
	else if (alignmentX == Centered)
	{
		float width = step;
		dx -= width / 2;
	}
	if (alignmentY == RJustify)
	{
		dy += textHeight;
	}
	else if (alignmentY == Centered)
	{
		dy += textHeight/2.0f;
	}
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);
    // Discard pixels with low alpha

	gdl::RGBA8Floats f = gdl::ColorToFloats(color);
	glBegin(GL_QUADS);
	glColor3f(f.red, f.green, f.blue);

		vec2 tx= _Font_GetTextureCoordinateGlyph(font, glyph); //LOW LEFT!

		// LOW LEFT!
		glTexCoord2f(tx.x, tx.y);
		glVertex3f(dx, dy - textHeight, dz);

		// LOW RIGHT
		glTexCoord2f(tx.x + uvW, tx.y);
		glVertex3f(dx + step, dy - textHeight, dz);

		// TOP RIGHT
		glTexCoord2f(tx.x + uvW, tx.y + uvH);
		glVertex3f(dx + step, dy, dz);

		// TOP LEFT
		glTexCoord2f(tx.x, tx.y + uvH);
		glVertex3f(dx, dy, dz);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

}


void _Font_PrintAligned(Font* font, u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text)
{
	GLuint textureName = font->_fontImage->textureId;
	const float step = font->_aspect * textHeight;

	float dx = x;
	float dy = y;
	float dz = 0.0f;

	const float uvW = font->_uvWidth;
	const float uvH = font->_uvHeight;

	if (alignmentX == RJustify)
	{
		const float width = step * strlen(text);
		dx -= width;
	}
	else if (alignmentX == Centered)
	{
		const float width = step * strlen(text);
		dx -= width / 2;
	}
	const float left = dx;
	if (alignmentY == RJustify)
	{
		dy += textHeight;
	}
	else if (alignmentY == Centered)
	{
		dy += textHeight/2.0f;
	}
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);
    // Discard pixels with low alpha

	gdl::RGBA8Floats f = gdl::ColorToFloats(color);
	glBegin(GL_QUADS);
	glColor3f(f.red, f.green, f.blue);
	for (short c = 0; text[c] != '\0'; c++)
	{
		char character = text[c];
		if (character == '\n')
		{
			dx = left;
			dy -= textHeight + font->_spacingY;
			continue;
		}
		vec2 tx= _Font_GetTextureCoordinate(font, character); //LOW LEFT!


		// LOW LEFT!
		glTexCoord2f(tx.x, tx.y);
		glVertex3f(dx, dy - textHeight, dz);

		// LOW RIGHT
		glTexCoord2f(tx.x + uvW, tx.y);
		glVertex3f(dx + step, dy - textHeight, dz);

		// TOP RIGHT
		glTexCoord2f(tx.x + uvW, tx.y + uvH);
		glVertex3f(dx + step, dy, dz);

		// TOP LEFT
		glTexCoord2f(tx.x, tx.y + uvH);
		glVertex3f(dx, dy, dz);

		dx += step + font->_spacingX;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	font->_spacingX = 0.0f;
	font->_spacingY = 0.0f;
}

void Font_Print(Font* font, u32 color, float x, float y, float textHeight, const char* text)
{
	Font_PrintAligned(font, color, x, y, textHeight, gdl::AlignmentModes::LJustify, gdl::AlignmentModes::LJustify, text);
}

void Font_PrintOrigo(Font* font, u32 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text)
{
	Font_PrintAligned(font, color, 0.0f, 0.0f, textHeight, alignmentX, alignmentY, text);
}


void Font_Printf(Font* font, u32 color, float x, float y, float textHeight, const char* format, ... )
{
	MGDL_PRINTF_TO_BUFFER(format)

	Font_PrintAligned(font, color, x, y, textHeight, gdl::AlignmentModes::LJustify, gdl::AlignmentModes::LJustify, MGDL_GetPrintfBuffer());
}

void Font_PrintfOrigo(Font* font, u32 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... )
{
	MGDL_PRINTF_TO_BUFFER(format)

	Font_PrintAligned(font, color, 0, 0, textHeight, alignmentX, alignmentY, MGDL_GetPrintfBuffer());
}

void Font_PrintfAligned( Font* font, u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... )
{
	// Draw quads

	MGDL_PRINTF_TO_BUFFER(format)

	Font_PrintAligned(font, color, x, y, textHeight, alignmentX, alignmentY, MGDL_GetPrintfBuffer());
}

void Font_SetSpacingOnce (Font* font, float x, float y )
{
	font->_spacingX = x;
	font->_spacingY = y;
}

// Version that only creates texture coordinates for
// the given characters.
// All other characters are the same as the first one

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void _Font_CreateTextureCoordListSelective (Font* font, short rows, short charactersPerRow, short texW, short texH, std::string characters )
{
	gdl_assert_print(font->cw > 0 && font->ch > 0, "Character dimensions not set");
	gdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	gdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	// Need to also create texture coordinates for characters that are not included
	char first = characters[0];
	char last = characters[characters.length()-1];
	short textureArraySize =  (last - first) + 1;
	short toBeFoundAmount = static_cast<short>(characters.length());
	printf("find between %d and %d: %d characters\n", first, last, toBeFoundAmount);


	size_t tListSize = sizeof(vec2)*textureArraySize;
	if (font->_tList == NULL)
	{
		font->_tList = (vec2*)gdl::AllocateAlignedMemory(tListSize);
		gdl_assert_print(font->_tList != nullptr, "Out of memory when allocation font txcord list");
	}

	printf("Creating coordinates: rows %d, cperRow %d, textureW %d, textrureH %d\n", rows, charactersPerRow, texW, texH);

	char currentCharacter = font->_firstIndex;
	gdl_assert_print(currentCharacter >= ' ', "First character must be space or bigger");

	// If the last character in image is not included this crashes unless we keep track of how many should be found
	short textureIndex = 0;
	short found = 0;

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {
			// Coordinates to source image
			// Is this character included
			bool included = characters.find_first_of(currentCharacter) != std::string::npos;
			if (!included)
			{
				// Not included: set texture coords same as first one

				_Font_CreateCoordinatesForGlyph(font, textureIndex, 0, 0, texW, texH);

				//printf("Excluded: %c\n", currentCharacter);
				// Decrement cx so that characters that are not included
				// do not advance coordinates
				cx--;
			}
			else
			{
				found++;
				// printf("Included: %c. Found %d/%d\n", currentCharacter, found, toBeFoundAmount);

				_Font_CreateCoordinatesForGlyph(font, textureIndex, cx, cy, texW, texH);
			}

			// In this version the textureIndex and cx are not the same
			// Because even the characters that are not included have
			// coordinates for them
			textureIndex += 1; // Four coordinates per character

			currentCharacter++;
			if (found == toBeFoundAmount)
			{
				printf("All %d characters found\n", toBeFoundAmount);
				break;
			}

		}
		if (found == toBeFoundAmount)
		{
			break;
		}
	}
	gdl::CacheFlushRange(font->_tList, tListSize);
}

void _Font_CreateCoordinatesForGlyph (Font* font, u32 textureIndex, short cx, short cy, short texW, short texH )
{
	u32 tc = textureIndex;
	// NOTE: These are pixel coordinates
	// where origo is in the lower left coordinate
	// and Y increases upwards

	// But the first glyph is in the top left corner of the
	// image

	// tx and ty are lower left corner of glyph's area
	float tx = font->cw * cx;
	float ty = texH - font->ch - font->ch * cy;

	// NOTE: Stored in the quad drawing order
	// Lower-left
	font->_tList[tc+0].x	= tx/texW;
	font->_tList[tc+0].y	= ty/texH;
}


void _Font_CreateTextureCoordList(Font* font, short rows, short charactersPerRow, short texW, short texH)
{
	gdl_assert_print(font->cw > 0 && font->ch > 0, "Character dimensions not set");
	gdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	gdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	short characterAmount = rows * charactersPerRow;
	size_t tListSize = sizeof(vec2)*characterAmount;
	if (font->_tList == NULL)
	{
		font->_tList = (vec2*)gdl::AllocateAlignedMemory(tListSize);
		gdl_assert_print(font->_tList != nullptr, "Out of memory when allocation font txcord list");
	}

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {

			// Texture coordinate array index
			u32 tc = (cx+(charactersPerRow*cy));
			_Font_CreateCoordinatesForGlyph(font, tc, cx, cy, texW, texH);
		}
	}
	gdl::CacheFlushRange(font->_tList, tListSize);
}

vec2 _Font_GetTextureCoordinate(Font* font, char character)
{
	int	tc = (character - font->_firstIndex);
	return font->_tList[tc];
}
vec2 _Font_GetTextureCoordinateGlyph(Font* font, gdl::IconSymbol glyph)
{
	int	tc = ((short)glyph - font->_firstIndex);
	return font->_tList[tc];
}
