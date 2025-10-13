#pragma once

/**
 * @file mgdl-defaultfont.h
 * @ingroup asset
 * @brief Function for getting the Default font.
 */

#include <mgdl/mgdl-font.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Returns pointer to the Font which is always loaded.
 *
 * @return Pointer to the Default Font.
 */
Font* DefaultFont_GetDefaultFont(void);

#ifdef __cplusplus
}
#endif
