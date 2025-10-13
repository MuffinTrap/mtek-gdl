#pragma once

#include <mgdl/mgdl-color.h>

/**
 * @file mgdl-splash.h
 * @ingroup core draw2d
 * @brief Functions to draw and set colors of the splash screen
 */


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
