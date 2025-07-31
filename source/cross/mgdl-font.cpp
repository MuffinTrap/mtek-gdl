
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-logger.h>

#include <string.h>
#include <stdarg.h>

static short lineLimit_ = -1;

Font* Font_Create(void)
{
	Font* font = (Font*)malloc(sizeof(Font));
	font->_fontTexture = nullptr;
	font->_tList = nullptr;
	font->characterWidth = 0;
	font->characterHeight = 0;
	font->_uvWidth = 0;
	font->_uvHeight = 0;
	font->_firstIndex = 0;
	font->_characterCount = 0;
	font->_spacingX = 0;
	font->_spacingY = 0;
	font->_aspect = 0;
	return font;
}

Font* Font_Load(Texture* fontTexture, short charw, short charh, char firstCharacter )
{
	mgdl_assert_print(fontTexture != nullptr, "Font_Load got nullptr for fontTexture\n");

	Font* font = Font_Create();
	font->_fontTexture = fontTexture;
	// TODO Log info printf("\tCreating font/sprite coordinates\n");
	_Font_Bind(font, charw, charh, firstCharacter);
	return font;
}

Font* Font_LoadPadded(Texture* fontTexture, short charw, short charh, char firstCharacter, short charactersPerRow )
{
	mgdl_assert_print(fontTexture != nullptr, "Font_Load got nullptr for fontTexture\n");

	Font* font = Font_Create();
	font->_fontTexture = fontTexture;
	// TODO Log info printf("\tCreating font/sprite coordinates\n");
	_Font_BindPadded(font, charw, charh, firstCharacter, charactersPerRow);
	return font;
}

Font* Font_LoadSelective(Texture* fontTexture, short charw, short charh, short charactersPerRow, const char* characters )
{
	mgdl_assert_print(fontTexture != nullptr, "Font_Load got nullptr for fontTexture\n");

	Font* font = Font_Create();
	font->_fontTexture = fontTexture;
	// TODO Log info printf("\tCreating font/sprite coordinates\n");
	_Font_BindSelective(font, charw, charh, characters, charactersPerRow);
	return font;
}


void _Font_BindPadded(Font* font,short charw, short charh, char firstCharacter, short charactersPerRow )
{
	const short tw = font->_fontTexture->width;
	const short th = font->_fontTexture->height;
	short rows = th / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)
	font->_firstIndex = firstCharacter;
	font->characterWidth = charw;
	font->characterHeight = charh;
	font->_uvWidth = (float)charw/(float)tw;
	font->_uvHeight = (float)charh/(float)th;
	font->_aspect = (float)charw/(float)charh;

	_Font_CreateTextureCoordList(font, rows, charactersPerRow, tw, th);

	font->_spacingX = 0.0f;
	font->_spacingY = 0.0f;
}

void _Font_Bind(Font* font, short charw, short charh, char firstCharacter )
{
	short charactersPerRow = font->_fontTexture->width/ charw;
	_Font_BindPadded(font, charw, charh, firstCharacter, charactersPerRow);
}

void _Font_BindSelective (Font* font, short charw, short charh, const char* characters, short charactersPerRow )
{
	const short tw = font->_fontTexture->width;
	const short th = font->_fontTexture->height;
	short rows = th / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)

	font->_firstIndex = characters[0];

	font->characterWidth = charw;
	font->characterHeight = charh;
	font->_uvWidth = (float)charw/(float)tw;
	font->_uvHeight = (float)charh/(float)th;
	font->_aspect = (float)charw/(float)charh;

	_Font_CreateTextureCoordListSelective(font, rows, charactersPerRow, tw, th, characters);

	font->_spacingX = 0.0f;
	font->_spacingY = 0.0f;
}


void Font_Icon (Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, IconSymbol glyph )
{
	Font_IconRotated(font, color, x, y, textHeight, alignmentX, alignmentY, 0, glyph);
}

void Font_IconRotated(Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, u8 rotation, IconSymbol glyph)
{
	GLuint textureName = font->_fontTexture->textureId;
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

	vec2 tx= _Font_GetTextureCoordinateGlyph(font, glyph); //LOW LEFT!
	float uvs[] = {tx.x, tx.y,  // low left
		tx.x + uvW, tx.y,       // low right
		tx.x + uvW, tx.y + uvH, // high right
		tx.x, tx.y + uvH,       // high left
	};
	short uvIndex = 0 + rotation * 2;

	glBegin(GL_QUADS);
	mgdl_glColor3f(color);


		// LOW LEFT!
		glTexCoord2f(uvs[uvIndex], uvs[uvIndex+1]);
		glVertex3f(dx, dy - textHeight, dz);
		uvIndex = (uvIndex + 2) % 8;

		// LOW RIGHT
		glTexCoord2f(uvs[uvIndex], uvs[uvIndex+1]);
		glVertex3f(dx + step, dy - textHeight, dz);

		uvIndex = (uvIndex + 2) % 8;

		// TOP RIGHT
		glTexCoord2f(uvs[uvIndex], uvs[uvIndex+1]);
		glVertex3f(dx + step, dy, dz);

		uvIndex = (uvIndex + 2) % 8;

		// TOP LEFT
		glTexCoord2f(uvs[uvIndex], uvs[uvIndex+1]);
		glVertex3f(dx, dy, dz);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

}


