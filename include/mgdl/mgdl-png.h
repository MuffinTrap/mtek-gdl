#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "mgdl-types.h"
#include "mgdl-opengl.h"

namespace gdl
{
	class PNGFile
	{
	public:
		PNGFile();
		~PNGFile();
		bool ReadBuffer(const u8* buffer, size_t size);
		bool ReadFile(const char* filename);
		void DeleteData();
		GLenum GetGLFormat();

		GLsizei GetWidth();
		GLsizei GetHeight();
		GLint GetBytesPerPixel();
		GLubyte *GetTexels();

	private:
		bool ReadPNG(FILE* fp);
		bool ReadTextureInfo(int color_type);
		GLsizei width;
		GLsizei height;
		int pngFormat;
		GLint bytesPerPixel;
		GLubyte* texels;
	};
}
