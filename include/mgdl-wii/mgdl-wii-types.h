/*! \file mgdl-types.h
 *	\brief Library types, macros, defines and enums header
 */

#ifndef _GDL_WII_TYPES_H
#define _GDL_WII_TYPES_H

#include <gccore.h>


#ifndef GDL_PLATFORM_WII
//!< Macro for checking what port of the library is used
#define GDL_PLATFORM_WII
#endif


#ifndef PI
#define PI      3.141592653589793f	/*!< PI value. */
#endif

#ifndef ROTPI
#define ROTPI   PI/180				/*!< PI value for degree to radian conversion. */
#endif




//! Library namespace
namespace gdl {

//! For the error callback
typedef void (*ErrorCallback)(const char *message);

//! For the internal screen image capture function pointer
typedef bool (ImageCapFunc)(const char *fileName);

typedef void (InputPollFunc)(void);


//! Wii specific functions, structs and global variables.
/*!
 *	\note Be wary of this if you plan to make your game or engine cross-platform compatible as these objects/variables
 *	    are not available in the other ports. Use the GDL_PLATFORM_WII macro to detect which port of the library
 *	    was used to compile your project.
 */
namespace wii {


    //! 2D vertex coordinate.
    /*!
     *	A generic 2D short vector struct used to handle 2D vertices in this library.
     */
    typedef struct {
        s16 x;
        s16 y;
    } VERT2s16;


    //! UV texture coordinate.
    /*!
     *	Generic floating-point texture coordinate struct used to handle texcoords in this library.
     */
    typedef struct {
        f32	u;
        f32 v;
    } TEX2f32;


    //! 3D vertex coordinate.
    /*!
     *	Generic 3D float vector object not used by the library for now... Kept for convenience reasons in 3D prototyping.
     */
    typedef struct {
        f32 x;
        f32 y;
        f32 z;
    } VERT3f32;


};


}


#endif // _GDL_WII_TYPES_H
