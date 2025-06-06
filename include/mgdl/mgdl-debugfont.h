#pragma once

/**
 * @file mgdl-debugfont.h
 * @brief Function for getting the Debug font.
 */

#include <mgdl/mgdl-font.h>

namespace gdl
{
	/**
	 * @brief Returns pointer to the Debug Font which is always loaded.
	 *
	 * @return Pointer to the Debug Font.
	 */
	gdl::Font* GetDebugFont();
}
