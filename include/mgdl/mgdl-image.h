#pragma once

#include "mgdl-types.h"
#include "mgdl-util.h"
#include "mgdl-opengl.h"
#include "mgdl-png.h"


/**
	* @brief Abstract class for cross platform PNG image loading.
	*/
struct Image
{
	PNGFile* pngFile = nullptr;

	float aspectRatio = 1.0f;
	gdl::RGBA8Floats tint;
	GLsizei width = 0;
	GLsizei height = 0;
	gdl::ColorFormats colorFormat;
	GLuint textureId = 0;
};

extern "C"
{

	/**
		* @brief Sets the GL name and dimenions of the image.
		*
		* This is used when the image data has already been loaded using OpenGL.
		*
		* @param textureName GL name of the texture.
		* @param width Width of the image in pixels.
		* @param height Height of the image in pixels.
		* @param format Color format of the image.
		*/
	void Image_SetGLName(Image* img, GLuint textureName, GLsizei width, GLsizei height, gdl::ColorFormats format);

	/**
		* @brief Draws the image using 2D vertices.
		*
		* This function expects the ortographic projection to be set. This function preserves the aspect ratio.
		* @param x Upper left corner x of the image.
		* @param y Upper left corner y of the image.
		* @param scale Scaling of the image. Scale of 1.0 does not alter the size. Negative scale does not flip the image.
		* @param alignX Alignment on the X axis.
		* @param alignY Alignment on the Y axis.
		*/
	void Image_Draw2DAligned(Image* img, short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);

	/**
		* @brief Draws the image using 2D vertices.
		*
		* This function expects the ortographic projection to be set. This function can stretch the image.
		* @param x Upper left corner x of the image.
		* @param y Upper left corner y of the image.
		* @param x2 Lower right corner x of the image.
		* @param y2 Lower right corner y of the image.
		*/
	void Image_Draw2DAbsolute(Image* img, short x, short y, short x2, short y2);

	/**
		* @brief Draws the image on the origo in 3D space.
		*
		* This function expects the perspective projection to be set. This function preserves the aspect ratio.
		* @param alignX Alignment on the X axis.
		* @param alignY Alignment on the Y axis.
		*/
	void Image_Draw3D(Image* img, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);

	/**
		* @brief Sets the tint of the image.
		*
		* This function sets the tint color. Each pixel is multiplied by the tint color. Default tint is white (1,1,1)
		* @param red Red component of the tint color, [0,1].
		* @param green Red component of the tint color, [0,1].
		* @param blue Red component of the tint color, [0,1].
		*/
	void Image_SetTint(Image* img, float red, float green, float blue);


	/**
		* @brief Loads an image from a file.
		* @param filename Name of the image file.
		* @param filterMode Filtering mode to use.
		* @return True if loading was succesfull.
		*/
	Image* Image_LoadFile(const char* filename, gdl::TextureFilterModes filterMode);

	/**
		* @brief Loads an image from a PNG file object.
		* @param pngFile PNG file object.
		* @param filterMode Filtering mode to use.
		* @return True if loading was succesfull.
		*/
	Image* Image_LoadPNG(PNGFile* pngFile, gdl::TextureFilterModes filterMode);

	/**
	 * @brief Generates a 8x8 checkerboard image
	 * @return The generated image
	 */
	Image* Image_GenerateCheckerBoard();

}
