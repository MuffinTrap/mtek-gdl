#include <mgdl/mgdl-color.h>

Color4f Color_Create4f(float red, float green, float blue, float alpha)
{
    Color4f color = {red, green, blue, alpha};
	return color;
}

Color4f Color_CreateFromPointer4f(Color4f* color)
{
    Color4f copy = {color->red, color->green, color->blue, color->alpha};
	return copy;
}

Color4b Color_Create4b(u8 red, u8 green, u8 blue, u8 alpha)
{
    Color4b color = {red, green, blue, alpha};
	return color;
}

Color4b Color_HexToBytes(u32 color)
{
    Color4b components = {0,0,0,0};
    components.red = RED(color);
    components.green = GREEN(color);
    components.blue = BLUE(color);
    components.alpha = ALPHA(color);
    return components;
}
u32 Color_FloatsToHex(Color4f components)
{
    u32 color = TO_RGBA(
    components.red,
    components.green,
    components.blue,
    components.alpha);
    return color;
}

Color4f Color_HexToFloats(u32 color)
{
    Color4f components = {0,0,0,0};
    components.red = (float)RED(color)/255.0f;
    components.green = (float)GREEN(color)/255.0f;
    components.blue = (float)BLUE(color)/255.0f;
    components.alpha = (float)ALPHA(color)/255.0f;
    return components;
}

Color4f* Color_GetDefaultColor(DefaultColor color)
{
    static Color4f white =  Color_Create4f(1.0f, 1.0f, 1.0f, 1.0f);
    static Color4f black =  Color_Create4f(0.0f, 0.0f, 0.0f, 1.0f);
    static Color4f red =    Color_Create4f(1.0f, 0.0f, 0.0f, 1.0f);
    static Color4f green =  Color_Create4f(0.0f, 1.0f, 0.0f, 1.0f);
    static Color4f blue =   Color_Create4f(0.0f, 0.0f, 1.0f, 1.0f);
    switch(color)
    {
        case Color_White: return &white; break;
        case Color_Black: return &black; break;
        case Color_Red: return &red; break;
        case Color_Green: return &green; break;
        case Color_Blue: return &blue; break;
        default: return &white; break;
    }
}
