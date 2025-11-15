#pragma once

// PI and other constants
// Does not work on windows
#ifdef __cplusplus
#   include <cmath>
#   include <cstdint>
#else
#   include <math.h>
#   include <stdint.h>
#   include <stdbool.h>
#endif

// Vector types
#include <mgdl/mgdl-vectorfunctions.h>

#include <mgdl/mgdl-opengl.h>

// For MacOS
#include <stddef.h>

#ifdef GEKKO
#   include <gctypes.h>
#else

/**
 * @file mgdl-types.h
 * @ingroup core
 * @brief Variable types and constants
 */

// Mimic ogc type names on PC platforms
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;

#endif

#ifdef MGDL_PLATFORM_WINDOWS
#   if defined(MGDL_WINDOWS_NATIVE)
        typedef size_t sizetype;
#   else
        typedef ssize_t sizetype;
#   endif 
const double M_PI = 3.14159265358979323846;
const double M_PI_2 = 1.57079632679489661923;
#else
typedef size_t sizetype;
#endif


/**
 * @brief Callback function type needed for platform initializing and rendering
 */
typedef void (*CallbackFunction)(void);

/**
 * @brief Input color format options
 *	@details Options for the gdl::Texture::ConvertRawImage() function when converting raw image data into a texture.
 *		These are not to be confused with gdl::TextureFormatModes which designates GX texture formats.
 */
enum ColorFormats {
    Gray,		//!< 8-bit grayscale (I8).
    GrayAlpha,	//!< 16-bit grayscale with alpha (I8A8).
    RGB,		//!< 24-bit true-color RGB (R8G8B8).
    RGBA,		//!< 32-bit true-color RGBA (R8G8B8A8).
};
typedef enum ColorFormats ColorFormats;

/**
 * @brief Texture wrap modes
 *
 * @details Wrapping modes for gdl::Texture::SetWrapMode().
 *
 * @note Wrapping modes other than gdl::Clamp require a texture resolution that is a power of two to work correctly.
 */
enum TextureWrapModes {
    Clamp,		//!< Clamped (no wrap).
    Repeat,	//!< Repeated wrap (only applicable to power of two textures).
    Mirror,	//!< Mirrored wrap (only applicable to power of two textures).
};
typedef enum TextureWrapModes TextureWrapModes;

/**
 * @brief Texture filter modes
 *
 * @details Filter modes for gdl::Texture and gdl::Image classes for creation and loading functions.
 *
 * @note Mipmapped filtering modes only work if the texture or image was created with gdl::Texture::CreateMipmapped(),
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
typedef enum TextureFilterModes TextureFilterModes;


enum TextureFlipModes {
    FlipNone = 0,
    FlipVertical = 1,
    FlipHorizontal = 2
};
typedef enum TextureFlipModes TextureFlipModes;

/**
 * @brief Material types
 *
 * @details Different types of materials for rendering meshes.
 */
enum MaterialType {
    Diffuse,    // Straightforward texture
    Matcap      // Mesh UV's are recalculated to sample from texture based on the camera view matrix
};
typedef enum MaterialType MaterialType;

/**
 * @brief Alignment modes
 *
 * @details Alignment modes for different drawing functions. For vertical (y) alignment the LJustify means top and RJustify means bottom
 */
enum AlignmentModes {
    Centered	= 0x7ff0,	/**< Centered. */
    PCentered	= 0x7ff1,	/**< Pixel centered (most noticable when image is zoomed in). */
    Pivot       = 0x7ff2,	/**< Aligned according to its pivot coordinate (only applicable to gdl::SpriteSet functions). */
    CPivot      = 0x7ff3,	/**< Aligned according to its pivot coordinate and pixel centered (only applicable to gdl::SpriteSet functions). */
    RJustify	= 0x7ff4,	/**< Right justified (also bottom justified when used for the Y pivot axis). */
    LJustify    = 0x7ff5    /**< Left justified (also top justified when used for the Y axis) The default alignment; */
};
typedef enum AlignmentModes AlignmentModes;

// Screen aspect ratios supported
enum ScreenAspect {
    ScreenAuto, // Use system default
    Screen16x9,
    Screen4x3
};
typedef enum ScreenAspect ScreenAspect;

// Flags for initializing the system. They can be combined
enum PlatformInitFlag
{
    FlagNone = 0x0,
    FlagPauseUntilA = 0x01, // Enters a loop after system init and continues when A button is pressed
    FlagFullScreen = 0x02, // Start in full screen mode
    FlagSplashScreen = 0x04, // Show splash screen with logo. If FlagPauseUntilA is set will stay in splash screen
};
typedef enum PlatformInitFlag PlatformInitFlag;

// Debug font contains these glyphs
enum IconSymbol
{
    Icon_Dot = 0x7f,
    Icon_FaceInvert = 0x80,
    Icon_Face,
    Icon_ArrowUp,
    Icon_TriangleUp,
    Icon_ScrollArrow,
    Icon_Printer,
    Icon_Skull,
    Icon_LightningBolt,
    Icon_Notes,
    Icon_Sparkle,
    Icon_Key,
    Icon_Chevrons,
    Icon_Alien,
    Icon_Spiral,
    Icon_FloppyDisk,
    Icon_Folder,
    Icon_Lock,
    Icon_Bird,
    Icon_Clock,
    Icon_Ghost,
    Icon_Pill,
    Icon_SquareWave,
    Icon_SawWave,
    Icon_TriangleWave,
    Icon_Bottle,
    Icon_PartyLeben,
    Icon_WiFi,
    Icon_Popsicle,
    Icon_CursorPoint, // : DiagonalFill
    Icon_CursorBase,
    Icon_NekoEar, // CursorWing: These are the same. Rotate Ear 90 right to get cursor
    Icon_NekoFace,
    IconSymbol_Count,

    Icon_DiagonalFill = Icon_CursorPoint,
    Icon_CursorWing = Icon_NekoEar
};
typedef enum IconSymbol IconSymbol;


enum MeshAttributeFlags
{
    FlagNormals = 1,
    FlagUVs = 2,
    FlagColors = 4
};
typedef enum MeshAttributeFlags MeshAttributeFlags;

enum CameraMode
{
    CameraTarget = 0,
    CameraDirection = 1
};
typedef enum CameraMode CameraMode;

// Windows uses Rectangle
struct Rect
{
    short x;
    short y;
    short w;
    short h;
};
typedef struct Rect Rect;

// Windows uses Rectangle
struct RectF
{
    float x;
    float y;
    float w;
    float h;
};
typedef struct RectF RectF;

