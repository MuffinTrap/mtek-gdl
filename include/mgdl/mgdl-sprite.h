#pragma once

#include "mgdl-font.h"

// This class is almost the same as the font class, but supports
// combining sprites and doing animations on them
using namespace gdl;

struct Sprite
{
	Font* _font;
};

extern "C"
{
	Sprite* Sprite_Load(Font* font);

	short Sprite_GetWidth(Sprite* sprite);
	short Sprite_GetHeight(Sprite* sprite);

	void Sprite_Draw2D(Sprite* sprite, u16 spriteIndex, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor);

	void Sprite_Draw3D(Sprite* sprite, u16 spriteIndex, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor);

	vec3 Sprite_AdjustDrawingPosition(Sprite* sprite, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);

	// TODO u16 CreateAnimation(u16 start, u16 stop, float frameTime);
	// TODO PlayAnimation(u16 index);
	// TODO ResetAnimation(u16 index);
	// TODO u16 CreateMegaSprite
	// TODO DrawMegaSprite(u16 megaIndex)
}
