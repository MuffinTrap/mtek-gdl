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

struct Color4f
{
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
};
typedef struct Color4f Color4f;

struct Color4b
{
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};
typedef struct Color4b Color4b;

enum DefaultColor
{
	Color_White,
	Color_Black,
	Color_Red,
	Color_Green,
	Color_Blue
};
typedef enum DefaultColor DefaultColor;

#ifdef __cplusplus
extern "C"
{
#endif

Color4f Color_Create4f(float red, float green, float blue, float alpha);
Color4f Color_CreateFromPointer4f(Color4f* color);
Color4b Color_Create4b(u8 red, u8 green, u8 blue, u8 alpha);
Color4f Color_HexToFloats(u32 color);
Color4b Color_HexToBytes(u32 color);
u32 Color_FloatsToHex(Color4f components);

// Debugging colors
Color4f* Color_GetDefaultColor(DefaultColor color);

#ifdef __cplusplus
}
#endif
