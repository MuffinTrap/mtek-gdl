#include "mgdl-wii/mgdl-util.h"
#include "mgdl-wii/mgdl-types.h"

gdl::RGBA8Components gdl::ColorToComponents(u_int color)
{
    RGBA8Components components = {0,0,0,0};
    components.red = RED(color);
    components.green = GREEN(color);
    components.blue = BLUE(color);
    components.alpha = ALPHA(color);
    return components;
}
u_int gdl::ComponentsToColor(gdl::RGBA8Components components)
{
    u_int color = RGBA(
    components.red,
    components.green, 
    components.blue,
    components.alpha);
    return color;
}
