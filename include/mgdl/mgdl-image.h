#pragma once

#include "mgdl-types.h"
#include "mgdl-opengl.h"

namespace gdl
{

	class PNGFile;

	/*! Abstract class for cross platform image loading
	 *
	 */
	class Image
	{
		public:
			bool LoadFile(const char* filename, gdl::TextureFilterModes filterMode);
			bool LoadBuffer(const u8* buffer, size_t size, gdl::TextureFilterModes filterMode);
			bool LoadPNG(PNGFile* pngFile, gdl::TextureFilterModes filterMode);
			bool LoadGLName(GLuint textureName, GLsizei width, GLsizei height, gdl::ColorFormats format);
			void Draw2DAligned(short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);
			void Draw2DAbsolute(short x, short y, short x2, short y2);
			void Draw3D(float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY);
			void SetTint(float red, float green, float blue);

			short GetWidth();
			short GetHeight();
			float GetAspectRatio();
			gdl::ColorFormats GetColorFormat();
			static GLint TextureFilterToGLFilter(gdl::TextureFilterModes filterMode);
			GLuint GetTextureId();

	private:
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
