#pragma once

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-types.h>


/** @file mgdl-color.h
* @ingroup core draw2d
* @brief Color struct and functions
*/

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

#define REDf(c) (float)RED(c)/255.0f
#define GREENf(c) (float)GREEN(c)/255.0f
#define BLUEf(c) (float)BLUE(c)/255.0f
#define ALPHAf(c) (float)ALPHA(c)/255.0f

typedef u32 color32;

struct RGBAf
{
	float red, green,blue, alpha;
};
typedef struct RGBAf RGBAf;


const color32 Debug_Black		= 0x000000FF;// 0 Black
const color32 Debug_DarkGray  	= 0x555555FF; // 1 Dark Gray
const color32 Debug_Blue			= 0x5555FFFF; // 2 Blue
const color32 Debug_Green		= 0x55FF55FF; // 3 Green
const color32 Debug_Red			= 0xFF5555FF; // 4 Red
const color32 Debug_Magenta		= 0xFF55FFFF; // 5 Magenta
const color32 Debug_Yellow		= 0xFFFF55FF; // 6 Yellow
const color32 Debug_White		= 0xFFFFFFFF;  // 7 White

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief Creates a new color from normalized values.
 * @param red Red channel value from 0 to 1
 * @param green Red channel value from 0 to 1
 * @param blue Red channel value from 0 to 1
 * @param alpha Alpha channel value from 0 to 1
 * @returns The color
 */
color32 Color_Create4f(float red, float green, float blue, float alpha);

/**
 * @brief Creates a new color from normalized values.
 * @param componentes Struct containing the components
 * @returns The color
 */
u32 Color_FloatsToHex(RGBAf components);

/**
 * @brief Creates a new color from byte values
 * @param red Red channel value from 0 to 255
 * @param green Red channel value from 0 to 255
 * @param blue Red channel value from 0 to 255
 * @param alpha Alpha channel value from 0 to 255
 * @returns The color
 */

color32 Color_Create4b(u8 red, u8 green, u8 blue, u8 alpha);
/**
 * @brief Decomposes a color to normalized components
 * @param color The color to decompose
 * @returns The struct with components
 */
RGBAf Color_HexToFloats(u32 color);

/**
 * @brief Decomposes a color to normalized components in target array
 * @param color The color to decompose
 * @param target4f Array of size 4 in order RGBA
 */
void Color_HexToGLfloats(color32 color, GLfloat* target4f);

#ifdef __cplusplus
}
#endif
