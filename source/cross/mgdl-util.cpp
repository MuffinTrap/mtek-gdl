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

Color4b ColorToComponents(u32 color)
{
    Color4b components = {0,0,0,0};
    components.red = RED(color);
    components.green = GREEN(color);
    components.blue = BLUE(color);
    components.alpha = ALPHA(color);
    return components;
}
u32 ComponentsToColor(Color4f components)
{
    u32 color = TO_RGBA(
    components.red,
    components.green,
    components.blue,
    components.alpha);
    return color;
}

Color4f ColorToFloats(u32 color)
{
    Color4f components = {0,0,0,0};
    components.red = (float)RED(color)/255.0f;
    components.green = (float)GREEN(color)/255.0f;
    components.blue = (float)BLUE(color)/255.0f;
    components.alpha = (float)ALPHA(color)/255.0f;
    return components;
}

float GetRandomFloat(float min, float max)
{
    float range = max - min;
	return min + range * ((float)rand()/(float)RAND_MAX);
}
float GetRandomFloatNormal()
{
	return GetRandomFloat(0.0f, 1.0f);
}

int GetRandomInt(int min, int max)
{
    int range = max - min;
	if (range == 0) { return 0;}
	return min + rand()%range;
}

u32 clampU32(u32 val, u32 minVal, u32 maxVal)
{
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;
	return val;
}

float clampF(float val, float minVal, float maxVal)
{
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;
	return val;
}

float minF(float a, float b)
{
	return (a < b ? a : b);
}
float maxF(float a, float b)
{
	return (a > b ? a : b);
}
u32 minU32(u32 a, u32 b)
{
	return (a < b ? a : b);
}
u32 maxU32(u32 a, u32 b)
{
	return (a > b ? a : b);
}

float Deg2Rad(float degrees)
{
    return (float)M_PI * degrees/180.0f;
}

float Rad2Deg(float radians)
{
    return 180.0f * radians/(float)M_PI;
}

GLint TextureFilterToGLFilter(TextureFilterModes filterMode)
{
	switch(filterMode)
	{
		case TextureFilterModes::Linear:
			return GL_LINEAR;
			break;
		case TextureFilterModes::Nearest:
			return GL_NEAREST;
			break;
		case TextureFilterModes::LN_MM_LN:
			return GL_LINEAR_MIPMAP_LINEAR;
			break;
		case TextureFilterModes::LN_MM_NR:
			return GL_LINEAR_MIPMAP_NEAREST;
			break;
		case TextureFilterModes::NR_MM_LN:
			return GL_NEAREST_MIPMAP_LINEAR;
			break;
		case TextureFilterModes::NR_MM_NR:
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
