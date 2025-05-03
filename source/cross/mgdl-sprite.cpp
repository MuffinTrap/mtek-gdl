
#include <mgdl/mgdl-sprite.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-main.h>


Sprite* Sprite_Load(Font* font)
{
	Sprite* sprite = new Sprite();
	sprite->_font = font;
	return sprite;
}

vec3 Sprite_AdjustDrawingPosition(Sprite* sprite, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY)
{
	float dx = x;
	float dy = y;
	float dz = 0.0f;

	float width = sprite->_font->_aspect * scale;
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

void Sprite_Draw2D(Sprite* sprite, u16 spriteIndex, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	vec3 drawPos = Sprite_AdjustDrawingPosition(sprite, x, y, scale, alignX, alignY);
	float width = sprite->_font->_aspect * scale;
	float height = scale;
	const float uvW = sprite->_font->_uvWidth;
	const float uvH = sprite->_font->_uvHeight;

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glColor3f(f.red, f.green, f.blue);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->_font->_fontImage->textureId);

	glBegin(GL_QUADS);


	vec2 tx = _Font_GetTextureCoordinate(sprite->_font, spriteIndex); //LOW LEFT!

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


void Sprite_Draw3D(Sprite* sprite, u16 spriteIndex, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor)
{
	vec3 drawPos = Sprite_AdjustDrawingPosition(sprite, 0, 0, scale, alignX, alignY);
	float width = sprite->_font->_aspect * scale;
	float height = scale;
	const float uvW = sprite->_font->_uvWidth;
	const float uvH = sprite->_font->_uvHeight;

	gdl::RGBA8Floats f = gdl::ColorToFloats(tintColor);
	glColor3f(f.red, f.green, f.blue);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->_font->_fontImage->textureId);

	glBegin(GL_QUADS);

	vec2 tx= _Font_GetTextureCoordinate(sprite->_font, spriteIndex); //LOW LEFT!

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

short Sprite_GetWidth(Sprite* sprite)
{
	return sprite->_font->characterWidth;
}

short Sprite_GetHeight(Sprite* sprite)
{
	return sprite->_font->characterHeight;
}

