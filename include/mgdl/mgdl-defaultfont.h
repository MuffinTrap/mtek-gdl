#pragma once

/**
 * @file mgdl-defaultfont.h
 * @ingroup asset
 * @brief Function for getting the Default font.
 */

struct Texture;


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Returns pointer to a Texture with SpriteAtlas mapped for the default font.
 * @return Pointer to the Default Font.
 */
Texture* DefaultFont_GetDefaultFont(void);

#ifdef __cplusplus
}
#endif
