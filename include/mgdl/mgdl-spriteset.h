#pragma once

#include "mgdl-font.h"

namespace gdl
{
	// This class is almost the same as the font class, but supports
	// combining sprites and doing animations on them

	class SpriteSet
	{
	public:
		void LoadFromImage(const char* filename, short spriteWidth, short spriteHeight);
		short GetSpriteWidth();
		short GetSpriteHeight();

		void Draw2D(u16 spriteIndex, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor);
		void Draw3D(u16 spriteIndex, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY, u32 tintColor);

	private:
		Font* font;
		vec3 AdjustDrawingPosition(short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);

		// TODO u16 CreateAnimation(u16 start, u16 stop, float frameTime);
		// TODO PlayAnimation(u16 index);
		// TODO ResetAnimation(u16 index);
		// TODO u16 CreateMegaSprite
		// TODO DrawMegaSprite(u16 megaIndex)
	};
};
