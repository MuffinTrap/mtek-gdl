#pragma once

#include "mgdl-types.h"
#include <mgdl/mgdl-opengl.h>
#include <stdarg.h>
// helper functions

namespace gdl
{
    struct RGBA8Components {
        u8 red;
        u8 green;
        u8 blue;
        u8 alpha;
    };

	struct RGBA8Floats {
		float red;
		float green;
		float blue;
		float alpha;
	};

    RGBA8Floats ColorToFloats(u32 color);
    RGBA8Components ColorToComponents(u32 color);
    u32 ComponentsToColor(RGBA8Components components);

    float GetRandomFloat(float min = 0.0f, float max = 1.0f);
    int GetRandomInt(int min, int max);

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
	GLint TextureFilterToGLFilter(gdl::TextureFilterModes filterMode);

};

/**
 * @brief Tells if a flag is set.
 * @param bitfield The bitfield containing flags
 * @param flag The flag to inspect
 * @return True if the flag is set.
 */
bool mgdl_IsFlagSet(u32 bitfield, u32 flag)
{
	return (bitfield & flag) != 0;
}

/**
 * @return Buffer containging the text, max size 255 chars.
 */
char* mgdl_GetPrintfBuffer();

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


/*! \addtogroup colorMacros Color Handling Macros
 *	\brief Macros for handling 32-bit color values.
 *	@{
 */

#define RED(c)		(((c)>>24)&0xFF)	/*!< Gets the red component intensity from a 32-bit color value.
										 *	 \param[in] c 32-bit RGBA color value
										 *	 \return Red component of value
										 */

#define GREEN(c)	(((c)>>16)&0xFF)	/*!< Gets the green component intensity from a 32-bit color value.
										 *	 \param[in] c 32-bit RGBA color value
										 *	 \return Green component of value
										 */

#define BLUE(c) 	(((c)>>8)&0xFF)		/*!< Gets the blue component intensity from a 32-bit color value
										 *	 \param[in] c 32-bit RGBA color value
										 *	 \return Blue component of value
										 */

#define ALPHA(c)	((c)		&0xFF)	/*!< Gets the alpha component intensity from a 32-bit color value.
										 *	 \param[in] c 32-bit RGBA color value
										 *	 \return Alpha component of value
										 */

#define TO_RGBA(r,g,b,a)	((u32)((((u32)(r))<<24) | \
						((((u32)(g))&0xFF)<<16) | \
						((((u32)(b))&0xFF)<<8) | \
						(((u32)(a))&0xFF)))
										/*!< Constructs a 32-bit RGBA color value.
										 *	 \param[in] r Red component
										 *	 \param[in] g Green component
										 *	 \param[in] b Blue component
										 *	 \param[in] a Alpha component
										 *	 \return 32-bit color value
										 */

#define TO_IA(i,a) (i&0xff)|((a&0xff)<<8)	/*!< Constructs a 16-bit intensity with alpha value.
										 *	 \param[in] i Intensity component
										 *	 \param[in] a Alpha component
										 *	 \return 16-bit intensity with alpha value
										 */

/*! @} */
