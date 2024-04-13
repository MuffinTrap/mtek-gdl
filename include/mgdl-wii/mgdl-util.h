#pragma once
/*! \file mgdl-util.h
 *	\brief Collection of utility functions
 */

/*!	\mainpage
 *
 *	\version	0.01
 *	\author		muffintrap
 *	\copyright	GNU General Public License
 *
 */
#include <gccore.h>

 namespace gdl
 {
    typedef struct {
        u_char red;
        u_char green;
        u_char blue;
        u_char alpha;
    }RGBA8Components ;

    RGBA8Components ColorToComponents(u_int color);
    u_int ComponentsToColor(RGBA8Components components);
 };