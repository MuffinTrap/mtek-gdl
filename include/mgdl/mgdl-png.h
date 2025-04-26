#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "mgdl-types.h"
#include "mgdl-opengl.h"

struct PNGFile
{
	GLsizei width;
	GLsizei height;
	GLint bytesPerPixel;

	int _pngFormat;
	GLubyte* _texels;
};

PNGFile* PNG_ReadFile(const char* filename);
void PNG_DeleteData(PNGFile* png);

GLenum PNG_GetGLFormat(PNGFile* png);
GLenum PNG_GetGLInternalFormat(PNGFile* png);

GLubyte* PNG_GetTexels(PNGFile* png);
gdl::rgba8 PNG_GetRGBA(PNGFile* png, int x, int y);
float PNG_GetGrayscale(PNGFile* png, int x, int y);

GLenum PNG_PNGtoGLFormat(int pngFormat);
GLenum PNG_PNGtoGLInternalFormat(int pngFormat);

PNGFile* _PNG_ReadFilePointer(FILE* fp);
bool _PNG_ReadTextureInfo(int color_type);
