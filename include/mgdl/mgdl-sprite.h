#pragma once

#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-color.h>

struct Texture;

/**
 * @file mgdl-sprite.h
 * @ingroup draw2d
 * @brief  Functions for drawing sprites from SpriteAtlas
 */

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief Sets up drawcall for drawing multiple instances of the same sprite
	 * @param sprite The sprite to draw
	 */
	void Texture_BeginSpriteBatch(Texture* texture);
	/**
	 * @brief Ends the drawcall after drawing multiple sprites
	 */
	void Texture_EndSpriteBatch(void);

	void Texture_DrawBatchedSprite(Texture* texture, u16 spriteIndex, short x, short y, float scale, Color4f* tintColor);

	void Texture_DrawSprite(Texture* texture, u16 spriteIndex, short x, short y, float scale, Color4f* tintColor);

	// TODO u16 CreateAnimation(u16 start, u16 stop, float frameTime);
	// TODO PlayAnimation(u16 index);
	// TODO ResetAnimation(u16 index);
	// TODO u16 CreateMegaSprite
	// TODO DrawMegaSprite(u16 megaIndex)

#ifdef __cplusplus
}
#endif
