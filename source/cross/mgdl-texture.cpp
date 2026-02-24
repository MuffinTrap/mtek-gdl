#include <mgdl/mgdl-texture.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-logger.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

Texture* Texture_Create()
{
	Texture* img = (Texture*)malloc(sizeof(Texture));
	img->aspectRatio = 1.0f;
	img->width = 0;
	img->height = 0;
	img->tint.red = 1.0f;
	img->tint.green = 1.0f;
	img->tint.blue = 1.0f;
	img->tint.alpha = 1.0f;
	img->textureId = 0;
	img->pngFile = nullptr;
	return img;
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

	// Data is loaded to OpenGX, release the buffers
	PNG_DeleteData(pngFile);
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
	Texture_SetTint(image, 1.0f, 1.0f, 1.0f);
									  //(1.0f/(float)png->height) * 0.375f);

	return image;
}

void Texture_SetGLName(Texture* img, GLuint textureName, GLsizei width, GLsizei height, ColorFormats format)
{
	img->width = width;
	img->height = height;
	img->textureId = textureName;
	img->colorFormat = format;
	Texture_SetTint(img, 1.0f, 1.0f, 1.0f);
}

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void Texture_Draw2DAbsolute(Texture* img, short x, short y, short x2, short y2)
{
	float dx = (float)x;
	float dy = (float)y;
	float dx2 = (float)x2;
	float dy2 = (float)y2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->textureId);
	glBegin(GL_QUADS);
		glColor3f(img->tint.red, img->tint.green, img->tint.blue);

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

void Texture_Draw2DAligned(Texture* img, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY)
{
	short w = img->width * scale;
	short h = img->height * scale;


	if (alignX == RJustify)
	{
		x -= w;
	}
	else if (alignX == Centered)
	{
		x -= w / 2;

	}
	if (alignY == RJustify)
	{
		y += h;
	}
	else if (alignY == Centered)
	{
		y -= h/2;
	}
	Texture_Draw2DAbsolute(img, x, y, x+w, y-h);
}

void Texture_Draw3D(Texture* img, float scale, AlignmentModes alignX, AlignmentModes alignY)
{
	float aspect = img->aspectRatio;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float hs = scale/2.0f;

	if (alignX == RJustify)
	{
		x -= scale * aspect;
	}
	else if (alignX == Centered)
	{
		x -= (scale * aspect)/ 2.0f;

	}
	if (alignY == RJustify)
	{
		y += scale;
	}
	else if (alignY == Centered)
	{
		y += scale/2.0f;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, img->textureId);
	glBegin(GL_QUADS);
		glColor3f(img->tint.red, img->tint.green, img->tint.blue);

		// TODO calculate uv to be inside border pixels to avoid repeating
		// Lower left
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x-aspect*hs, y-hs, z);

		// Lower right
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+aspect*hs, y-hs, z);

		// Upper right
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+aspect*hs, y+hs, z);

		// Upper left
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x-aspect*hs, y+hs, z);


	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}



void Texture_SetTint (Texture* img, float red, float green, float blue )
{
	img->tint.red = red;
	img->tint.green = green;
	img->tint.blue = blue;
}

GLuint PixelsToOpenGL(u32 width, u32 height, void* pixels, GLenum colorFormat, GLenum dataType)
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
	 * @return The generated texture
	 */
	Texture* Texture_GenerateColorTexture(Color4f* color)
	{
		const u32 width = 2;
		const u32 height = 2;
		GLubyte pixels[height][width][4];

		Color4b colorByte;
		colorByte.red = color->red * 255;
		colorByte.green = color->green * 255;
		colorByte.blue = color->blue * 255;
		colorByte.alpha = color->alpha * 255;

		for(u32 y = 0; y < height; y++)
		{
			for(u32 x = 0; x < width; x++)
			{
				pixels[y][x][0] = colorByte.red;
				pixels[y][x][1] = colorByte.green;
				pixels[y][x][2] = colorByte.blue;
				pixels[y][x][3] = colorByte.alpha;
			}
		}
		GLuint texName = PixelsToOpenGL(width, height, pixels, GL_RGBA, GL_UNSIGNED_BYTE);

		Texture* img = Texture_Create();
		Texture_SetGLName(img, texName, width, height, ColorFormats::RGBA);
		return img;

	}

	/**
	 * @brief Generates a random noise texture
	 * @return The generated texture
	 */
	Texture* Texture_GenerateNoiseTexture(u16 width, u16 height, u32 seed)
	{
		GLubyte noise[height][width];
		Random_SetSeed(seed);

		for(u32 y = 0; y < height; y++)
		{
			for(u32 x = 0; x < width; x++)
			{
				noise[y][x]= Random_FloatNormalized() * 255;
			}
		}

		GLuint texName = PixelsToOpenGL(width, height, noise, GL_LUMINANCE, GL_UNSIGNED_BYTE);


		Texture* img = Texture_Create();
		Texture_SetGLName(img, texName, width, height, ColorFormats::Gray);
		return img;
	}



