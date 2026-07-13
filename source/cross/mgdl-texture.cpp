#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-memory.h>
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
	img->hasMipmaps = false;
	return img;
}

void Texture_Destroy(Texture* texture)
{
	mgdl_FreeGraphicsMemory(texture);
}

Texture* Texture_LoadFile ( const char* filename, TextureFilterModes filterMode, bool generateMipmaps)
{
	// Load using png
	Log_InfoF("Loading image %s\n", filename);

	PNGFile* pngFile = PNG_ReadFile(filename);
	if (pngFile == nullptr)
	{
		return Texture_GenerateCheckerBoard(generateMipmaps);
	}
	Texture* image = Texture_LoadPNG(pngFile, filterMode, generateMipmaps);
	Log_InfoF("Loaded image to texture size %d %d\n", image->width, image->height);

	// Data is loaded to OpenGX, release the buffers
	PNG_DeleteData(pngFile);
	mgdl_FreeGraphicsMemory(pngFile);
	return image;
}

// The png might belong to someone else, do not free it in this function
Texture* Texture_LoadPNG(PNGFile* png, TextureFilterModes filterMode, bool generateMipmaps)
{
	mgdl_assert_print(png != nullptr, "Texture_LoadPNG got nullptr for png\n");

	Texture* image = Texture_Create();
	GLint glFilter = TextureFilterToGLFilter(filterMode);

	GLint alignment;
	glGenTextures(1, &image->textureId);
	glBindTexture(GL_TEXTURE_2D, image->textureId);

	// Load to OpenGL

	// Read current alignment and set alignment to 1
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (generateMipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,
					PNG_GetGLInternalFormat(png),
					png->width, png->height,
					PNG_GetGLFormat(png),
					PNG_GetGLType(png),
					PNG_GetTexels(png));
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0,
					PNG_GetGLInternalFormat(png),
					png->width, png->height,
					0,  // Border
					PNG_GetGLFormat(png),
					PNG_GetGLType(png),
					PNG_GetTexels(png));
	}

	// restore previous alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	// copy data
	image->width = png->width;
	image->height = png->height;
	image->aspectRatio = image->width / image->height;
	image->hasMipmaps = generateMipmaps;

	Texture_SetFilterModeMag(image, filterMode);
	Texture_SetFilterModeMin(image, filterMode);
	Texture_SetWrapMode(image, Wrap_Clamp);

	glBindTexture(GL_TEXTURE_2D, 0);

	return image;
}

void Texture_SetGLName(Texture* img, GLuint textureName, GLsizei width, GLsizei height, ColorFormats format, bool hasMipmaps)
{
	ASSERT_DEBUG(img != nullptr);

	img->width = width;
	img->height = height;
	img->textureId = textureName;
	img->colorFormat = format;
	img->hasMipmaps = hasMipmaps;
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
	glScalef(scale, scale, 1.0f);

	Texture_DrawRectF(img, RectF_Create(x, y, img->width, img->height));
}

static GLuint PixelsToOpenGL(u32 width, u32 height, void* pixels, GLenum internalFormat, GLenum colorFormat, GLenum dataType, bool generateMipmaps)
{

	GLint alignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint texName;
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	if (generateMipmaps)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D,
			internalFormat,
			width, height,
			colorFormat, dataType,
			pixels);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0,
			internalFormat,
			width, height, 0,
			colorFormat, dataType,
			pixels);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texName;

}

Texture* Texture_CreateFromArray(TextureFilterModes filterMode, u16 width, u16 height, void* pixels, GLenum internalFormat, GLenum colorFormat, GLenum dataFormat, bool generateMipmaps)
{
	ASSERT_DEBUG(pixels != nullptr);
	GLuint texName = PixelsToOpenGL(width, height, pixels, internalFormat, colorFormat, dataFormat, generateMipmaps);

	ColorFormats format = ColorFormats::RGBA;
	switch(colorFormat)
	{
		case GL_LUMINANCE:
			format = ColorFormats::Gray;
			break;
		case GL_LUMINANCE_ALPHA:
			format = ColorFormats::GrayAlpha;
			break;
		case GL_RGB:
			format = ColorFormats::RGB;
			break;
		case GL_RGBA:
			format = ColorFormats::RGBA;
			break;
	};
	Texture* img = Texture_Create();

	Texture_SetGLName(img, texName, width, height, format, generateMipmaps);
	Texture_SetFilterModeMag(img, filterMode);
	Texture_SetFilterModeMin(img, filterMode);
	Texture_SetWrapMode(img, Wrap_Clamp);
	return img;
}

Texture* Texture_GenerateCheckerBoard(bool generateMipmaps)
{
	const u32 width = 8;
	const u32 height = 8;
	u32 index = 0;
	GLubyte checkerTexture[height][width];

	for(u32 y = 0; y < height; y++)
	{
		for(u32 x = 0; x < width; x++)
		{
			index = (x+y)%2;

			checkerTexture[y][x] = index == 0? 255 :0;
		}
	}

	TextureFilterModes filter = Linear;
	if (generateMipmaps)
	{
		filter = MipmapLinear;
	}
	GLuint texName = PixelsToOpenGL(width, height, checkerTexture, GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, generateMipmaps);


	Texture* img = Texture_Create();
	Texture_SetGLName(img, texName, width, height, ColorFormats::Gray, generateMipmaps);
	Texture_SetFilterModeMag(img, Nearest);
	Texture_SetFilterModeMin(img, filter);
	return img;
}
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
	GLuint texName = PixelsToOpenGL(width, height, pixels, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false);

	Texture* img = Texture_Create();
	Texture_SetGLName(img, texName, width, height, ColorFormats::RGB, false);
	Texture_SetFilterModeMag(img, Nearest);
	Texture_SetFilterModeMin(img, Nearest);

	mgdl_FreeGeneralMemory(pixels);
	return img;
}

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

	GLuint texName = PixelsToOpenGL(width, height, noise, GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, false);

	Texture* img = Texture_Create();
	Texture_SetGLName(img, texName, width, height, ColorFormats::Gray, false);
	Texture_SetFilterModeMag(img, Nearest);
	Texture_SetFilterModeMin(img, Nearest);

	mgdl_FreeGeneralMemory(noise);
	return img;
}

void Texture_SetFilterModeMag(Texture* texture, TextureFilterModes mode)
{
	ASSERT_DEBUG(texture != nullptr);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);

	if ((mode == Nearest || mode == Linear) == false)
	{
		Log_Error("Setting the texture Magnifying filter to minmap filter is not possible");
	}

	GLint glFilter = TextureFilterToGLFilter(mode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture_SetFilterModeMin(Texture* texture, TextureFilterModes mode)
{
	ASSERT_DEBUG(texture != nullptr);
	glBindTexture(GL_TEXTURE_2D, texture->textureId);

	if (texture->hasMipmaps && (mode == Nearest || mode == Linear))
	{
		Log_Warning("Setting Linear or Nearest filtering mode to texture with mipmaps will not use mipmaps when rendering\n");
	}
	else if (texture->hasMipmaps == false && (mode == Nearest || mode == Linear) == false)
	{
		Log_Error("Setting mipmap filtering mode to texture with no mipmaps will not render the texture");
	}

	GLint glFilter = TextureFilterToGLFilter(mode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
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
