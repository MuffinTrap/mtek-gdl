#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-png.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

gdl::Image* Image_LoadFile ( const char* filename, gdl::TextureFilterModes filterMode)
{
	// Load using png
	printf("Loading image %s\n", filename);

	PNGFile* pngFile = PNG_ReadFile(filename);
	if (pngFile == nullptr)
	{
		return nullptr;
	}
	gdl::Image* image =  Image_LoadPNG(pngFile, filterMode);

	// Data is loaded to OpenGX, release the buffers
	PNG_DeleteData(pngFile);
	return image;
}

// The png might belong to someone else, do not free it in this function
gdl::Image* Image_LoadPNG(PNGFile* png, gdl::TextureFilterModes filterMode)
{
	gdl::Image* image = new gdl::Image();
	GLint glFilter = gdl::Image::TextureFilterToGLFilter(filterMode);

	GLint alignment;
	glGenTextures(1, &image->textureId);
	glBindTexture(GL_TEXTURE_2D, image->textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

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

	return image;
}

void gdl::Image::SetGLName(GLuint textureName, GLsizei width, GLsizei height, gdl::ColorFormats format)
{
	this->width = width;
	this->height = height;
	this->textureId = textureName;
	this->colorFormat = format;
}

// TODO add padding to UVs so that the corners are inside the pixels and not in between
void gdl::Image::Draw2DAbsolute(short x, short y, short x2, short y2)
{
	float dx = (float)x;
	float dy = (float)y;
	float dx2 = (float)x2;
	float dy2 = (float)y2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);
		glColor3f(red, green, blue);

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

void gdl::Image::Draw2DAligned(short x, short y, float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY)
{
	short w = width * scale;
	short h = height * scale;


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
	Draw2DAbsolute(x, y, x+w, y+h);
}

void gdl::Image::Draw3D(float scale, gdl::AlignmentModes alignX, gdl::AlignmentModes alignY)
{
	float aspect = (float)width/(float)height;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float hs = scale/2.0f;

	if (alignX == RJustify)
	{
		x -= scale * aspect;;
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
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);
		glColor3f(red, green, blue);

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

GLint gdl::Image::TextureFilterToGLFilter(gdl::TextureFilterModes filterMode)
{
	switch(filterMode)
	{
		case gdl::TextureFilterModes::Linear:
			return GL_LINEAR;
			break;
		case gdl::TextureFilterModes::Nearest:
			return GL_NEAREST;
			break;
		case gdl::TextureFilterModes::LN_MM_LN:
			return GL_LINEAR_MIPMAP_LINEAR;
			break;
		case gdl::TextureFilterModes::LN_MM_NR:
			return GL_LINEAR_MIPMAP_NEAREST;
			break;
		case gdl::TextureFilterModes::NR_MM_LN:
			return GL_NEAREST_MIPMAP_LINEAR;
			break;
		case gdl::TextureFilterModes::NR_MM_NR:
			return GL_NEAREST_MIPMAP_NEAREST;
			break;
		default:
			return GL_LINEAR;
			break;
	};
}

GLuint gdl::Image::GetTextureId()
{
	return textureId;
}

short gdl::Image::GetHeight()
{
	return height;
}

short gdl::Image::GetWidth()
{
	return width;
}

float gdl::Image::GetAspectRatio()
{
	return aspectRatio;
}

gdl::ColorFormats gdl::Image::GetColorFormat()
{
	return colorFormat;
}

void gdl::Image::SetTint ( float red, float green, float blue )
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}



