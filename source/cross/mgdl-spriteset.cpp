
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

vec3 gdl::SpriteSet::AdjustDrawingPosition(short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY)
{
	float dx = x;
	float dy = y;
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
	return vec3New(dx, dy, dz);
}

void gdl::SpriteSet::Draw2D(u16 spriteIndex, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	vec3 drawPos = AdjustDrawingPosition(x, y, scale, alignX, alignY);
	float width = aspect * scale;
	float height = scale;

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glColor3f(f.red, f.green, f.blue);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);

	glBegin(GL_QUADS);

	vec2 tx0 = GetTextureCoordinate(spriteIndex, 0); // TOP LEFT
	vec2 tx1= GetTextureCoordinate(spriteIndex, 1); // TOP RIGHT

	vec2 tx2= GetTextureCoordinate(spriteIndex, 2); // LOW RIGHT
	vec2 tx3= GetTextureCoordinate(spriteIndex, 3); //LOW LEFT!

	// TOP LEFT
	glTexCoord2f(tx0.x, tx0.y);
	glVertex2f(drawPos.x, drawPos.y );

	// LOW LEFT!
	glTexCoord2f(tx3.x, tx3.y);
	glVertex2f(drawPos.x, drawPos.y - height);

	// LOW RIGHT
	glTexCoord2f(tx2.x, tx2.y);
	glVertex2f(drawPos.x + width, drawPos.y - height);

	// TOP RIGHT
	glTexCoord2f(tx1.x, tx1.y);
	glVertex2f(drawPos.x + width, drawPos.y);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


void gdl::SpriteSet::Draw3D(u16 spriteIndex, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	vec3 drawPos = AdjustDrawingPosition(0, 0, scale, alignX, alignY);
	float width = aspect * scale;
	float height = scale;

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glColor3f(f.red, f.green, f.blue);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureName);

	glBegin(GL_QUADS);

	vec2 tx0 = GetTextureCoordinate(spriteIndex, 0); // TOP LEFT
	vec2 tx1= GetTextureCoordinate(spriteIndex, 1); // TOP RIGHT

	vec2 tx2= GetTextureCoordinate(spriteIndex, 2); // LOW RIGHT
	vec2 tx3= GetTextureCoordinate(spriteIndex, 3); //LOW LEFT!

	// TOP LEFT
	glTexCoord2f(tx0.x, tx0.y);
	glVertex3f(drawPos.x, drawPos.y, drawPos.z);

	// LOW LEFT!
	glTexCoord2f(tx3.x, tx3.y);
	glVertex3f(drawPos.x, drawPos.y - height, drawPos.z);

	// LOW RIGHT
	glTexCoord2f(tx2.x, tx2.y);
	glVertex3f(drawPos.x + width, drawPos.y - height, drawPos.z);

	// TOP RIGHT
	glTexCoord2f(tx1.x, tx1.y);
	glVertex3f(drawPos.x + width, drawPos.y, drawPos.z);

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

