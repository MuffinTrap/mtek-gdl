
#include <mgdl/mgdl-spriteset.h>
#include <mgdl/mgdl-util.h>

void gdl::SpriteSet::LoadFromImage(const char* filename, short spriteWidth, short spriteHeight)
{
	Image* fontImage = Image_LoadFile(filename, gdl::TextureFilterModes::Linear);
	font = Font_Load(fontImage, spriteWidth, spriteHeight, 0);
}

vec3 gdl::SpriteSet::AdjustDrawingPosition(short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY)
{
	float dx = x;
	float dy = y;
	float dz = 0.0f;

	float width = font->_aspect * scale;
	float height = scale;
	if (alignX == RJustify)
	{
		dx -= width;
	}
	else if (alignX == Centered)
	{
		dx -= width / 2;
	}
	if (alignY == RJustify)
	{
		dy += height;
	}
	else if (alignY == Centered)
	{
		dy += height/2.0f;
	}
	return vec3New(dx, dy, dz);
}

void gdl::SpriteSet::Draw2D(u16 spriteIndex, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	vec3 drawPos = AdjustDrawingPosition(x, y, scale, alignX, alignY);
	float width = font->_aspect * scale;
	float height = scale;
	const float uvW = font->_uvWidth;
	const float uvH = font->_uvHeight;

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glColor3f(f.red, f.green, f.blue);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font->_fontImage->textureId);

	glBegin(GL_QUADS);


	vec2 tx = _Font_GetTextureCoordinate(font, spriteIndex); //LOW LEFT!

	// LOW LEFT!
	glTexCoord2f(tx.x, tx.y);
	glVertex2f(drawPos.x, drawPos.y - height);

	// LOW RIGHT
	glTexCoord2f(tx.x + uvW, tx.y);
	glVertex2f(drawPos.x + width, drawPos.y - height);

	// TOP RIGHT
	glTexCoord2f(tx.x + uvW, tx.y + uvH);
	glVertex2f(drawPos.x + width, drawPos.y);

	// TOP LEFT
	glTexCoord2f(tx.x, tx.y + uvH);
	glVertex2f(drawPos.x, drawPos.y );

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


void gdl::SpriteSet::Draw3D(u16 spriteIndex, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	vec3 drawPos = AdjustDrawingPosition(0, 0, scale, alignX, alignY);
	float width = font->_aspect * scale;
	float height = scale;
	const float uvW = font->_uvWidth;
	const float uvH = font->_uvHeight;

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glColor3f(f.red, f.green, f.blue);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font->_fontImage->textureId);

	glBegin(GL_QUADS);

	vec2 tx= _Font_GetTextureCoordinate(font, spriteIndex); //LOW LEFT!

	// LOW LEFT!
	glTexCoord2f(tx.x, tx.y);
	glVertex3f(drawPos.x, drawPos.y - height, drawPos.z);

	// LOW RIGHT
	glTexCoord2f(tx.x + uvW, tx.y);
	glVertex3f(drawPos.x + width, drawPos.y - height, drawPos.z);

	// TOP RIGHT
	glTexCoord2f(tx.x + uvW, tx.y + uvH);
	glVertex3f(drawPos.x + width, drawPos.y, drawPos.z);

	// TOP LEFT
	glTexCoord2f(tx.x, tx.y + uvH);
	glVertex3f(drawPos.x, drawPos.y, drawPos.z);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

short gdl::SpriteSet::GetSpriteWidth()
{
	return font->cw;
}

short gdl::SpriteSet::GetSpriteHeight()
{
	return font->ch;
}

