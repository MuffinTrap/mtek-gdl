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

/**
 * @brief Creates the random number generator. This is done automatically on init.
 */
void Random_CreateGenerator(void);

/**
 * @brief Sets the random generation seed
 * @param seed The seed number
 */
void Random_SetSeed(u32 seed);
/**
 * @brief Returns random floating point number between given limits
 * @note Limits are inclusive
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @returns Randon number
 */
float Random_Float(float min, float max);

/**
 * @brief Returns a number between [0, 1.0]
 */
float Random_FloatNormalized(void);

/**
 * @brief Returns a random integer number between given limits
 * @note Limits are inclusive
 * @param min Minimum allowed value
 * @param max Maximum allowed value
 * @returns Random integer
 */
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

/**
 * @brief Calculates the aligned top left point
 * @details If you want to center image on (x,y), set both alignments to Center. Aligning to Left/Bottom does nothing. Aligning to right means that the right edge of image lies on the given x coordinate. Aligning to bottom means that the bottom edge of the image lies on the given y coordinate.
 * @param x Top left x coordinate
 * @param y Top left y coordinate
 * @param width Width to align
 * @param height to align
 * @param alignmentX How to align on X axis
 * @param alignmentY How to align on Y axis.
 * @returns New coordinates for top left point
 */
Vector2 mgdl_CalculateAlignedTopLeft(float x, float y, float width, float height, AlignmentModes alignmentX, AlignmentModes alignmentY);

/**
 * @return Buffer containging the buffered text, max size 255 chars.
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

#ifdef __cplusplus
}
#endif

// File system
bool mgdl_DoesFileExist(const char* filename);
bool mgdl_DoesFileExist(const zstr& filename);
