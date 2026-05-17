
#include <mgdl/mgdl-spriteatlas.h>
#include <mgdl/mgdl-defaultfont.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-memory.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-texture.h>

#include <string.h>
#include <stdarg.h>

SpriteAtlas* SpriteAtlas_Create(void)
{
	SpriteAtlas* font = (SpriteAtlas*)malloc(sizeof(SpriteAtlas));
	font->uvList = nullptr;
	font->characterWidth = 0;
	font->characterHeight = 0;
	font->uvWidth = 0;
	font->uvHeight = 0;
	font->firstIndex = 0;
	font->characterCount = 0;
	font->aspectRatio = 1;
	return font;
}

s16 Texture_GetSpriteWidth(Texture* texture) { return texture->spriteAtlas->spriteWidth; }
s16 Texture_GetSpriteHeight(Texture* texture) { return texture->spriteAtlas->spriteHeight; }
s16 Texture_GetCharacterWidth(Texture* texture) { return texture->spriteAtlas->characterWidth; }
s16 Texture_GetCharacterHeight(Texture* texture) { return texture->spriteAtlas->characterHeight; }

void SpriteAtlas_MapSimple(Texture* texture, short charw, short charh, char firstCharacter )
{
	mgdl_assert_print(texture != nullptr, "SpriteAtlas_MapSimple got nullptr for texture\n");
	mgdl_assert_print(texture->spriteAtlas == nullptr, "SpriteAtlas_MapSimple Already mapped\n");

	texture->spriteAtlas = SpriteAtlas_Create();

	SpriteAtlas_Bind(texture, charw, charh, firstCharacter);
}

void SpriteAtlas_MapSelective(Texture* texture, short int charw, short int charh, const char* characters)
{
	mgdl_assert_print(texture != nullptr, "SpriteAtlas_Load got nullptr for fontTexture\n");

	SpriteAtlas* font = SpriteAtlas_Create();
	texture->spriteAtlas = font;

	SpriteAtlas_BindSelective(texture, charw, charh, characters);
}

static void SpriteAtlas_CalculateSizes(Texture* texture, short charw, short charh)
{
	ASSERT_DEBUG(texture != nullptr);
	const short tw = texture->width;
	const short th = texture->height;
	// Calculate the vertex and texture coordinates (vertices are not used)
	SpriteAtlas* font = texture->spriteAtlas;
	font->characterWidth = charw;
	font->characterHeight = charh;
	font->uvWidth = (float)charw/(float)tw;
	font->uvHeight = (float)charh/(float)th;
	font->aspectRatio = (float)charw/(float)charh;
}


void SpriteAtlas_Bind(Texture* texture,short charw, short charh, char firstCharacter)
{
	SpriteAtlas_CalculateSizes(texture, charw, charh);
	SpriteAtlas* font = texture->spriteAtlas;
	font->firstIndex = firstCharacter;
	short rows = texture->width / charh;
	short charactersPerRow = texture->width / charw;

	SpriteAtlas_CreateTextureCoordList(font, rows, charactersPerRow, texture->width, texture->height);
}


void SpriteAtlas_BindSelective (Texture* texture, short charw, short charh, const char* characters)
{
	mgdl_assert_print(characters != nullptr, "No characters given to BindSelective");
	SpriteAtlas_CalculateSizes(texture, charw, charh);
	SpriteAtlas* font = texture->spriteAtlas;
	font->firstIndex = characters[0];
	short rows = texture->width / charh;
	short charactersPerRow = texture->width / charw;
	SpriteAtlas_CreateTextureCoordListSelective(font, rows, charactersPerRow, texture->width, texture->height, characters);
}


void DefaultFont_DrawIcon (color32 color, float x, float y, float textHeight, IconSymbol glyph )
{
	DefaultFont_DrawIconRotated(color, x, y, textHeight, 0, glyph);
}

void DefaultFont_DrawIconRotated(color32 color, float x, float y, float textHeight, u8 rotation, IconSymbol glyph)
{
	Texture* font = DefaultFont_GetDefaultFont();
	GLuint textureName = font->textureId;
	float step = font->spriteAtlas->aspectRatio * textHeight;
	float dx = x;
	float dy = y;
	float dz = 0.0f;

	const float uvW = font->spriteAtlas->uvWidth;
	const float uvH = font->spriteAtlas->uvHeight;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);
    // Discard pixels with low alpha

	Vector2 tx= DefaultFont_GetTextureCoordinateGlyph(glyph); //LOW LEFT!
	float uvs[] = {tx.x, tx.y,  // low left
		tx.x + uvW, tx.y,       // low right
		tx.x + uvW, tx.y + uvH, // high right
		tx.x, tx.y + uvH,       // high left
	};
	short uvIndex = 0 + rotation * 2;

	glBegin(GL_QUADS);
	mgdl_glColor32(color);


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


