#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-file.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-logger.h>

#if 0
/** Old implementation that used
 * libpng directly
 */

#include <png.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <csetjmp>

GLint PNG_ColorTypeToBPP(int color_type)
{
	short bytesPerPixel = 0;
	switch(color_type)
	{
		case PNG_COLOR_TYPE_GRAY:
			// format = GL_LUMINANCE;
			bytesPerPixel = 1;
		break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			// texInfo->format = GL_LUMINANCE_ALPHA;
			bytesPerPixel = 2;
		break;
		case PNG_COLOR_TYPE_RGB:
			// texInfo->format = GL_RGB;
			bytesPerPixel = 3;
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			// texInfo->format = GL_RGBA;
			bytesPerPixel = 4;
		break;
		default:
			Log_Warning("Unsupported PNG color type\n");
		break;
	};
	return bytesPerPixel;
}

GLenum PNG_PNGtoGLFormat(int pngFormat)
{
	switch(pngFormat)
	{
		case PNG_COLOR_TYPE_GRAY:
			return GL_LUMINANCE;
		break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			return GL_LUMINANCE_ALPHA;
		break;
		case PNG_COLOR_TYPE_RGB:
			return GL_RGB;
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			return GL_RGBA;
		break;
		default:
			Log_Warning("Unsupported PNG color type\n");
			return 0;
		break;
	};
	return 0;
}

GLenum PNG_PNGtoGLInternalFormat(int pngFormat)
{
	switch(pngFormat)
	{
		case PNG_COLOR_TYPE_GRAY:
			return GL_UNSIGNED_BYTE;
		break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			return GL_UNSIGNED_BYTE;
		break;
		case PNG_COLOR_TYPE_RGB:
			return GL_UNSIGNED_BYTE;
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
			return GL_UNSIGNED_BYTE;
		break;
		default:
			Log_Warning("Unsupported PNG color type\n");
			return 0;
		break;
	};
	return 0;

}


PNGFile* _PNG_ReadFilePointer(FILE* fp)
{
	// Read from file pointer
	png_byte magic[8];
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers = nullptr;
	int bit_depth, color_type;


	// Temp pointer for texels
	GLubyte* texelPtr = nullptr;

	// Read Magic number :O
	fread(magic, 1, sizeof(magic), fp);
	if (png_check_sig(magic, sizeof(magic)) == false)
	{
		Log_Error("Invalid PNG data\n");
		fclose(fp);
		return nullptr;
	}

	// Create struct for reading the file
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == nullptr)
	{
		Log_Error("Failed to create PNG read struct\n");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return nullptr;
	}

	// Create struct to hold info about the file
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr)
	{
		Log_Error("Failed to create PNG info struct\n");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return nullptr;
	}

	// This sets what happens if there is an error during read
	if (setjmp (png_jmpbuf(png_ptr)))
	{
		if (row_pointers != nullptr)
		{
			free(row_pointers);
		}
		if (texelPtr != nullptr)
		{
			free (texelPtr);
		}
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return nullptr;
	}

	// Lets read the file now!
	png_init_io(png_ptr, fp);

	// Skip magic number since we read it already
	png_set_sig_bytes(png_ptr, sizeof(magic));

	png_read_info(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	// React to info we got
	Log_Info("\tColor: ");
	switch(color_type)
	{
		case PNG_COLOR_TYPE_PALETTE:
		{
			// Read this as RGB image
			Log_Info("palette ");
			png_set_palette_to_rgb(png_ptr);
		}
		break;
		case PNG_COLOR_TYPE_GRAY:
		{
			Log_Info("grayscale ");
			// Small precision grayscale to 8 bit format
			if (bit_depth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}
		}
		break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			Log_Info("grayscale with alpha ");
			// Small precision grayscale to 8 bit format
			if (bit_depth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}
		}
		break;
		case PNG_COLOR_TYPE_RGB:
		{
			Log_Info("RGB ");
		}
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			Log_Info("RGBA ");
		}
		break;
	};
	Log_Info("Alpha: ");
	// Is transparency info read ok?
	png_uint_32 getResult = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
	if (getResult == 0)
	{
		Log_Info("YES ");
		// Direct it to alpha channel
		png_set_tRNS_to_alpha(png_ptr);
	}
	else
	{
		Log_Info("NO ");
	}

	Log_Info("Bit depth: ");
	if (bit_depth == 16)
	{
		Log_Info("16 ");
		png_set_strip_16(png_ptr);
	}
	else if (bit_depth < 8)
	{
		// ???
		Log_Info("<8 ");
		png_set_packing(png_ptr);
	}
	else
	{
		Log_InfoF("%d ", bit_depth);
	}
	Log_Info("\n");

	// Update our changes and read again
	png_read_update_info(png_ptr, info_ptr);
	png_uint_32 w, h;
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);

	short width = 0;
	short height = 0;
	// Wii cannot handle very big textures
	//
	if ((w > 1024) || (h > 1024)) {
		Log_Error("PNG image too large, max size 1024\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return nullptr;
	}
	else
	{
		Log_InfoF("\tSize: %u x %u\n", w, h);
		width = w;
		height = h;
	}

	short bpp = PNG_ColorTypeToBPP(color_type);
	if (bpp == 0)
	{
		Log_Warning("Strange PNG color type\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return nullptr;
	}

	// Allocate!
	size_t imageDataSize = sizeof(GLubyte) * w * h * bpp;

	// Danger. Wii only has 20 megabytes of texture memoy
	Log_InfoF("\tAllocating %zu bytes\n", imageDataSize);
	texelPtr = (GLubyte*)mgdl_AllocateAlignedMemory(imageDataSize);

	if (texelPtr == nullptr)
	{
		Log_Error("Out of memory\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return nullptr;
	}

	PNGFile* png = (PNGFile*)malloc(sizeof(PNGFile));
	png->width = width;
	png->height = height;
	png->bytesPerPixel = bpp;
	png->_pngFormat = color_type;
	png->_texels = texelPtr;

	// Set up array for rows to read
	size_t rowPointersSize = sizeof(png_bytep) * h;
	row_pointers = (png_bytep*)mgdl_AllocateAlignedMemory(rowPointersSize);

	// Read all rows
	size_t rowSize = w * png->bytesPerPixel;

	// Is the first row at the start or at the end?
 #define READ_BOTTOM_UP

	for (png_uint_32 i = 0; i < h; i++)
	{
#ifdef READ_BOTTOM_UP
		size_t readStart = (h - (i+1)) * rowSize;
#else
		size_t readStart = i * rowSize;
#endif
		row_pointers[i] = (png_bytep)(png->_texels + readStart);
	}
	mgdl_CacheFlushRange(row_pointers, rowPointersSize);

	// Pointers to rows are set, read the pixels on the rows
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	// Reading is over, rows no longer needed
	free(row_pointers);

	Log_Info("PNG read done\n");

	// Flush read texels
	mgdl_CacheFlushRange(png->_texels, imageDataSize);
	return png;
}

PNGFile* PNG_ReadFile(const char* filename)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == nullptr)
	{
		mgdl_assert_printf(false, "PNG_ReadFile did not find %s\n", filename);
		return nullptr;
	}

	PNGFile* png = _PNG_ReadFilePointer(fp);
	fclose(fp);
	return png;
}


GLenum PNG_GetGLFormat(PNGFile* png)
{
	return PNG_PNGtoGLFormat(png->_pngFormat);
}
GLenum PNG_GetGLInternalFormat(PNGFile* png)
{
	return PNG_PNGtoGLInternalFormat(png->_pngFormat);
}

#endif