void Font_PrintAligned(Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* text)
{
	GLuint textureName = font->_fontTexture->textureId;
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

	glBegin(GL_QUADS);
	mgdl_glColor3f(color);
	for (short c = 0; text[c] != '\0'; c++)
	{
		if (lineLimit_ >= 0 && c >= lineLimit_) { break;}

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
	lineLimit_ = -1;
}

void Font_Print(Font* font, Color4f* color, float x, float y, float textHeight, const char* text)
{
	Font_PrintAligned(font, color, x, y, textHeight, AlignmentModes::LJustify, AlignmentModes::LJustify, text);
}

void Font_PrintOrigo(Font* font, Color4f* color, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* text)
{
	Font_PrintAligned(font, color, 0.0f, 0.0f, textHeight, alignmentX, alignmentY, text);
}


void Font_Printf(Font* font, Color4f* color, float x, float y, float textHeight, const char* format, ... )
{
	MGDL_PRINTF_TO_BUFFER(format)

	Font_PrintAligned(font, color, x, y, textHeight, AlignmentModes::LJustify, AlignmentModes::LJustify, mgdl_GetPrintfBuffer());
}

void Font_PrintfOrigo(Font* font, Color4f* color, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* format, ... )
{
	MGDL_PRINTF_TO_BUFFER(format)

	Font_PrintAligned(font, color, 0, 0, textHeight, alignmentX, alignmentY, mgdl_GetPrintfBuffer());
}

void Font_PrintfAligned( Font* font, Color4f* color, float x, float y, float textHeight, AlignmentModes alignmentX, AlignmentModes alignmentY, const char* format, ... )
{
	// Draw quads

	MGDL_PRINTF_TO_BUFFER(format)

	Font_PrintAligned(font, color, x, y, textHeight, alignmentX, alignmentY, mgdl_GetPrintfBuffer());
}

void Font_SetSpacingOnce (Font* font, float x, float y )
{
	font->_spacingX = x;
	font->_spacingY = y;
}

void Font_SetLineLimitOnce(short limit)
{
	lineLimit_ = limit;
}

static int String_FindFirst(const char* str, char character)
{
	sizetype length = strlen(str);
	for (sizetype i = 0; i < length; i++)
	{
		if (str[i] == character)
		{
			return (int)i;
		}
	}
	return -1;

}

RectF Font_GetUVRect(Font* font, char letter)
{
	vec2 uv = _Font_GetTextureCoordinate(font, letter);
	return {V2f_X(uv), V2f_Y(uv), font->_uvWidth, font->_uvHeight};
}

RectF Font_GetUVRectIcon(Font* font, IconSymbol glyph)
{
	vec2 uv = _Font_GetTextureCoordinateGlyph(font, glyph);
	return {V2f_X(uv), V2f_Y(uv), font->_uvWidth, font->_uvHeight};
}
// Version that only creates texture coordinates for
// the given characters.
// All other characters are the same as the first one

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void _Font_CreateTextureCoordListSelective (Font* font, short rows, short charactersPerRow, short texW, short texH, const char* characters )
{
	mgdl_assert_print(font->characterWidth > 0 && font->characterHeight > 0, "Character dimensions not set");
	mgdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	mgdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	// Need to also create texture coordinates for characters that are not included
	char first = characters[0];
	char last = characters[strlen(characters)-1];
	short textureArraySize =  (last - first) + 1;
	short toBeFoundAmount = static_cast<short>(strlen(characters));
	Log_InfoF("find between %d and %d: %d characters\n", first, last, toBeFoundAmount);


	font->_characterCount = textureArraySize;
	size_t tListSize = sizeof(vec2)*textureArraySize;
	if (font->_tList == NULL)
	{
		font->_tList = (vec2*)mgdl_AllocateAlignedMemory(tListSize);
		mgdl_assert_print(font->_tList != nullptr, "Out of memory when allocation font txcord list");
	}

	Log_InfoF("Creating coordinates: rows %d, cperRow %d, textureW %d, textrureH %d\n", rows, charactersPerRow, texW, texH);

	char currentCharacter = font->_firstIndex;
	mgdl_assert_print(currentCharacter >= ' ', "First character must be space or bigger");

	// If the last character in image is not included this crashes unless we keep track of how many should be found
	short textureIndex = 0;
	short found = 0;

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {
			// Coordinates to source image
			// Is this character included
			bool included = String_FindFirst(characters, currentCharacter) != -1;
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
				Log_InfoF("All %d characters found\n", toBeFoundAmount);
				break;
			}

		}
		if (found == toBeFoundAmount)
		{
			break;
		}
	}
	mgdl_CacheFlushRange(font->_tList, tListSize);
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
	float tx = font->characterWidth * cx;
	float ty = texH - font->characterHeight * (cy+1);

	// Lower-left
	font->_tList[tc+0].x	= tx/texW;
	font->_tList[tc+0].y	= ty/texH;
}



void _Font_CreateTextureCoordList(Font* font, short rows, short charactersPerRow, short texW, short texH)
{
	mgdl_assert_print(font != nullptr, "Font is null");
	mgdl_assert_print(font->characterWidth > 0 && font->characterHeight > 0, "Character dimensions not set");
	mgdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	mgdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	short characterAmount = rows * charactersPerRow;
	font->_characterCount = characterAmount;
	size_t tListSize = sizeof(vec2)*characterAmount;
	if (font->_tList == NULL)
	{
		font->_tList = (vec2*)mgdl_AllocateAlignedMemory(tListSize);
		mgdl_assert_print(font->_tList != nullptr, "Out of memory when allocation font txcord list");
	}

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {

			// Texture coordinate array index
			u32 tc = (cx+(charactersPerRow*cy));
			_Font_CreateCoordinatesForGlyph(font, tc, cx, cy, texW, texH);
		}
	}
	mgdl_CacheFlushRange(font->_tList, tListSize);
}

vec2 _Font_GetTextureCoordinate(Font* font, char character)
{
	int	tc = (character - font->_firstIndex);
	return font->_tList[tc];
}
vec2 _Font_GetTextureCoordinateGlyph(Font* font, IconSymbol glyph)
{
	int	tc = ((short)glyph - font->_firstIndex);
	return font->_tList[tc];
}
