#pragma once

#include "mgdl-types.h"

// 2D drawing functions for debugging and ui elements
// and simple games
namespace gdl
{
	void TranslateTo(short x, short y);
	void DrawBox(short x, short y, short x2, short y2, u32 color);
	void DrawBoxF(short x, short y, short x2, short y2, u32 color);
	void Print2D(short x, short y,  u32 color, const char* text);
}
