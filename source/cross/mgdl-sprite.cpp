
#include <mgdl/mgdl-sprite.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-opengl_util.h>


Sprite* Sprite_Load(Font* font)
{
	Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
	sprite->_font = font;
	return sprite;
}

V3f Sprite_AdjustDrawingPosition2D(Sprite* sprite, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY)
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
	return V3f_Create(dx, dy, dz);
}

V3f Sprite_AdjustDrawingPosition3D(Sprite* sprite, V3f position, float scale, AlignmentModes alignX, AlignmentModes alignY)
{
	float dx = V3f_X(position);
	float dy = V3f_Y(position);
	float dz = V3f_Z(position);

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
	return V3f_Create(dx, dy, dz);
}

void Sprite_BeginDrawBatch(Sprite* sprite)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite->_font->_fontTexture->textureId);

	glBegin(GL_QUADS);
}

void Sprite_Draw2DBatched(Sprite* sprite, u16 spriteIndex, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY, Color4f* tintColor)
{
	V3f drawPos = Sprite_AdjustDrawingPosition2D(sprite, x, y, scale, alignX, alignY);

	float width = sprite->_font->_aspect * scale;
	float height = scale;
	const float uvW = sprite->_font->_uvWidth;
	const float uvH = sprite->_font->_uvHeight;

	mgdl_glColor4f(tintColor);
	V2f tx = _Font_GetTextureCoordinate(sprite->_font, spriteIndex); //LOW LEFT!

	// LOW LEFT!
	glTexCoord2f(V2f_X(tx), V2f_Y(tx));
	glVertex2f(V3f_X(drawPos), V3f_Y(drawPos) - height);

	// LOW RIGHT
	glTexCoord2f(V2f_X(tx) + uvW, V2f_Y(tx));
	glVertex2f(V3f_X(drawPos) + width, V3f_Y(drawPos) - height);

	// TOP RIGHT

	glTexCoord2f(V2f_X(tx) + uvW, V2f_Y(tx) + uvH);
	glVertex2f(V3f_X(drawPos) + width, V3f_Y(drawPos));

	// TOP LEFT
	glTexCoord2f(V2f_X(tx), V2f_Y(tx) + uvH);
	glVertex2f(V3f_X(drawPos), V3f_Y(drawPos) );
}

void Sprite_Draw2D(Sprite* sprite, u16 spriteIndex, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY, Color4f* tintColor)
{
	Sprite_BeginDrawBatch(sprite);

	Sprite_Draw2DBatched(sprite, spriteIndex, x, y, scale, alignX, alignY, tintColor);

	Sprite_EndDrawBatch();
}

void Sprite_EndDrawBatch(void)
{
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Sprite_Draw3DBatched(Sprite* sprite, u16 spriteIndex, V3f position, float scale, AlignmentModes alignX, AlignmentModes alignY, Color4f* tintColor)
{
	V3f drawPos = Sprite_AdjustDrawingPosition3D(sprite, position, scale, alignX, alignY);
	float width = sprite->_font->_aspect * scale;
	float height = scale;
	const float uvW = sprite->_font->_uvWidth;
	const float uvH = sprite->_font->_uvHeight;

	V2f tx= _Font_GetTextureCoordinate(sprite->_font, spriteIndex); //LOW LEFT!

	mgdl_glColor4f(tintColor);

	// LOW LEFT!
	glTexCoord2f(V2f_X(tx), V2f_Y(tx));
	glVertex3f(V3f_X(drawPos), V3f_Y(drawPos) - height, V3f_Z(drawPos));

	// LOW RIGHT
	glTexCoord2f(V2f_X(tx) + uvW, V2f_Y(tx));
	glVertex3f(V3f_X(drawPos) + width, V3f_Y(drawPos) - height, V3f_Z(drawPos));

	// TOP RIGHT
	glTexCoord2f(V2f_X(tx) + uvW, V2f_Y(tx) + uvH);
	glVertex3f(V3f_X(drawPos) + width, V3f_Y(drawPos), V3f_Z(drawPos));

	// TOP LEFT
	glTexCoord2f(V2f_X(tx), V2f_Y(tx) + uvH);
	glVertex3f(V3f_X(drawPos), V3f_Y(drawPos), V3f_Z(drawPos));
}

void Sprite_Draw3D(Sprite* sprite, u16 spriteIndex, float scale, AlignmentModes alignX, AlignmentModes alignY, Color4f* tintColor)
{
	Sprite_BeginDrawBatch(sprite);

	Sprite_Draw3DBatched(sprite, spriteIndex, V3f_Create(0.0f, 0.0f, 0.0f), scale, alignX, alignY, tintColor);

	Sprite_EndDrawBatch();
}

short Sprite_GetWidth(Sprite* sprite)
{
	return sprite->_font->characterWidth;
}

short Sprite_GetHeight(Sprite* sprite)
{
	return sprite->_font->characterHeight;
}

