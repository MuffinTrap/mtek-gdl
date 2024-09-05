#pragma once

// For MacOS
#include <stddef.h>

#ifdef GEKKO

    #include <gctypes.h>

#else

    // Mimic ogc type names
    #include <cstdint>
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef int16_t s16;
    typedef uint32_t u32;

#endif

namespace gdl
{
    /*! Input color format options
     *
     *	\details Options for the gdl::Texture::ConvertRawImage() function when converting raw image data into a texture.
     *		These are not to be confused with gdl::TextureFormatModes which designates GX texture formats.
     */
    enum ColorFormats {
        Gray,		//!< 8-bit grayscale (I8).
        GrayAlpha,	//!< 16-bit grayscale with alpha (I8A8).
        RGB,		//!< 24-bit true-color RGB (R8G8B8).
        RGBA,		//!< 32-bit true-color RGBA (R8G8B8A8).
    };

    /*! Texture wrap modes
     *
     *	\details Wrapping modes for gdl::Texture::SetWrapMode().
     *
     *	\note Wrapping modes other than gdl::Clamp require a texture resolution that is a power of two to work correctly.
     */
    enum TextureWrapModes {
        Clamp,		//!< Clamped (no wrap).
        Repeat,	//!< Repeated wrap (only applicable to power of two textures).
        Mirror,	//!< Mirrored wrap (only applicable to power of two textures).
    };

    /*! Texture filter modes
     *
     *	\details Filter modes for gdl::Texture and gdl::Image classes for creation and loading functions.
     *
     *	\note Mipmapped filtering modes only work if the texture or image was created with gdl::Texture::CreateMipmapped(),
     *		gdl::Image::CreateMipmapped() and gdl::Image::LoadImageMipmapped() and must be specified to the minFilt parameter
     *		of said functions to work.
     */
    enum TextureFilterModes {
        Nearest,			//!< Nearest-neighbor interpolation.
        Linear,		//!< Linear interpolation.
        NR_MM_NR,	//!< Near-mipmap-Near (texture must be mipmapped).
        LN_MM_NR,	//!< Linear-mipmap-Near (texture must be mipmapped).
        NR_MM_LN,	//!< Near-mipmap-Linear (texture must be mipmapped).
        LN_MM_LN,	//!< Linear-mipmap-Linear (texture must be mipmapped).
    };

    /*! Alignment modes
     *
     *	\details Alignment modes for gdl::Font, gdl::FFont, gdl::Image and gdl::SpriteSet drawing functions.
     *
     *	\note For gdl::Font and gdl::FFont functions, only gdl::Centered is applicable to said classes.
     */
    enum AlignmentModes {
        Centered	= 0x7ff0,	//!< Centered.
        PCentered	= 0x7ff1,	//!< Pixel centered (most noticable when image is zoomed in).
        Pivot       = 0x7ff2,	//!< Alligned according to its pivot coordinate (only applicable to gdl::SpriteSet functions).
        CPivot      = 0x7ff3,	//!< Alligned according to its pivot coordinate and pixel centered (only applicable to gdl::SpriteSet functions).
        RJustify	= 0x7ff4,	//!< Right justified (also bottom justified when used for the Y pivot axis).
        LJustify    = 0x7ff5    //!< Left justified (also top justified when used for the Y axis) The default alignment;
    };

    // Screen aspect ratios supported
    enum ScreenAspect {
        ScreenAuto = 0, // Use system default
        Screen16x9,
        Screen4x3
    };

    struct vec2
    {
        float x;
        float y;
    };

    /*!	Color values
     *
     *	\details Preset colors for prototyping and convenience reasons.
     *
     *	\note For a better method of selecting colors, use the RGBA() macro instead.
     */
    enum Colors { // RRGGBBAA
        Black		= 0x000000ff,	//!< Black.
        White		= 0xffffffff,	//!< White.
        Red			= 0xff0000ff,	//!< Red.
        Green		= 0x00ff00ff,	//!< Green.
        Blue		= 0x0000ffff,	//!< Blue.
        Yellow		= 0xffff00ff,	//!< Yellow.
        Cyan		= 0x00ffffff,	//!< Cyan.
        Magenta		= 0xff00ffff,	//!< Magenta.
        LightRed	= 0xff7f7fff,	//!< Light red.
        LightGreen	= 0x7fff7fff,	//!< Light green.
        LightBlue	= 0x7f7fffff,	//!< Light blue.
    };

    typedef u32 Color;
}



