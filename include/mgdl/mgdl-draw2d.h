#pragma once

/**
 * @file mgdl-draw2d.h
 * @brief Functions for basic 2D drawing operations.
 *
 * 2D drawing functions for debugging and ui elements and simple games.
 * These functions expect the ortographic projection be set
 */

#include "mgdl-types.h"
using namespace gdl;

/**
 * @brief Translates the origo to given point.
 *
 * @param x Origo x in screen units.
 * @param x Origo y in screen units.
 */
void TranslateTo(short x, short y);


/**
 * @brief Draws a box border.
 *
 * The box is drawn using GL_LINES so the width is 1 pixel by default
 *
 * @param x Upper left corner x in screen units.
 * @param y Upper left corner y in screen units.
 * @param x2 Lower right corner x in screen units.
 * @param y2 Lower right corner y in screen units.
 * @param color Color of the border
 */
void DrawBox(short x, short y, short x2, short y2, rgba8 color);

/**
 * @brief Draws a filled box.
 *
 * @param x Upper left corner x in screen units.
 * @param y Upper left corner y in screen units.
 * @param x2 Lower right corner x in screen units.
 * @param y2 Lower right corner y in screen units.
 * @param color Color of the box.
 */
void DrawBoxF(short x, short y, short x2, short y2, rgba8 color);

/**
 * @brief Draws a filled quad with freely positioned corners.
 *
 * Note that the points needs to be in correct relation to each other since the quad is drawn counter-clockwise.
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
void DrawQuad(short xtl, short ytl, short xbl, short ybl, short xbr, short ybr, short xtr, short ytr, rgba8 color);

/**
 * @brief Draws a line.
 *
 * @param x Start point x
 * @param y Start point y
 * @param x2 End point x
 * @param y2 End point y
 * @param color Color of the line.
 */
void DrawLine ( short int x, short int y, short int x2, short int y2, rgba8 color );

// TODO
/**
 * @brief Draws text using the default font.
 *
 * @param x Upper left x of first letter.
 * @param y Upper left y of first letter.
 * @param color Color of the text.
 * @param text Text to be drawn.
 */
void Print2D(short x, short y, rgba8 color, const char* text);
