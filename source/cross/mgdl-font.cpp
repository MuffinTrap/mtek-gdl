
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
		Bind(charw, charh, firstCharacter);
	}
	return imageOk;
}

void gdl::Font::Bind (short charw, short charh, char firstCharacter )
{
	printf("Binding font\n");
	short charactersPerRow = fontImage.GetWidth()/ charw;
	short rows = fontImage.GetHeight() / charh;
	// Calculate the vertex and texture coordinates (vertices are not used)
	this->firstIndex = firstCharacter;
	this->cw = charw;
	this->ch = charh;
	aspect = (float)cw/(float)ch;
	CreateTextureCoordList(rows, charactersPerRow, fontImage.GetWidth(), fontImage.GetHeight());
	printf("Bind done\n");

	spacingX = 0.0f;
	spacingY = 0.0f;
}

void gdl::Font::Print(u32 color, float scale, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* text)
{
	float step = aspect * scale;

	float dx = 0.0f;
	float dy = 0.0f;
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
		dy += scale;
	}
	else if (alignmentY == Centered)
	{
		dy += scale/2.0f;
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
			dy -= scale + spacingY;
			continue;
		}
		gdl::vec2 tx0 = GetTextureCoordinate(character, 0); // TOP LEFT
		gdl::vec2 tx1= GetTextureCoordinate(character, 1); // TOP RIGHT

		gdl::vec2 tx2= GetTextureCoordinate(character, 2); // LOW RIGHT
		gdl::vec2 tx3= GetTextureCoordinate(character, 3); //LOW LEFT!

		// TOP LEFT
		glTexCoord2f(tx0.x, tx0.y);
		glVertex3f(dx, dy, dz);

		// TOP RIGHT
		glTexCoord2f(tx1.x, tx1.y);
		glVertex3f(dx + step, dy, dz);

		// LOW RIGHT
		glTexCoord2f(tx2.x, tx2.y);
		glVertex3f(dx + step, dy - scale, dz);

		// LOW LEFT!
		glTexCoord2f(tx3.x, tx3.y);
		glVertex3f(dx, dy - scale, dz);

		dx += step + spacingX;
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	spacingX = 0.0f;
	spacingY = 0.0f;
}


void gdl::Font::Printf(u32 color, float scale, gdl::AlignmentModes alignmentX, gdl::AlignmentModes alignmentY, const char* format, ... )
{
	// Draw quads
	va_list args;
	char	buff[256];

	va_start(args, format);
	vsprintf(buff, format, args);
	va_end(args);

	Print(color, scale, alignmentX, alignmentY, buff);
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

		// Upper left
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x-aspect*0.5f, y+0.5f, z);

		// Upper right
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+aspect*0.5f, y+0.5f, z);

		// Lower right
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+aspect*0.5f, y-0.5f, z);

		// Lower left
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x-aspect*0.5f, y-0.5f, z);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);


	glBegin(GL_LINE_LOOP);
		glColor3f(1.0f, 1.0f, 1.0f);

		// Upper left
		glVertex3f(x-aspect*0.5f, y+0.5f, z);

		// Upper right
		glVertex3f(x+aspect*0.5f, y+0.5f, z);

		// Lower right
		glVertex3f(x+aspect*0.5f, y-0.5f, z);

		// Lower left
		glVertex3f(x-aspect*0.5f, y-0.5f, z);

	glEnd();
}

void gdl::Font::CreateTextureCoordList(short rows, short charactersPerRow, short texW, short texH)
{
	gdl_assert_print(cw > 0 && ch > 0, "Character dimensions not set");
	gdl_assert_print(rows > 0 && charactersPerRow > 0, "Rows and cpr at zero");
	gdl_assert_print(texW > 0 && texH > 0, "Texture size is 0");

	short characterAmount = rows * charactersPerRow;
	size_t tListSize = (sizeof(gdl::vec2)*4)*(characterAmount);
	if (tList == NULL)
	{
		tList = (gdl::vec2*)AllocateAlignedMemory(tListSize);
		gdl_assert_print(tList != nullptr, "Out of memory when allocation font txcord list");
	}

	for(short cy=0; cy<rows; cy++) {
		for(short cx=0; cx<charactersPerRow; cx++) {

			// Coordinates to source image
			float tx = cw * cx;
			float ty = ch * cy;
			float tx2 = (tx + cw) -1;
			float ty2 = (ty + ch) -1;

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

gdl::vec2 gdl::Font::GetTextureCoordinate(char character, char subIndex)
{
	int	tc = 4*(character - firstIndex);
	return tList[tc + subIndex];
}



short gdl::Font::GetCharacterWidth() { return cw;}
short gdl::Font::GetCharacterHeight() {return ch;}
