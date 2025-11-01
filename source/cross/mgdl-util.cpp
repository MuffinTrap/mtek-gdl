#include <mgdl/mgdl-util.h>
#include <cstdlib>
#include <mgdl/wflcg/WFLCG_c.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef MGDL_PLATFORM_WINDOWS
    // Windows does not find M_PI in math.h
    #ifndef M_PI
        #define M_PI  3.14159265358979323846
    #endif
#endif

static WFLCG_c randomGenerator;


void Random_CreateGenerator(void)
{
	WFLCG_c_init_default(&randomGenerator);
}

void Random_SetSeed(u32 seed)
{
	WFLCG_c_init_1_seed(&randomGenerator, seed);
}

float Random_Float(float min, float max)
{
    float range = max - min;
	// The wflcg returns [1.0f, 2.0f]
	return min + range * (WFLCG_c_get_float(&randomGenerator) -1.0f);
}
float Random_FloatNormalized()
{
	return Random_Float(0.0f, 1.0f);
}

int Random_Int(int min, int max)
{
    int range = max - min;
	if (range == 0) { return 0;}
	return min + WFLCG_c_get_value(&randomGenerator) % range;
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

static char printfBuffer[MGDL_PRINTF_BUFFER_SIZE];

char* mgdl_GetPrintfBuffer()
{
	return printfBuffer;
}

bool Flag_IsSet(u32 bitfield, u32 flag)
{
	return (bitfield & flag) != 0;
}

bool Flag_IsSetAll(u32 bitfield, u32 flags)
{
	return (bitfield & flags) == flags;
}

u32 Flag_Set(u32 bitfield, u32 flag)
{
	return (bitfield | flag);
}

u32 Flag_SetAll(u32 bitfield, u32 flags)
{
	return (bitfield | flags);
}

u32 Flag_Unset(u32 bitfield, u32 flag)
{
	return (bitfield & ~flag);
}

u32 Flag_UnsetAll(u32 bitfield, u32 flags)
{
	return (bitfield & ~flags);
}
