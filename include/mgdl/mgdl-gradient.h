
#ifndef GRADIENT_H
#define GRADIENT_H
// Refers to colors in color manager and has 2 or more stops
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-color.h>

/**
 * @file mgdl-gradient.h
 * @brief Struct that represents a gradient made of colors at stop points
 */

enum GradientShape
{
    GradientVertical, // 0
    GradientRadial,   // 1
    GradientCircle      // 2
};

enum GradientLoopMode
{
    GradientLoopRepeat,
    GradientLoopMirror
};

#define GRADIENT_SIZE 12
struct Gradient
{
    color32 colors[GRADIENT_SIZE];
    float stops[GRADIENT_SIZE];
    short color_amount;
    float alpha;
    float repeats;
    enum GradientShape shape;
    enum GradientLoopMode loop_mode;
};

/**
 * @brief Creates an empty gradient
 * @param shape Shape of the gradient
 * @param loopMode  How the gradient is looped
 * @returns Gradient struct
 */
struct Gradient Gradient_CreateEmpty(enum GradientShape shape, enum GradientLoopMode loopMode);

/**
 * @brief Adds a color to the end of the gradient
 * @note You must take care that the colors are in the order of their stops
 * @param gradient The gradient
 * @param color What color to add
 * @param float The normalized position of the color.
 * @returns Gradient struct
 */
void Gradient_PushColor(struct Gradient* gradient, color32 color, float stop);

/**
 * @brief Add an array of colors to the gradient at even intervals
 * @param gradient The gradient
 * @param colors Array of colors
 * @param amount Size of the array
 */
void Gradient_PushColorArray(struct Gradient* gradient, color32* colors, u8 amount);

/**
 * @brief Sets a color from the gradient to OpenGL state
 * @param gradient The gradient
 * @param stop The sample point
 */
void Gradient_glColor(struct Gradient* gradient, float stop);
/**
 * @brief Sets a color from the gradient to OpenGL state with custom alpha value
 * @param gradient The gradient
 * @param stop The sample point
 * @param alpha The alpha value that overrides the sampled value
 */
void Gradient_glColorA(struct Gradient* gradient, float stop, float alpha);

/**
 * @brief Gets a color from the gradient at sample point
 * @param gradient The gradient
 * @param stop The sample point
 * @returns The color at sample point
 */
color32 Gradient_GetColor(struct Gradient* gradient, float stop);

#endif
