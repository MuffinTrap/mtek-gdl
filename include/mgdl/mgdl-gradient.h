
#ifndef GRADIENT_H
#define GRADIENT_H
// Refers to colors in color manager and has 2 or more stops
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-color.h>

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
    Color4f colors[GRADIENT_SIZE];
    float stops[GRADIENT_SIZE];
    short color_amount;
    float alpha;
    float repeats;
    enum GradientShape shape;
    enum GradientLoopMode loop_mode;
};

struct Gradient Gradient_CreateEmpty(enum GradientShape shape, enum GradientLoopMode loop_mode);
void Gradient_PushColor(struct Gradient* gradient, Color4f* color, float stop);

void Gradient_PushColorArray(struct Gradient* gradient, Color4f* colors, short amount);

void Gradient_glColor(struct Gradient* gradient, float stop);
void Gradient_glColorA(struct Gradient* gradient, float stop, float alpha);

Color4f Gradient_GetColor(struct Gradient* gradient, float stop);

#endif
