#include <mgdl/mgdl-util.h>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef MGDL_PLATFORM_WINDOWS
    // Windows does not find M_PI in math.h
    #ifndef M_PI
        #define M_PI  3.14159265358979323846
    #endif
#endif

gdl::RGBA8Components gdl::ColorToComponents(u32 color)
{
    RGBA8Components components = {0,0,0,0};
    components.red = RED(color);
    components.green = GREEN(color);
    components.blue = BLUE(color);
    components.alpha = ALPHA(color);
    return components;
}
u32 gdl::ComponentsToColor(gdl::RGBA8Components components)
{
    u32 color = TO_RGBA(
    components.red,
    components.green,
    components.blue,
    components.alpha);
    return color;
}

gdl::RGBA8Floats gdl::ColorToFloats(u32 color)
{
    RGBA8Floats components = {0,0,0,0};
    components.red = (float)RED(color)/255.0f;
    components.green = (float)GREEN(color)/255.0f;
    components.blue = (float)BLUE(color)/255.0f;
    components.alpha = (float)ALPHA(color)/255.0f;
    return components;
}

float gdl::GetRandomFloat(float min, float max)
{
    float range = max - min;
	return min + range * ((float)rand()/(float)RAND_MAX);
}

int gdl::GetRandomInt(int min, int max)
{
    int range = max - min;
	return min + rand()%range;
}

u32 gdl::clampU32(u32 val, u32 minVal, u32 maxVal)
{
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;
	return val;
}

float gdl::clampF(float val, float minVal, float maxVal)
{
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;
	return val;
}

float gdl::minF(float a, float b)
{
	return (a < b ? a : b);
}
float gdl::maxF(float a, float b)
{
	return (a > b ? a : b);
}
u32 gdl::minU32(u32 a, u32 b)
{
	return (a < b ? a : b);
}
u32 gdl::maxU32(u32 a, u32 b)
{
	return (a > b ? a : b);
}

float gdl::Deg2Rad(float degrees)
{
    return (float)M_PI * degrees/180.0f;
}

float gdl::Rad2Deg(float radians)
{
    return 180.0f * radians/(float)M_PI;
}

GLint gdl::TextureFilterToGLFilter(gdl::TextureFilterModes filterMode)
{
	switch(filterMode)
	{
		case gdl::TextureFilterModes::Linear:
			return GL_LINEAR;
			break;
		case gdl::TextureFilterModes::Nearest:
			return GL_NEAREST;
			break;
		case gdl::TextureFilterModes::LN_MM_LN:
			return GL_LINEAR_MIPMAP_LINEAR;
			break;
		case gdl::TextureFilterModes::LN_MM_NR:
			return GL_LINEAR_MIPMAP_NEAREST;
			break;
		case gdl::TextureFilterModes::NR_MM_LN:
			return GL_NEAREST_MIPMAP_LINEAR;
			break;
		case gdl::TextureFilterModes::NR_MM_NR:
			return GL_NEAREST_MIPMAP_NEAREST;
			break;
		default:
			return GL_LINEAR;
			break;
	};
}

static char printfBuffer[256];

char* mgdl_GetPrintfBuffer()
{
	return printfBuffer;
}

bool mgdl_IsFlagSet(u32 bitfield, u32 flag)
{
	return (bitfield & flag) != 0;
}
