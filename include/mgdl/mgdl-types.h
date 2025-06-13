#pragma once

// PI and other constants
// Does not work on windows
#ifdef __cplusplus
#include <cmath>
#include <cstdint>
#else
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#endif

// Vector types
#include <mgdl/mgdl-vectorfunctions.h>

// For MacOS
#include <stddef.h>

#ifdef GEKKO

#include <gctypes.h>

#else

// Mimic ogc type names on PC platforms
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;

#endif

#ifdef MGDL_PLATFORM_WINDOWS
typedef ssize_t sizetype;
const double M_PI = 3.14159265358979323846;
const double M_PI_2 = 1.57079632679489661923;
#else
typedef size_t sizetype;
#endif


/**
 * @brief Callback function type needed for platform initializing
 */
typedef void (*CallbackFunction)(void);

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
typedef enum ColorFormats ColorFormats;

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
typedef enum TextureWrapModes TextureWrapModes;

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
typedef enum TextureFilterModes TextureFilterModes;

/*! Material types
 *
 *	\details Different types of materials for rendering meshes.
 *
 *	\note Mipmapped filtering modes only work if the texture or image was created with gdl::Texture::CreateMipmapped(),
 *		gdl::Image::CreateMipmapped() and gdl::Image::LoadImageMipmapped() and must be specified to the minFilt parameter
 *		of said functions to work.
 */
enum MaterialType {
    Diffuse,    // Straightforward texture
    Matcap      // Mesh UV's are recalculated to sample from texture based on the camera view matrix
};
typedef enum MaterialType MaterialType;

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
typedef enum AlignmentModes AlignmentModes;

// Screen aspect ratios supported
enum ScreenAspect {
    ScreenAuto = 0, // Use system default
    Screen16x9,
    Screen4x3
};
typedef enum ScreenAspect ScreenAspect;

// Flags for initializing the system. They can be combined
enum PlatformInitFlag
{
    FlagPauseUntilA = 0x01, // Enters a loop after system init and continues when A button is pressed
    FlagFullScreen = 0x02, // Start in full screen mode
    FlagSplashScreen = 0x04, // Show splash screen with logo. If FlagPauseUntilA is set will stay in splash screen
};
typedef enum PlatformInitFlag PlatformInitFlag;

/*!	Color values
 *
 *	\details Preset colors for prototyping and convenience reasons.
 *
 *	\note For a better method of selecting colors, use the RGBA() macro instead.
 */
enum Colors : u32 { // RRGGBBAA
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
typedef enum Colors Colors;

// Color in four bytes, each from 0 to 255(FF)
typedef u32 rgba8;

// Debug font contains these glyphs
enum IconSymbol : short
{
    FaceInvert = 0x80,
    Face,
    ArrowUp,
    TriangleUp,
    ScrollArrow,
    Printer,
    Skull,
    LightningBolt,
    Notes,
    Sparkle,
    Key,
    Chevrons,
    Alien,
    Spiral,
    FloppyDisk,
    Folder,
    Lock,
    Bird,
    Clock,
    Ghost,
    Pill,
    SquareWave,
    SawWave,
    TriangleWave,
    Bottle,
    PartyLeben,
    WiFi,
    Popsicle,
    CursorPoint, // : DiagonalFill
    CursorBase,
    NekoEar, // CursorWing: These are the same. Rotate Ear 90 right to get cursor
    NekoFace,
    IconSymbol_Count,

    DiagonalFill = CursorPoint,
    CursorWing = NekoEar
};
typedef enum IconSymbol IconSymbol;

struct Color4f
{
    float red;
    float green;
    float blue;
    float alpha;
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

typedef u32 ColorHex;

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

