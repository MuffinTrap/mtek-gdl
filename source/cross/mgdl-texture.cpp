#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-types.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

Texture* Texture_Create()
{
	Texture* img = (Texture*)mgdl_AllocateGraphicsMemory(sizeof(Texture));
	img->aspectRatio = 1.0f;
	img->width = 0;
	img->height = 0;
	img->textureId = 0;
	img->pngFile = nullptr;
	img->spriteAtlas = nullptr;
	return img;
}

void Texture_Destroy(Texture* texture)
{
	mgdl_FreeGraphicsMemory(texture);
}

Texture* Texture_LoadFile ( const char* filename, TextureFilterModes filterMode)
{
	// Load using png
	Log_InfoF("Loading image %s\n", filename);

	PNGFile* pngFile = PNG_ReadFile(filename);
	if (pngFile == nullptr)
	{
		return Texture_GenerateCheckerBoard();
	}
	Texture* image = Texture_LoadPNG(pngFile, filterMode);
	Log_InfoF("Loaded image to texture size %d %d\n", image->width, image->height);

	// Data is loaded to OpenGX, release the buffers
	PNG_DeleteData(pngFile);
	mgdl_FreeGraphicsMemory(pngFile);
	return image;
}

// The png might belong to someone else, do not free it in this function
Texture* Texture_LoadPNG(PNGFile* png, TextureFilterModes filterMode)
{
	mgdl_assert_print(png != nullptr, "Texture_LoadPNG got nullptr for png\n");

	Texture* image = Texture_Create();
	GLint glFilter = TextureFilterToGLFilter(filterMode);

	GLint alignment;
	glGenTextures(1, &image->textureId);
	glBindTexture(GL_TEXTURE_2D, image->textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Load to OpenGL

	// Read current alignment and set alignment to 1
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// We dont have mipmaps..
	glTexImage2D(GL_TEXTURE_2D, 0, png->bytesPerPixel,
			  png->width, png->height,
			  0, PNG_GetGLFormat(png),
			  PNG_GetGLInternalFormat(png),
			  PNG_GetTexels(png));

	// restore previous alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	// copy data
	image->width = png->width;
	image->height = png->height;
	image->aspectRatio = image->width / image->height;

	return image;
}

void Texture_SetGLName(Texture* img, GLuint textureName, GLsizei width, GLsizei height, ColorFormats format)
{
	ASSERT_DEBUG(img != nullptr);

	img->width = width;
	img->height = height;
	img->textureId = textureName;
	img->colorFormat = format;
}

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void Texture_DrawRectF(Texture* img, RectF area)
{
	ASSERT_DEBUG(img != nullptr);
	float dx = (float)area.x;
	float dy = (float)area.y;
	float dx2 = (float)area.x + area.w;
	float dy2 = (float)area.y - area.h;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->textureId);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		// Lower left
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(dx, dy2);
		// Lower right
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(dx2, dy2);
		// Upper right
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(dx2, dy);
		// Upper left
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(dx, dy);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Texture_Draw(Texture* img, float x, float y, float scale)
{
	ASSERT_DEBUG(img != nullptr);
	float w = img->width * scale;
	float h = img->height * scale;

	Texture_DrawRectF(img, RectF_Create(x, y, w, h));
}

static GLuint PixelsToOpenGL(u32 width, u32 height, void* pixels, GLenum colorFormat, GLenum dataType)
{
	GLint alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint texName;
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, width, height, 0, colorFormat, dataType, pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	return texName;

}

Texture* Texture_CreateFromArray(u16 width, u16 height, void* pixels, GLenum colorFormat, GLenum dataFormat)
{
	ASSERT_DEBUG(pixels != nullptr);
	GLuint texName = PixelsToOpenGL(width, height, pixels, colorFormat, dataFormat);

	ColorFormats f = ColorFormats::RGBA;
	switch(colorFormat)
	{
		case GL_LUMINANCE:
			f = ColorFormats::Gray;
			break;
		case GL_LUMINANCE_ALPHA:
			f = ColorFormats::GrayAlpha;
			break;
		case GL_RGB:
			f = ColorFormats::RGB;
			break;
		case GL_RGBA:
			f = ColorFormats::RGBA;
			break;
	};
	Texture* img = Texture_Create();
	Texture_SetGLName(img, texName, width, height, f);
	return img;
}

Texture* Texture_GenerateCheckerBoard()
{
	const u32 width = 8;
	const u32 height = 8;
	u32 index = 0;
	GLubyte checkerTexture[height][width][2];

	for(u32 y = 0; y < height; y++)
	{
		for(u32 x = 0; x < width; x++)
		{
			index = (x+y)%2;

			checkerTexture[y][x][0] = index == 0? 255 :0;
			checkerTexture[y][x][1] = 255;
		}
	}

	GLuint texName = PixelsToOpenGL(width, height, checkerTexture, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE);


	Texture* img = Texture_Create();
	Texture_SetGLName(img, texName, width, height, ColorFormats::GrayAlpha);
	return img;
}
	/**
	 * @brief Generates a 2x2 single color texture
	 * @param color The alpha value is ignored.
	 * @return The generated texture
	 */
	Texture* Texture_GenerateColorTexture(color32 color)
	{
		const u32 width = 2;
		const u32 height = 2;
		GLubyte* pixels = (GLubyte*)mgdl_AllocateGeneralMemory(sizeof(GLubyte)*height*width*3);

		u8 red = RED(color);
		u8 green = GREEN(color);
		u8 blue = BLUE(color);

		for(u32 y = 0; y < height; y++)
		{
			for(u32 x = 0; x < width; x++)
			{
				pixels[y*width + x*3 + 0] = red;
				pixels[y*width + x*3 + 1] = green;
				pixels[y*width + x*3 + 2] = blue;
			}
		}
		GLuint texName = PixelsToOpenGL(width, height, pixels, GL_RGB, GL_UNSIGNED_BYTE);

		Texture* img = Texture_Create();
		Texture_SetGLName(img, texName, width, height, ColorFormats::RGB);
		mgdl_FreeGeneralMemory(pixels);
		return img;

	}

	/**
	 * @brief Generates a random noise texture
	 * @return The generated texture
	 */
	Texture* Texture_GenerateNoiseTexture(u16 width, u16 height, u32 seed)
	{
		GLubyte* noise = (GLubyte*)mgdl_AllocateGeneralMemory(sizeof(GLubyte)*height*width);
		Random_SetSeed(seed);

		for(u32 y = 0; y < height; y++)
		{
			for(u32 x = 0; x < width; x++)
			{
				noise[y * width + x]= Random_FloatNormalized() * 255;
			}
		}

		GLuint texName = PixelsToOpenGL(width, height, noise, GL_LUMINANCE, GL_UNSIGNED_BYTE);


		Texture* img = Texture_Create();
		Texture_SetGLName(img, texName, width, height, ColorFormats::Gray);
		mgdl_FreeGeneralMemory(noise);
		return img;
	}

	void Texture_SetFilterMode(Texture* texture, TextureFilterModes mode)
	{
		ASSERT_DEBUG(texture != nullptr);
		glBindTexture(GL_TEXTURE_2D, texture->textureId);

		GLint glFilter = TextureFilterToGLFilter(mode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture_SetWrapMode(Texture* texture, TextureWrapModes mode)
	{
		ASSERT_DEBUG(texture != nullptr);
		glBindTexture(GL_TEXTURE_2D, texture->textureId);
		GLint glWrap = TextureWrapToGLWrap(mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);
		glBindTexture(GL_TEXTURE_2D, 0);

	}


