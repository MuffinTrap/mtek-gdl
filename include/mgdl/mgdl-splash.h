#pragma once

#include <mgdl/mgdl-types.h>


#ifdef __cplusplus
extern "C"
{
#endif

	void SetSplashScreenColors(rgba8 bg, rgba8 textDim, rgba8 textLight);
	void SetSplashScreenDuration(float seconds);
	float DrawSplashScreen(float deltaTime, bool drawHoldAMessage, float aHoldTimer);

#ifdef __cplusplus
}
#endif
