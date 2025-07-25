#pragma once

#include <mgdl/mgdl-color.h>


#ifdef __cplusplus
extern "C"
{
#endif

	void SetSplashScreenColors(Color4f* bg, Color4f* textDim, Color4f* textLight);
	void SetSplashScreenDuration(float seconds);
	float DrawSplashScreen(float deltaTime, bool drawHoldAMessage, float aHoldTimer);

#ifdef __cplusplus
}
#endif
