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

	/**
	 * @brief Set custom colors to use in splash screen
	 * @param bg Background color
	 * @param textDim Dim color of the text
	 * @param textLight Light color of the text
	 */
	void SetSplashScreenColors(color32 bg, color32 textDim, color32 textLight);
	/**
	 * @brief Set custom duration for the splash screen
	 */
	void SetSplashScreenDuration(float seconds);
	/**
	 * @brief Draws the splash screen
	 * @param deltaTime Delta time to use
	 * @param drawHoldAMessage Show that user has to hold A button
	 */
	float DrawSplashScreen(float deltaTime, bool drawHoldAMessage, float aHoldTimer);

#ifdef __cplusplus
}
#endif
