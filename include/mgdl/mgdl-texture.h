#pragma once

#include "mgdl-types.h"
#include "mgdl-util.h"
#include "mgdl-opengl.h"
#include "mgdl-png.h"
#include "mgdl-spriteatlas.h"

/**
 * @file mgdl-texture.h
 * @ingroup asset draw2d draw3d
 * @brief Texture class and functions
 */

// Forward declares

/**
* @brief Holds pixels data and allows using them for drawing operations
*/
struct Texture
{
	PNGFile* pngFile;
	SpriteAtlas* spriteAtlas;

	float aspectRatio;
	GLsizei width;
	GLsizei height;
	ColorFormats colorFormat;
	GLuint textureId;
};
typedef struct Texture Texture;

#ifdef __cplusplus
extern "C"
{
#endif

	Texture* Texture_Create(void);
	void Texture_Destroy(Texture* texture);

	/**
		* @brief Sets the GL name and dimenions of the image.
		*
		* @note This is used when the image data has already been loaded using OpenGL.
		*
		* @param textureName GL name of the texture.
		* @param width Width of the image in pixels.
		* @param height Height of the image in pixels.
		* @param format Color format of the image.
		*/
	void Texture_SetGLName(Texture* img, GLuint textureName, GLsizei width, GLsizei height, ColorFormats format);

	/**
		* @brief Draws the image using 2D vertices.
		*
		* This function expects the ortographic projection to be set. This function preserves the aspect ratio.
		* @param x Lower left corner x of the image.
		* @param y Lower left corner y of the image.
		* @param scale Scaling of the image. Scale of 1.0 does not alter the size. Negative scale does not flip the image.
		*/
	void Texture_Draw(Texture* img, float x, float y, float scale);
	void Texture_DrawV(Texture* img, Vector2 lowerLeft, float scale);

	/**
		* @brief Draws the image using 2D vertices.
		*
		* This function expects the ortographic projection to be set. This function can stretch the image.
		*/
	void Texture_DrawRectF(Texture* img, RectF area);

	/**
		* @brief Loads an image from a file.
		* @param filename Name of the image file.
		* @param filterMode Filtering mode to use.
		* @return True if loading was succesfull.
		*/
	Texture* Texture_LoadFile(const char* filename, TextureFilterModes filterMode);

	/**
		* @brief Loads an image from a PNG file object.
		* @param pngFile PNG file object.
		* @param filterMode Filtering mode to use.
		* @return True if loading was succesfull.
		*/
	Texture* Texture_LoadPNG(PNGFile* pngFile, TextureFilterModes filterMode);

	/**
	 * @brief Generates a 8x8 checkerboard black and white texture
	 * @return The generated texture
	 */
	Texture* Texture_GenerateCheckerBoard(void);

	/**
	 * @brief Generates a 2x2 single color texture
	 * @return The generated texture
	 */
	Texture* Texture_GenerateColorTexture(color32 color);

	/**
	 * @brief Generates a random noise texture
	 * @return The generated texture
	 */
	Texture* Texture_GenerateNoiseTexture(u16 width, u16 height, u32 seed);

	Texture* Texture_CreateFromArray(u16 width, u16 height, void* pixels, GLenum colorFormat, GLenum dataFormat);

	/**
	 * @brief Sets the filtering mode of the texture
	 * @param texture The texture
	 * @param mode The mode to use
	 */
	void Texture_SetFilterMode(Texture* texture, TextureFilterModes mode);
	/**
	 * @brief Sets the wrapping mode of the texture
	 * @param texture The texture
	 * @param mode The mode to use
	 */
	void Texture_SetWrapMode(Texture* texture, TextureWrapModes mode);


#ifdef __cplusplus
}
#endif
