#pragma once

#include <mgdl/raymath/raymath.h>
// TODO How to efficiently store these?

#if defined(__cplusplus)
extern "C" {
#endif

	void InitVector2Factory(u32 amountStored);
	Vector2* Vector2Factory_Create(float x, float y);


#if defined(__cplusplus)
}
#endif
