#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-file.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-assert.h>


#ifdef MGDL_WINDOWS_NATIVE

#define STBI_ONLY_PNG
#define STBI_MAX_DIMENSIONS 1024
#define STBI_NO_FAILURE_STRINGS
#define STBI_ASSERT(x) mgdl_assert_test(x)
#define STB_IMAGE_IMPLEMENTATION
#include <mgdl/stb/stb_image.h>

PNGFile* PNG_ReadFile(const char* filename)
{
	int width, height, channels;
	unsigned char* texelPtr = stbi_load(filename, &width, &height, &channels, 0);
	if (texelPtr == NULL)
	{
		Log_ErrorF("Could not read PNG file %s\n", filename);
		return nullptr;
	}
	Log_InfoF("Loaded Png file w:%d h:%d channels:%d\n", width, height, channels);

	// Flip picture on X axis
	unsigned char* tmpRow = (unsigned char*)malloc(width * channels);
	for (int r = 0; r < height/2; r++)
	{
		// First row to temp
		// last row to first
		// tmp to last
		for (int c = 0; c < width; c++)
		{

			for (int b = 0; b < channels; b++)
			{
				int byteFromStart = (r * width + c) * channels + b;
				int byteFromEnd = ((height - r - 1) * width + c)* channels + b;
				tmpRow[c * channels + b] = texelPtr[byteFromStart];
				texelPtr[byteFromStart] = texelPtr[byteFromEnd];
				texelPtr[byteFromEnd] = tmpRow[c * channels + b];
			}
		}
	}
	free(tmpRow);

	PNGFile* png = (PNGFile*)malloc(sizeof(PNGFile));
	png->width = width;
	png->height = height;
	png->bytesPerPixel = channels;
	png->_pngFormat = channels;
	png->_texels = texelPtr;
	
	return png;
}

GLenum PNG_PNGtoGLFormat(int stbi_format)
{
	switch (stbi_format)
	{
	case STBI_grey:
		return GL_LUMINANCE;
		break;
	case STBI_grey_alpha:
		return GL_LUMINANCE_ALPHA;
		break;
	case STBI_rgb:
		return GL_RGB;
		break;
	case STBI_rgb_alpha:
		return GL_RGBA;
		break;
	default:
		Log_Warning("Unsupported PNG color type\n");
		return 0;
		break;
	};
	return 0;
}

GLenum PNG_PNGtoGLInternalFormat(int stbi_format)
{
	switch (stbi_format)
	{
	case STBI_grey:
		return GL_UNSIGNED_BYTE;
		break;
	case STBI_grey_alpha:
		return GL_UNSIGNED_BYTE;
		break;
	case STBI_rgb:
		return GL_UNSIGNED_BYTE;
		break;
	case STBI_rgb_alpha:
		return GL_UNSIGNED_BYTE;
		break;
	default:
		Log_Warning("Unsupported PNG color type\n");
		return 0;
		break;
	};
	return 0;
}

#endif