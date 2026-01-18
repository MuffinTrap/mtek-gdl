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
 * @brief Creates a rectangle struct
 * @param x Top left corner x
 * @param y Top left corner y
 * @param w Width of rectangle
 * @param h Height of rectangle
 * @return Created rectangle
 */
Rect Rect_Create(short x, short y, short w, short h);

/**
 * @brief Creates a rectangle struct
 * @param position Top left corner
 * @param size Size of rectangle: x is width, y is height
 * @return Created rectangle
 */
Rect Rect_CreateV2f(vec2 position, vec2 size);

/**
 * @brief Translates the origo to given point.
 *
 * @param x Origo x in screen units.
 * @param x Origo y in screen units.
 */
void Draw2D_OrigoTo(short x, short y);
void Draw2D_OrigoToV2f(vec2 origo);


/**
 * @brief Draws a box border.
 *
 * @details The box is drawn using GL_LINES so the width is 1 pixel by default
 *
 * @param x Upper left corner x in screen units.
 * @param y Upper left corner y in screen units.
 * @param x2 Lower right corner x in screen units.
 * @param y2 Lower right corner y in screen units.
 * @param color Color of the border
 */
void Draw2D_RectLines(short x, short y, short x2, short y2, Color4f* color);
void Draw2D_RectLinesWH(short x, short y, short w, short h, Color4f* color);
void Draw2D_RectLinesRec(Rect rect, Color4f* color);

/**
 * @brief Draws a filled box.
 *
 * @param x Upper left corner x in screen units.
 * @param y Upper left corner y in screen units.
 * @param x2 Lower right corner x in screen units.
 * @param y2 Lower right corner y in screen units.
 * @param color Color of the box.
 */
void Draw2D_Rect(short x, short y, short x2, short y2, Color4f* color);

/**
 * @brief Draws a filled box.
 *
 * @param x Upper left corner x in screen units.
 * @param y Upper left corner y in screen units.
 * @param w Width of box
 * @param h Height of box
 * @param color Color of the box.
 */
void Draw2D_RectWH(short x, short y, short w, short h, Color4f* color);
void Draw2D_RectR(Rect rect, Color4f* color);

/**
 * @brief Draws a filled quad with freely positioned corners.
 *
 * @note The points needs to be in correct relation to each other since the quad is drawn counter-clockwise.
 *
 * @param xtl Upper left corner x in screen units.
 * @param ytl Upper left corner y in screen units.
 * @param xbl Lower left corner x in screen units.
 * @param ybl Lower left corner y in screen units.
 * @param xbr Lower right corner x in screen units.
 * @param ybr Lower right corner y in screen units.
 * @param xtr Upper right corner x in screen units.
 * @param ytr Upper right corner y in screen units.
 * @param color Color of the quad.
 */
void Draw2D_Quad(short xtl, short ytl, short xbl, short ybl, short xbr, short ybr, short xtr, short ytr, Color4f* color);
void Draw2D_QuadV2f(vec2 tl, vec2 bl, vec2 br, vec2 tr, Color4f* color);

/**
 * @brief Draws a line.
 *
 * @param x Start point x
 * @param y Start point y
 * @param x2 End point x
 * @param y2 End point y
 * @param color Color of the line.
 */
void Draw2D_Line( short int x, short int y, short int x2, short int y2, Color4f* color );

/**
 * @brief Draws a line.
 *
 * @param start Start point
 * @param end Start point
 * @param color Color of the line.
 */
void Draw2D_LineV2f(vec2 start, vec2 end, Color4f* color );

/**
 * @brief Draws text using the default font.
 *
 * @param x Upper left x of first letter.
 * @param y Upper left y of first letter.
 * @param text Text to be drawn.
 * @param color Color of the text.
 */
void Draw2D_Text(short x, short y, const char* text, Color4f* color);

/**
 * @brief Draws text using the default font.
 *
 * @param position Upper left corner of first letter
 * @param text Text to be drawn.
 * @param color Color of the text.
 */
void Draw2D_TextV2f(vec2 position, const char* text, Color4f* color);


#ifdef __cplusplus
}
#endif
