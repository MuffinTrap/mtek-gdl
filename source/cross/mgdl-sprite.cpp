
#include <mgdl/mgdl-sprite.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-assert.h>

void Texture_BeginSpriteBatch(Texture* texture)
{
	ASSERT_DEBUG(texture != nullptr);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);

	glBegin(GL_QUADS);
}

void Texture_DrawBatchedSprite(Texture* texture, u16 spriteIndex, s16 x, s16 y, float scale, color32 tintColor)
{
	ASSERT_DEBUG(texture != nullptr);
	Vector3 drawPos = Vector3New(x,y,0);
	SpriteAtlas* atlas = texture->spriteAtlas;

	float width = atlas->aspectRatio * scale;
	float height = scale;
	const float uvW = atlas->uvWidth;
	const float uvH = atlas->uvHeight;

	mgdl_glColor32(tintColor);
	Vector2 tx = SpriteAtlas_GetTextureCoordinateSprite(atlas, spriteIndex); //LOW LEFT!

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

void Texture_DrawSprite(Texture* texture, u16 spriteIndex, s16 x, s16 y, float scale, color32 tintColor)
{
	ASSERT_DEBUG(texture != nullptr);
	Texture_BeginSpriteBatch(texture);

	Texture_DrawBatchedSprite(texture, spriteIndex, x, y, scale, tintColor);

	Texture_EndSpriteBatch();
}

void Texture_EndSpriteBatch(void)
{
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}
