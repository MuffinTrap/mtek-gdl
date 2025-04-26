#pragma once

#include "mgdl-types.h"
#include "mgdl-opengl.h"
#include "mgdl-png.h"

namespace gdl
{

	/**
	 * @brief Abstract class for cross platform PNG image loading.
	 */
	class Image
	{
		public:


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
			void SetGLName(GLuint textureName, GLsizei width, GLsizei height, gdl::ColorFormats format);

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
			void Draw2DAligned(short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);

			/**
			 * @brief Draws the image using 2D vertices.
			 *
			 * This function expects the ortographic projection to be set. This function can stretch the image.
			 * @param x Upper left corner x of the image.
			 * @param y Upper left corner y of the image.
			 * @param x2 Lower right corner x of the image.
			 * @param y2 Lower right corner y of the image.
			 */
			void Draw2DAbsolute(short x, short y, short x2, short y2);

			/**
			 * @brief Draws the image on the origo in 3D space.
			 *
			 * This function expects the perspective projection to be set. This function preserves the aspect ratio.
			 * @param alignX Alignment on the X axis.
			 * @param alignY Alignment on the Y axis.
			 */
			void Draw3D(float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);

			/**
			 * @brief Sets the tint of the image.
			 *
			 * This function sets the tint color. Each pixel is multiplied by the tint color. Default tint is white (1,1,1)
			 * @param red Red component of the tint color, [0,1].
			 * @param green Red component of the tint color, [0,1].
			 * @param blue Red component of the tint color, [0,1].
			 */
			void SetTint(float red, float green, float blue);

			/**
			 * @brief Returns the width of the image.
			 * @return Width of the image in pixels.
			 */
			short GetWidth();

			/**
			 * @brief Returns the width of the image.
			 * @return Height of the image in pixels.
			 */
			short GetHeight();

			/**
			 * @brief Returns the aspect ratio of the image.
			 * @return Aspect ratio: width divided by height.
			 */
			float GetAspectRatio();

			/**
			 * @brief Returns the color format of the image.
			 * @return Color format : resolution and order of color components on each pixel.
			 */
			gdl::ColorFormats GetColorFormat();

			/**
			 * @brief Converts texture filter enumeration to OpenGL enumeration.
			 * @param filterMode Filtering mode to convert.
			 * @return OpenGL enumeration for the given format.
			 */
			static GLint TextureFilterToGLFilter(gdl::TextureFilterModes filterMode);

			/**
			 * @brief Returns the texture id used by OpenGL to draw this image.
			 * @return OpenGL texture ide.
			 */
			GLuint GetTextureId();

		PNGFile* pngFile;

		float aspectRatio;
		float red = 1.0f;
		float green = 1.0f;
		float blue = 1.0f;
		GLsizei width;
		GLsizei height;
		gdl::ColorFormats colorFormat;
		GLuint textureId;
	};
}

/**
 * @brief Loads an image from a file.
 * @param filename Name of the image file.
 * @param filterMode Filtering mode to use.
 * @return True if loading was succesfull.
 */
gdl::Image* Image_LoadFile(const char* filename, gdl::TextureFilterModes filterMode);

/**
 * @brief Loads an image from a PNG file object.
 * @param pngFile PNG file object.
 * @param filterMode Filtering mode to use.
 * @return True if loading was succesfull.
 */
gdl::Image* Image_LoadPNG(PNGFile* pngFile, gdl::TextureFilterModes filterMode);
