#pragma once

#include "mgdl-font.h"

// This class is almost the same as the font class, but supports
// combining sprites and doing animations on them

struct Sprite
{
	Font* _font;
};
typedef struct Sprite Sprite;

#ifdef __cplusplus
extern "C"
{
#endif
	Sprite* Sprite_Load(Font* font);

	short Sprite_GetWidth(Sprite* sprite);
	short Sprite_GetHeight(Sprite* sprite);

	void Sprite_Draw2D(Sprite* sprite, u16 spriteIndex, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY, Color4f* tintColor);

	void Sprite_Draw3D(Sprite* sprite, u16 spriteIndex, float scale, AlignmentModes alignX, AlignmentModes alignY, Color4f* tintColor);

	V3f Sprite_AdjustDrawingPosition(Sprite* sprite, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY);

	// TODO u16 CreateAnimation(u16 start, u16 stop, float frameTime);
	// TODO PlayAnimation(u16 index);
	// TODO ResetAnimation(u16 index);
	// TODO u16 CreateMegaSprite
	// TODO DrawMegaSprite(u16 megaIndex)

#ifdef __cplusplus
}
#endif
