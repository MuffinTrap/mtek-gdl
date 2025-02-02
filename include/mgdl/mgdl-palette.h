#pragma once

#include "mgdl-types.h"

namespace gdl
{
	// A palette that can be filled with custom colors

	class Palette
	{
	public:
		static Palette* GetDefaultPalette();
		Palette(u32* colorsArray, u8 size);
		u32 GetColor(u8 index);
		u8 GetSize();
	private:
		u32* colors;
		u8 size;
	};
}
