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

/**
 * @brief Returns a number between [0, 1.0]
 */
float Random_FloatNormalized(void);
int Random_Int(int min, int max);

s16 clampS16(s16 val, s16 minVal, s16 maxVal);
s32 clampS32(s32 val, s32 minVal, s32 maxVal);
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
	* @brief Converts texture wrap enumeration to OpenGL enumeration.
	* @param wrapMode Wrap mode to convert.
	* @return OpenGL enumeration for the given format.
	*/
GLint TextureWrapToGLWrap(TextureWrapModes wrapMode);


/**
 * @brief Tells if any of the bits are set
 * @param bitfield The bitfield
 * @param flag The bits to check
 * @return True if any of the bits is set.
 */
bool Flag_IsSetAny(u32 bitfield, u32 bits) ;

/**
 * @brief Tells if all given bits are set
 * @param bitfield The bitfield
 * @param flag The bits to check
 * @return True if the bits are all set
 */
bool Flag_IsSetAll(u32 bitfield, u32 bits);

/**
 * @brief Sets all given bits
 * @param bitfield The bitfield
 * @param flag The bits to set
 * @return The modified bitfield
 */
u32 Flag_SetAll(u32 bitfield, u32 bits);

/**
 * @brief Unsets all given bits on bitfield
 * @param bitfield The bitfield
 * @param flag The bits to unset
 * @return The modified bitfield
 */
u32 Flag_UnsetAll(u32 bitfield, u32 bits);

/**
 * @brief Tells if bit at given index is set
 * @param bitfield The bitfield
 * @param bitIndex The index of the bit to inspect
 * @returns True if the bit is set
 */
bool Flag_IsBitSet(u32 bitfield, u8 bitIndex);
/**
 * @brief Sets a bit in a bitfield
 * @param bitfield The bitfield
 * @param bitIndex The index of the bit to set
 * @returns The modified bitfield
 */
u32 Flag_SetBit(u32 bitfield, u8 bitIndex);
/**
 * @brief Unset a bit in a bitfield
 * @param bitfield The bitfield
 * @param bitIndex The index of the bit to unset
 * @returns The modified bitfield
 */
u32 Flag_UnsetBit(u32 bitfield, u8 bitIndex);

Vector2 mgdl_CalculateAlignedTopLeft(float x, float y, float width, float height, AlignmentModes alignmentX, AlignmentModes alignmentY);

/**
 * @return Buffer containging the text, max size 255 chars.
 */
char* mgdl_GetPrintfBuffer(void);

/**
 * @brief Reverses the byte order of a variable.
 * @details Reverses the byte order of 16-bit and 32-bit variables useful when handling little-endian binary data.
 *
 * @param[in,out]	*var    Pointer to variable to reverse byte order.
 * @param[in]		size    Size of variable in bytes (for convenience, use sizeof()).
 */
void RevBytes(void *var, int size);

#ifdef __cplusplus
}
#endif

#define MGDL_PRINTF_BUFFER_SIZE 256
/**
 * @brief Writes the formatted text to a buffer.
 * @param format Format string.
 */
#define MGDL_PRINTF_TO_BUFFER(format) \
	memset(mgdl_GetPrintfBuffer(), '\0', MGDL_PRINTF_BUFFER_SIZE);\
	va_list args;\
	va_start(args, format); \
	vsnprintf(mgdl_GetPrintfBuffer(), MGDL_PRINTF_BUFFER_SIZE, format, args); \
	va_end(args);\


char* mgdl_BufferPrintf(const char* format, ...);

/*! @} */
