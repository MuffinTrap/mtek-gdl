
#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-util.h>
#include <string.h>
#include <stdarg.h>

bool gdl::Font::LoadFromBuffer (const u8* buffer, size_t size, short charw, short charh, char firstCharacter )
{
	bool imageOk = fontImage.LoadBuffer(buffer, size, gdl::TextureFilterModes::Linear);
	gdl_assert_print(imageOk, "Did not load font image buffer");
	textureName = fontImage.GetTextureId(); // Store this for rendering the letters
	if (imageOk)
	{
		Bind(charw, charh, firstCharacter);
	}
	return imageOk;
}

bool gdl::Font::LoadFromImage(const char* filename, short charw, short charh, char firstCharacter )
{
	bool imageOk = fontImage.LoadFile(filename, gdl::TextureFilterModes::Linear);
	gdl_assert_printf(imageOk, "Did not load font image file: %s", filename);
	textureName = fontImage.GetTextureId(); // Store this for rendering the letters
	if (imageOk)
	{
		printf("\tCreating font/sprite coordinates\n");
		Bind(charw, charh, firstCharacter);
	}
	return imageOk;
}

bool gdl::Font::LoadFromImage(const char* filename, short charw, short charh, char firstCharacter, short charactersPerRow )
{
	bool imageOk = fontImage.LoadFile(filename, gdl::TextureFilterModes::Linear);
	gdl_assert_printf(imageOk, "Did not load font image file: %s", filename);
	textureName = fontImage.GetTextureId(); // Store this for rendering the letters
	if (imageOk)
	{
		printf("\tCreating font/sprite coordinates\n");
		Bind(charw, charh, firstCharacter, charactersPerRow);
	}
	return imageOk;
}

bool gdl::Font::LoadFromImage(const char* filename, short charw, short charh, short charactersPerRow, std::string characters )
{
	bool imageOk = fontImage.LoadFile(filename, gdl::TextureFilterModes::Linear);
	gdl_assert_printf(imageOk, "Did not load font image file: %s", filename);
	textureName = fontImage.GetTextureId(); // Store this for rendering the letters
	if (imageOk)
	{
		printf("\tCreating font/sprite coordinates\n");
		Bind(charw, charh, characters, charactersPerRow);
	}
	return imageOk;
}

bool gdl::Font::LoadFromImage ( gdl::Image& image, short charw, short charh, char firstCharacter )
{
	fontImage = image;
	textureName = fontImage.GetTextureId(); // Store this for rendering the letters
	printf("\tCreating font/sprite coordinates\n");
	Bind(charw, charh, firstCharacter);
	return true;
}


void gdl::Font::Bind (short charw, short charh, char firstCharacter, short charactersPerRow )
{
	short rows = fontImage.GetHeight() / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)
	this->firstIndex = firstCharacter;
	this->cw = charw;
	this->ch = charh;
	aspect = (float)cw/(float)ch;
	CreateTextureCoordList(rows, charactersPerRow, fontImage.GetWidth(), fontImage.GetHeight());

	spacingX = 0.0f;
	spacingY = 0.0f;
}

void gdl::Font::Bind (short charw, short charh, char firstCharacter )
{
	short charactersPerRow = fontImage.GetWidth()/ charw;
	Bind(charw, charh, firstCharacter, charactersPerRow);
}

void gdl::Font::Bind ( short charw, short charh, std::string characters, short charactersPerRow )
{
	short rows = fontImage.GetHeight() / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)
	this->firstIndex = characters[0];
	this->cw = charw;
	this->ch = charh;
	aspect = (float)cw/(float)ch;
	CreateTextureCoordList(rows, charactersPerRow, fontImage.GetWidth(), fontImage.GetHeight(), characters);

	spacingX = 0.0f;
	spacingY = 0.0f;

}


