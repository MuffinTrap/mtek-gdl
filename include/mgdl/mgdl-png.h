#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "mgdl-types.h"
#include "mgdl-color.h"
#include "mgdl-opengl.h"

/**
 * @file mgdl-png.h
 * @ingroup asset
 * @brief Struct and functions for loading PNG files
 */

struct PNGFile
{
	GLsizei width;
	GLsizei height;
	GLint bytesPerPixel;

	int _pngFormat;
	GLubyte* _texels;
};
typedef struct PNGFile PNGFile;

#ifdef __cplusplus
extern "C"
{
#endif

PNGFile* PNG_GenerateColorImage(color32 color);

PNGFile* PNG_ReadFile(const char* filename);
void PNG_DeleteData(PNGFile* png);

GLenum PNG_GetGLFormat(PNGFile* png);
GLenum PNG_GetGLInternalFormat(PNGFile* png);
GLenum PNG_GetGLType(PNGFile* png);

GLubyte* PNG_GetTexels(PNGFile* png);
color32 PNG_GetRGBA(PNGFile* png, int x, int y);
float PNG_GetGrayscale(PNGFile* png, int x, int y);

GLenum PNG_PNGtoGLFormat(int pngFormat);
GLenum PNG_PNGtoGLInternalFormat(int pngFormat);
int PNG_GLFormattoPNGFormat(GLenum format);

#ifdef __cplusplus
}
#endif
