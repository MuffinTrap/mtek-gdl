#include <mgdl/mgdl-color.h>
#include <mgdl/mgdl-assert.h>

color32 Color_Create4f(float red, float green, float blue, float alpha)
{
    return Color_Create4b(
        (u8)(red*255.0f),
        (u8)(green*255.0f),
        (u8)(blue*255.0f),
        (u8)(alpha*255.0f));
}


color32 Color_Create4b(u8 red, u8 green, u8 blue, u8 alpha)
{
    color32 c = (red << 24) + (green << 16) + (blue << 8) + alpha;
	return c;
}


RGBAf Color_HexToFloats(u32 color)
{
    RGBAf components = {0,0,0,0};
    components.red = (float)RED(color)/255.0f;
    components.green = (float)GREEN(color)/255.0f;
    components.blue = (float)BLUE(color)/255.0f;
    components.alpha = (float)ALPHA(color)/255.0f;
    return components;
}

void Color_HexToGLfloats(color32 color, GLfloat target4f[4])
{
    target4f[0]= (float)RED(color)/255.0f;
    target4f[1]= (float)GREEN(color)/255.0f;
    target4f[2]= (float)BLUE(color)/255.0f;
    target4f[3]= (float)ALPHA(color)/255.0f;
}