void gdl::Font::Icon ( u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, gdl::IconSymbol glyph )
{
	float step = aspect * textHeight;
	float dx = x;
	float dy = y;
	float dz = 0.0f;

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
		vec2 tx0 = GetTextureCoordinate(glyph, 0); // TOP LEFT
		vec2 tx1= GetTextureCoordinate(glyph, 1); // TOP RIGHT

		vec2 tx2= GetTextureCoordinate(glyph, 2); // LOW RIGHT
		vec2 tx3= GetTextureCoordinate(glyph, 3); //LOW LEFT!

		// LOW LEFT!
		glTexCoord2f(tx3.x, tx3.y);
		glVertex3f(dx, dy - textHeight, dz);

		// LOW RIGHT
		glTexCoord2f(tx2.x, tx2.y);
		glVertex3f(dx + step, dy - textHeight, dz);

		// TOP RIGHT
		glTexCoord2f(tx1.x, tx1.y);
		glVertex3f(dx + step, dy, dz);

		// TOP LEFT
		glTexCoord2f(tx0.x, tx0.y);
		glVertex3f(dx, dy, dz);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

}


void gdl::Font::Print(u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text)
{
	float step = aspect * textHeight;

	float dx = x;
	float dy = y;
	float dz = 0.0f;

	if (alignmentX == RJustify)
	{
		float width = step * strlen(text);
		dx -= width;
	}
	else if (alignmentX == Centered)
	{
		float width = step * strlen(text);
		dx -= width / 2;
	}
	float left = dx;
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
			dy -= textHeight + spacingY;
			continue;
		}
		vec2 tx0 = GetTextureCoordinate(character, 0); // TOP LEFT
		vec2 tx1= GetTextureCoordinate(character, 1); // TOP RIGHT

		vec2 tx2= GetTextureCoordinate(character, 2); // LOW RIGHT
		vec2 tx3= GetTextureCoordinate(character, 3); //LOW LEFT!

		// LOW LEFT!
		glTexCoord2f(tx3.x, tx3.y);
		glVertex3f(dx, dy - textHeight, dz);

		// LOW RIGHT
		glTexCoord2f(tx2.x, tx2.y);
		glVertex3f(dx + step, dy - textHeight, dz);

		// TOP RIGHT
		glTexCoord2f(tx1.x, tx1.y);
		glVertex3f(dx + step, dy, dz);

		// TOP LEFT
		glTexCoord2f(tx0.x, tx0.y);
		glVertex3f(dx, dy, dz);
		dx += step + spacingX;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	spacingX = 0.0f;
	spacingY = 0.0f;
}

void gdl::Font::Print(u32 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text)
{
	Print(color, 0.0f, 0.0f, textHeight, alignmentX, alignmentY, text);
}

void gdl::Font::Print(u32 color, float x, float y, float textHeight, const char* text)
{
	Print(color, x, y, textHeight, gdl::AlignmentModes::LJustify, gdl::AlignmentModes::LJustify, text);
}

void gdl::Font::Printf(u32 color, float x, float y, float textHeight, const char* format, ... )
{
	va_list args;
	char	buff[256];

	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	Print(color, x, y, textHeight, gdl::AlignmentModes::LJustify, gdl::AlignmentModes::LJustify, buff);
}

void gdl::Font::Printf(u32 color, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... )
{
	va_list args;
	char	buff[256];

	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	Print(color, 0, 0, textHeight, alignmentX, alignmentY, buff);
}

void gdl::Font::Printf(u32 color, float x, float y, float textHeight, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... )
{
	// Draw quads
	va_list args;
	char	buff[256];

	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	Print(color, x, y, textHeight, alignmentX, alignmentY, buff);
}

void gdl::Font::SetSpacingOnce ( float x, float y )
{
	spacingX = x;
	spacingY = y;
}

void gdl::Font::DrawSheet ()
{

	float aspect = (float)cw/(float)ch;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);

		// Lower left
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x-aspect*0.5f, y-0.5f, z);
		// Lower right
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+aspect*0.5f, y-0.5f, z);
		// Upper right
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+aspect*0.5f, y+0.5f, z);
		// Upper left
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x-aspect*0.5f, y+0.5f, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