void Texture_DrawText(Texture* texture, color32 color, float x, float y, float textHeight, const char* text)
{
	ASSERT_DEBUG(texture != nullptr);

	GLuint textureName = texture->textureId;
	SpriteAtlas* font = texture->spriteAtlas;
	const float step = font->aspectRatio * textHeight;

	float dx = x;
	float dy = y;
	float dz = 0.0f;

	const float uvW = font->uvWidth;
	const float uvH = font->uvHeight;

	const float left = dx;
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);
    // Discard pixels with low alpha

	glBegin(GL_QUADS);
	mgdl_glColor32(color);
	for (short c = 0; text[c] != '\0'; c++)
	{
		char character = text[c];
		if (character == '\n')
		{
			dx = left;
			dy -= textHeight;
			continue;
		}
		Vector2 tx= SpriteAtlas_GetTextureCoordinateChar(font, character); //LOW LEFT!


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

		dx += step;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}


void Texture_DrawTextF(Texture* texture, color32 color, float x, float y, float textHeight, const char* format, ... )
{
	MGDL_PRINTF_TO_BUFFER(format)
	Texture_DrawText(texture, color, x, y, textHeight, mgdl_GetPrintfBuffer());
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

RectF SpriteAtlas_GetUVRect(SpriteAtlas* font, char letter)
{
	Vector2 uv = SpriteAtlas_GetTextureCoordinateChar(font, letter);
	return {V2f_X(uv), V2f_Y(uv), font->uvWidth, font->uvHeight};
}

RectF DefaultFont_GetUVRectIcon(SpriteAtlas* font, IconSymbol glyph)
{
	Vector2 uv = DefaultFont_GetTextureCoordinateGlyph(glyph);
	return {V2f_X(uv), V2f_Y(uv), font->uvWidth, font->uvHeight};
}
// Version that only creates texture coordinates for
// the given characters.
// All other characters are the same as the first one

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void SpriteAtlas_CreateTextureCoordListSelective (SpriteAtlas* font, short rows, short charactersPerRow, short texW, short texH, const char* characters )
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



	font->characterCount = textureArraySize;
	size_t tListSize = sizeof(Vector2)*textureArraySize;
	if (font->uvList == NULL)
	{
		font->uvList = (Vector2*)mgdl_AllocateGeneralMemory(tListSize);
		mgdl_assert_print(font->uvList != nullptr, "Out of memory when allocation font txcord list");
	}

	Log_InfoF("Creating coordinates: rows %d, cperRow %d, textureW %d, textrureH %d\n", rows, charactersPerRow, texW, texH);

	char currentCharacter = font->firstIndex;
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

				SpriteAtlas_CreateCoordinatesForGlyph(font, textureIndex, 0, 0, texW, texH);

				//printf("Excluded: %c\n", currentCharacter);
				// Decrement cx so that characters that are not included
				// do not advance coordinates
				cx--;
			}
			else
			{
				found++;
				// printf("Included: %c. Found %d/%d\n", currentCharacter, found, toBeFoundAmount);

				SpriteAtlas_CreateCoordinatesForGlyph(font, textureIndex, cx, cy, texW, texH);
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
}

void SpriteAtlas_CreateCoordinatesForGlyph (SpriteAtlas* font, u32 textureIndex, short cx, short cy, short texW, short texH )
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
	font->uvList[tc+0].x	= tx/texW;
	font->uvList[tc+0].y	= ty/texH;
}



void SpriteAtlas_CreateTextureCoordList(SpriteAtlas* font, short rows, short charactersPerRow, short texW, short texH)
{
	mgdl_assert_print(font != nullptr, "SpriteAtlas is null");
	mgdl_assert_print(font->characterWidth > 0 && font->characterHeight > 0, "Character dimensions not set");
	mgdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	mgdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	short characterAmount = rows * charactersPerRow;
	font->characterCount = characterAmount;
	size_t tListSize = sizeof(Vector2)*characterAmount;
	if (font->uvList == NULL)
	{
		font->uvList = (Vector2*)mgdl_AllocateGeneralMemory(tListSize);
		mgdl_assert_print(font->uvList != nullptr, "Out of memory when allocation font txcord list");
	}

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {

			// Texture coordinate array index
			u32 tc = (cx+(charactersPerRow*cy));
			SpriteAtlas_CreateCoordinatesForGlyph(font, tc, cx, cy, texW, texH);
		}
	}
}

Vector2 SpriteAtlas_GetTextureCoordinateChar(SpriteAtlas* font, char character)
{
	int	tc = (character - font->firstIndex);
	return font->uvList[tc];
}
Vector2 SpriteAtlas_GetTextureCoordinateSprite(SpriteAtlas* font, short sprite)
{
	int	tc = (sprite - font->firstIndex);
	return font->uvList[tc];
}
Vector2 DefaultFont_GetTextureCoordinateGlyph(IconSymbol glyph)
{
	SpriteAtlas* font = DefaultFont_GetDefaultFont()->spriteAtlas;
	int	tc = ((short)glyph - font->firstIndex);
	return font->uvList[tc];
}
