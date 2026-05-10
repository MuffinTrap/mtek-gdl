#pragma once

#include <mgdl/mgdl-color.h>
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-draw2d.h
 * @ingroup draw2d
 * @brief Functions for basic 2D drawing operations.
 *
 * 2D drawing functions for debugging and ui elements and simple games.
 * These functions expect the ortographic projection be set
 */


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Draws a box border.
 *
 * @details The box is drawn using GL_LINES so the width is 1 pixel by default
 *
 * @param x Top left corner x in screen units.
 * @param y Top left corner y in screen units.
 * @param w Width
 * @param h Height
 * @param color Color of the border
 */
void mgdl_DrawRectangle(float x, float y, float w, float h, color32 color);
void mgdl_DrawRectangleV(vec2 topleft, vec2 size, color32 color);
void mgdl_DrawRectangleLines(float x, float y, float w, float h, color32 color);
void mgdl_DrawRectangleLinesEx(RectF rec, float lineThickness, color32 color);

/**
 * @brief Draws a line.
 *
 * @param x Start point x
 * @param y Start point y
 * @param x2 End point x
 * @param y2 End point y
 * @param color Color of the line.
 */
void mgdl_DrawLine( float x, float y, float x2, float y2, color32 color );

/**
 * @brief Draws a line.
 *
 * @param start Start point
 * @param end Start point
 * @param color Color of the line.
 */
void mgdl_DrawLineV(vec2 start, vec2 end, color32 color );
void mgdl_DrawLineEx(vec2 start, vec2 end, float lineThickness, color32 color );

/**
 * @brief Draws text using the default font.
 *
 * @param x Upper left x of first letter.
 * @param y Upper left y of first letter.
 * @param text Text to be drawn.
 * @param color Color of the text.
 */
void mgdl_DrawText(float x, float y, const char* text, float fontSize, color32 color);

/**
 * @brief Draws text using the default font.
 *
 * @param position Upper left corner of first letter
 * @param text Text to be drawn.
 * @param color Color of the text.
 */
void mgdl_DrawTextV(vec2 topleft, const char* text, color32 color);


#ifdef __cplusplus
}
#endif