// Version that only creates texture coordinates for
// the given characters.
// All other characters are the same as the first one
void gdl::Font::CreateTextureCoordList ( short rows, short charactersPerRow, short texW, short texH, std::string characters )
{
	gdl_assert_print(cw > 0 && ch > 0, "Character dimensions not set");
	gdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	gdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	// Need to also create texture coordinates for characters that are not included
	char first = characters[0];
	char last = characters[characters.length()-1];
	short textureArraySize =  (last - first) + 1;
	short toBeFoundAmount = static_cast<short>(characters.length());
	printf("find between %d and %d: %d characters\n", first, last, toBeFoundAmount);


	size_t tListSize = (sizeof(vec2)*4)*(textureArraySize);
	if (tList == NULL)
	{
		tList = (vec2*)AllocateAlignedMemory(tListSize);
		gdl_assert_print(tList != nullptr, "Out of memory when allocation font txcord list");
	}

	char currentCharacter = this->firstIndex;
	gdl_assert_print(currentCharacter >= ' ', "First character must be space or bigger");
	short textureIndex = 0;

	printf("Creating coordinates: rows %d, cperRow %d, textureW %d, textrureH %d\n", rows, charactersPerRow, texW, texH);

	// If the last character in image is not included this crashes unless we keep track of how many should be found
	short found = 0;

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {
			// Coordinates to source image
			float tx = cw * cx;
			float ty = ch * cy;
			float tx2 = (tx + cw);
			float ty2 = (ty + ch);

			// Is this character included
			bool included = characters.find_first_of(currentCharacter) != std::string::npos;
			if (!included)
			{
				// Not included: set texture coords same as first one
				tx = 0;
				ty = 0;
				tx2 = (tx + cw);
				ty2 = (ty + ch);

				//printf("Excluded: %c\n", currentCharacter);
				// Decrement cx so that characters that are not included
				// do not advance coordinates
				cx--;
			}
			else
			{
				found++;
				// printf("Included: %c. Found %d/%d\n", currentCharacter, found, toBeFoundAmount);
			}

			u32 tc = textureIndex;
			// Upper-left
			tList[tc].x	= tx/texW;
			tList[tc].y	= ty/texH;

			// Upper-right
			tList[tc+1].x	= tx2/texW;
			tList[tc+1].y	= ty/texH;

			// Lower-right
			tList[tc+2].x	= tx2/texW;
			tList[tc+2].y	= ty2/texH;

			// Lower-left
			tList[tc+3].x	= tx/texW;
			tList[tc+3].y	= ty2/texH;

			// In this version the textureIndex and cx are not the same
			// Because even the characters that are not included have
			// coordinates for them
			textureIndex += 4; // Four coordinates per character

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
	CacheFlushRange(tList, tListSize);
}


void gdl::Font::CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH)
{
	gdl_assert_print(cw > 0 && ch > 0, "Character dimensions not set");
	gdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	gdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	short characterAmount = rows * charactersPerRow;
	size_t tListSize = (sizeof(vec2)*4)*(characterAmount);
	if (tList == NULL)
	{
		tList = (vec2*)AllocateAlignedMemory(tListSize);
		gdl_assert_print(tList != nullptr, "Out of memory when allocation font txcord list");
	}

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {

			// Coordinates to source image
			float tx = cw * cx;
			float ty = ch * cy;
			float tx2 = (tx + cw);
			float ty2 = (ty + ch);

			// Texture coordinate array index
			u32 tc = 4*(cx+(charactersPerRow*cy));

			// Upper-left
			tList[tc].x	= tx/texW;
			tList[tc].y	= ty/texH;

			// Upper-right
			tList[tc+1].x	= tx2/texW;
			tList[tc+1].y	= ty/texH;

			// Lower-right
			tList[tc+2].x	= tx2/texW;
			tList[tc+2].y	= ty2/texH;

			// Lower-left
			tList[tc+3].x	= tx/texW;
			tList[tc+3].y	= ty2/texH;
		}
	}
	CacheFlushRange(tList, tListSize);
}

// TODO: Optimize: return a rectangle
vec2 gdl::Font::GetTextureCoordinate(char character, char subIndex)
{
	int	tc = 4*(character - firstIndex);
	return tList[tc + subIndex];
}
vec2 gdl::Font::GetTextureCoordinate(gdl::IconSymbol glyph, char subIndex)
{
	int	tc = 4*((short)glyph - firstIndex);
	return tList[tc + subIndex];
}



short gdl::Font::GetCharacterWidth() { return cw;}
short gdl::Font::GetCharacterHeight() {return ch;}
