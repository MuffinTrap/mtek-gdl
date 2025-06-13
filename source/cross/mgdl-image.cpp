#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-logger.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

Image* Image_Create()
{
	Image* img = new Image();
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

Image* Image_LoadFile ( const char* filename, TextureFilterModes filterMode)
{
	// Load using png
	Log_InfoF("Loading image %s\n", filename);

	PNGFile* pngFile = PNG_ReadFile(filename);
	if (pngFile == nullptr)
	{
		return Image_GenerateCheckerBoard();
	}
	Image* image = Image_LoadPNG(pngFile, filterMode);

	// Data is loaded to OpenGX, release the buffers
	PNG_DeleteData(pngFile);
	return image;
}

// The png might belong to someone else, do not free it in this function
Image* Image_LoadPNG(PNGFile* png, TextureFilterModes filterMode)
{
	mgdl_assert_print(png != nullptr, "Image_LoadPNG got nullptr for png\n");

	Image* image = Image_Create();
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
	Image_SetTint(image, 1.0f, 1.0f, 1.0f);
									  //(1.0f/(float)png->height) * 0.375f);

	return image;
}

void Image_SetGLName(Image* img, GLuint textureName, GLsizei width, GLsizei height, ColorFormats format)
{
	img->width = width;
	img->height = height;
	img->textureId = textureName;
	img->colorFormat = format;
	Image_SetTint(img, 1.0f, 1.0f, 1.0f);
}

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void Image_Draw2DAbsolute(Image* img, short x, short y, short x2, short y2)
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
		glVertex2f(dx, dy);
		// Lower right
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(dx2, dy);
		// Upper right
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(dx2, dy2);
		// Upper left
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(dx, dy2);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Image_Draw2DAligned(Image* img, short x, short y, float scale, AlignmentModes alignX, AlignmentModes alignY)
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
	Image_Draw2DAbsolute(img, x, y, x+w, y+h);
}

void Image_Draw3D(Image* img, float scale, AlignmentModes alignX, AlignmentModes alignY)
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



void Image_SetTint (Image* img, float red, float green, float blue )
{
	img->tint.red = red;
	img->tint.green = green;
	img->tint.blue = blue;
}

Image* Image_GenerateCheckerBoard()
{
	const u32 width = 8;
	const u32 height = 8;
	u32 index = 0;
	GLubyte checkerImage[height][width][4];

	for(u32 y = 0; y < height; y++)
	{
		for(u32 x = 0; x < width; x++)
		{
			index = (x+y)%2;

			checkerImage[y][x][0] = index == 0? 255 :0;
			checkerImage[y][x][1] = index == 0? 255 :0;
			checkerImage[y][x][2] = index == 0? 255 :0;
			checkerImage[y][x][3] = 255;
		}
	}

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	Image* img = Image_Create();
	Image_SetGLName(img, texName, width, height, ColorFormats::RGBA);
	return img;
}



