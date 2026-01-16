#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-types.h>
#include <cstdlib>
#include <mgdl/wflcg/WFLCG_c.h>
#define _USE_MATH_DEFINES
#include <math.h>


// ****************************
// RANDOM GENERATOR FUNCTIONS
// ****************************
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

// ******************************
// MATH UTIL FUNCTIONS
// ******************************

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

// **************************************
// BITFIELD FUNCTIONS FOR FLAG PARAMETERS
// **************************************

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

// ******************************
// ENDIANNES HANDLING FUNCTION
// ******************************

void RevBytes(void *var, u32 size) {

	mgdl_assert_print(size <= sizeof(s64), "Cannot reverse more than 8 bytes");

	static u8 temp[sizeof(s64)];

	memcpy(temp, var, size);
	for(u32 i=0; i<size; i++) {
		((u8*)var)[i] = temp[(size-1)-i];
	}

}
