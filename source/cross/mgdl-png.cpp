#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-file.h>

#include <png.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <csetjmp>

gdl::PNGFile::PNGFile()
{
	texels = nullptr;

}

gdl::PNGFile::~PNGFile()
{
	DeleteData();
}

void gdl::PNGFile::DeleteData()
{
	if (texels != nullptr)
	{
		free(texels);
		texels = nullptr;
	}
}

bool gdl::PNGFile::ReadTextureInfo(int color_type)
{
	pngFormat = color_type;
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
			printf("\tUnsupported PNG COLOR TYPE!\n");
			return false;
		break;
	};
	return true;
}

GLenum gdl::PNGFile::GetGLFormat()
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
			printf("\tUnsupported PNG COLOR TYPE!\n");
			return 0;
		break;
	};
	return 0;
}

bool gdl::PNGFile::ReadPNG(FILE* fp)
{
	// Read from file pointer
	png_byte magic[8];
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *row_pointers = nullptr;
	int bit_depth, color_type;

	// Read Magic number :O
	fread(magic, 1, sizeof(magic), fp);
	if (png_check_sig(magic, sizeof(magic)) == false)
	{
		printf("\t[INVALID PNG]\n");
		fclose(fp);
		return false;
	}

	// Create struct for reading the file
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == nullptr)
	{
		printf("\t[FAILED TO READ]\n");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return false;
	}

	// Create struct to hold info about the file
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr)
	{
		printf("\t[FAILED TO GET INFO]\n");
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		return false;
	}

	// This sets what happens if there is an error during read
	if (setjmp (png_jmpbuf(png_ptr)))
	{
		if (row_pointers != nullptr)
		{
			free(row_pointers);
		}
		if (texels != nullptr)
		{
			free (texels);
		}
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}

	// Lets read the file now!
	png_init_io(png_ptr, fp);

	// Skip magic number since we read it already
	png_set_sig_bytes(png_ptr, sizeof(magic));

	png_read_info(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	// React to info we got
	printf("\tColor: ");
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		// Read this as RGB image
		printf("palette ");
		png_set_palette_to_rgb(png_ptr);
	}
	else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		// Small precision grayscale to 8 bit format
		printf("grayscale < 8 ");
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	else
	{
		printf("RGB ");
	}
	printf("Alpha: ");
	// Is transparency info read ok?
	png_uint_32 getResult = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
	if (getResult == 0)
	{
		printf("YES ");
		// Direct it to alpha channel
		png_set_tRNS_to_alpha(png_ptr);
	}
	else
	{
		printf("NO ");
	}

	printf("Bit depth: ");
	if (bit_depth == 16)
	{
		printf("16 ");
		png_set_strip_16(png_ptr);
	}
	else if (bit_depth < 8)
	{
		// ???
		printf("<8 ");
		png_set_packing(png_ptr);
	}
	else
	{
		printf("%d ", bit_depth);
	}
	printf("\n");

	// Update our changes and read again
	png_read_update_info(png_ptr, info_ptr);
	png_uint_32 w, h;
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);

	// Wii cannot handle very big textures
	if ((w > 1024) || (h > 1024)) {
		printf("\t[TOO LARGE: MAX 1024]\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return false;
	}
	else
	{
		printf("\tSize: %u x %u\n", w, h);
		width = w;
		height = h;
	}

	bool colorOk = ReadTextureInfo(color_type);
	if (colorOk == false)
	{
		printf("\t[STRANGE PNG COLOR TYPE]\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return false;
	}

	// Allocate!
	size_t imageDataSize = sizeof(GLubyte) * w * h * bytesPerPixel;
	printf("\tAllocating %zu bytes\n", imageDataSize);
	texels = (GLubyte*)AllocateAlignedMemory(imageDataSize);

	if (texels == nullptr)
	{
		printf("\t[OUT OF MEMORY]\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		fclose(fp);
		return false;
	}

	// Set up array for rows to read
	size_t rowPointersSize = sizeof(png_bytep) * h;
	row_pointers = (png_bytep*)AllocateAlignedMemory(rowPointersSize);

	// Read all rows
	size_t rowSize = w * bytesPerPixel;

	// Is the first row at the start or at the end?
// #define READ_BOTTOM_UP

	for (png_uint_32 i = 0; i < h; i++)
	{
#ifdef READ_BOTTOM_UP
		size_t readStart = (h - (i+1)) * rowSize;
#else
		size_t readStart = i * rowSize;
#endif
		row_pointers[i] = (png_bytep)(texels + readStart);
	}
	CacheFlushRange(row_pointers, rowPointersSize);

	// Pointers to rows are set, read the pixels on the rows
	png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	// Reading is over, rows no longer needed
	free(row_pointers);

	printf("\tPNG read done\n");

	// Flush read texels
	CacheFlushRange(texels, imageDataSize);
	return true;
}

bool gdl::PNGFile::ReadBuffer(const u8* buffer, size_t size)
{
	// fmemopen cannot read from const buffer :U
	// Copy data to temporary buffer before reading
	u8 *tempBuffer = (u8*)gdl::AllocateAlignedMemory(size);
	gdl_assert_print((tempBuffer != nullptr), "Cannot allocate enough memory for image buffer.");

	// Copy
	memcpy(tempBuffer, buffer, size);

	// Make sure that the data is in the main memory
	CacheFlushRange(tempBuffer, size);

	// Open file
	FILE *fp= gdl::BufferOpen(tempBuffer, size, "rb");

	if (fp == nullptr)
	{
		printf("Image buffer failed to open\n");
		free(tempBuffer);
		return false;
	}
	bool readOk = ReadPNG(fp);
	free(tempBuffer);
	fclose(fp);
	return readOk;
}

bool gdl::PNGFile::ReadFile(const char* filename)
{
	FILE *fp = fopen(filename, "rb");
	if (fp == nullptr)
	{
		printf("[NOT FOUND!]\n");
		return false;
	}

	bool readOk = ReadPNG(fp);
	fclose(fp);
	return readOk;
}

GLsizei gdl::PNGFile::GetWidth() { return width; }
GLsizei gdl::PNGFile::GetHeight() { return height; }
GLint gdl::PNGFile::GetBytesPerPixel() { return bytesPerPixel; }
GLubyte *gdl::PNGFile::GetTexels() { return texels; }
