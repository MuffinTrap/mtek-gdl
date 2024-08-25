
#include <mgdl/mgdl-spriteset.h>
#include <mgdl/mgdl-util.h>

void gdl::SpriteSet::LoadFromImage(const char* filename, short spriteWidth, short spriteHeight)
{
	gdl::Font::LoadFromImage(filename, spriteWidth, spriteHeight, 0);
}

void gdl::SpriteSet::LoadFromBuffer(const u8* buffer, size_t size, short spriteWidth, short spriteHeight)
{
	gdl::Font::LoadFromBuffer(buffer, size, spriteWidth, spriteHeight, 0);
}

void gdl::SpriteSet::Draw2D(u16 spriteIndex, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	glTranslatef((float)x, (float)y, 0.0f);
	Draw3D(spriteIndex, scale, alignX, alignY, tintColor);
	glTranslatef(-(float)x, -(float)y, 0.0f);
}

void gdl::SpriteSet::Draw3D(u16 spriteIndex, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	float dx = 0.0f;
	float dy = 0.0f;
	float dz = 0.0f;

	float width = aspect * scale;
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

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glBegin(GL_QUADS);
	glColor3f(f.red, f.green, f.blue);

	gdl::vec2 tx0 = GetTextureCoordinate(spriteIndex, 0); // TOP LEFT
	gdl::vec2 tx1= GetTextureCoordinate(spriteIndex, 1); // TOP RIGHT

	gdl::vec2 tx2= GetTextureCoordinate(spriteIndex, 2); // LOW RIGHT
	gdl::vec2 tx3= GetTextureCoordinate(spriteIndex, 3); //LOW LEFT!

	// TOP LEFT
	glTexCoord2f(tx0.x, tx0.y);
	glVertex3f(dx, dy, dz);

	// TOP RIGHT
	glTexCoord2f(tx1.x, tx1.y);
	glVertex3f(dx + width, dy, dz);

	// LOW RIGHT
	glTexCoord2f(tx2.x, tx2.y);
	glVertex3f(dx + width, dy - height, dz);

	// LOW LEFT!
	glTexCoord2f(tx3.x, tx3.y);
	glVertex3f(dx, dy - height, dz);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

short gdl::SpriteSet::GetSpriteWidth()
{
	return cw;
}

short gdl::SpriteSet::GetSpriteHeight()
{
	return ch;
}

