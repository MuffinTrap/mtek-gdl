#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-opengl.h>
#include <stdarg.h>
#include <string.h>

/**
 * @file mgdl-util.h
 * @ingroup core
 * @brief Helper functions and random number generation
 */


#ifdef __cplusplus
extern "C"
{
#endif

void Random_CreateGenerator(void);
void Random_SetSeed(u32 seed);
float Random_Float(float min, float max);
float Random_FloatNormalized(void);
int Random_Int(int min, int max);

u32 clampU32(u32 val, u32 minVal, u32 maxVal);
float clampF(float val, float minVal, float maxVal);
float minF(float a, float b);
float maxF(float a, float b);
u32 minU32(u32 a, u32 b);
u32 maxU32(u32 a, u32 b);

float Deg2Rad(float degrees);
float Rad2Deg(float radians);

/**
	* @brief Converts texture filter enumeration to OpenGL enumeration.
	* @param filterMode Filtering mode to convert.
	* @return OpenGL enumeration for the given format.
	*/
GLint TextureFilterToGLFilter(TextureFilterModes filterMode);


/**
 * @brief Tells if a flag is set.
 * @param bitfield The bitfield containing flags
 * @param flag The flag to inspect
 * @return True if the flag is set.
 */
bool Flag_IsSet(u32 bitfield, u32 flag);

/**
 * @brief Tells if all given flags are set
 * @param bitfield The bitfield containing flags
 * @param flag The flag to inspect
 * @return True if the flag is set.
 */
bool Flag_IsSetAll(u32 bitfield, u32 flags);

/**
 * @brief Sets a flag on bitfield
 * @param bitfield The bitfield containing flags
 * @param flag The flag to set
 * @return The modified bitfield
 */
u32 Flag_Set(u32 bitfield, u32 flag);

/**
 * @brief Sets all given flags on bitfield
 * @param bitfield The bitfield containing flags
 * @param flag The flags to set
 * @return The modified bitfield
 */
u32 Flag_SetAll(u32 bitfield, u32 flag);

/**
 * @brief Unsets a flag on bitfield
 * @param bitfield The bitfield containing flags
 * @param flag The flag to unset
 * @return The modified bitfield
 */
u32 Flag_Unset(u32 bitfield, u32 flag);
/**
 * @brief Unsets all given flags on bitfield
 * @param bitfield The bitfield containing flags
 * @param flag The flags to unset
 * @return The modified bitfield
 */
u32 Flag_UnsetAll(u32 bitfield, u32 flag);

/**
 * @return Buffer containging the text, max size 255 chars.
 */
char* mgdl_GetPrintfBuffer(void);

#ifdef __cplusplus
}
#endif

/**
 * @brief Writes the formatted text to a buffer.
 * @param format Format string.
 */
#define MGDL_PRINTF_TO_BUFFER(format) \
	memset(mgdl_GetPrintfBuffer(), '\0', 256);\
	va_list args;\
	va_start(args, format); \
	vsprintf(mgdl_GetPrintfBuffer(), format, args); \
	va_end(args);\



/*! @} */
