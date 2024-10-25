#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-cache.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-png.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>

bool gdl::Image::LoadBuffer (const u8* buffer, size_t size, gdl::TextureFilterModes filterMode )
{
	// Load using png
	printf("gdl:: Loading image buffer\n");
	pngFile = new PNGFile();
	if (pngFile->ReadBuffer(buffer, size) == false)
	{
		return false;
	}
	bool loadOk =  LoadPNG(pngFile, filterMode);
	// Data is loaded to OpenGX, release the buffers
	delete pngFile;
	pngFile = nullptr;
	return loadOk;
}

bool gdl::Image::LoadFile ( const char* filename, gdl::TextureFilterModes filterMode)
{
	// Load using png
	printf("Loading image %s\n", filename);

	pngFile = new PNGFile();
	if (pngFile->ReadFile(filename) == false)
	{
		return false;
	}
	bool loadOk =  LoadPNG(pngFile, filterMode);

	// Data is loaded to OpenGX, release the buffers
	delete pngFile;
	pngFile = nullptr;
	return loadOk;
}

// The png might belong to someone else, do not free it in this function
bool gdl::Image::LoadPNG(PNGFile* png, gdl::TextureFilterModes filterMode)
{
	GLint glFilter = TextureFilterToGLFilter(filterMode);

	GLint alignment;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);

	// Load to OpenGL

	// Read current alignment and set alignment to 1
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// We dont have mipmaps..
	glTexImage2D(GL_TEXTURE_2D, 0, png->GetBytesPerPixel(),
			  png->GetWidth(), png->GetHeight(),
			  0, png->GetGLFormat(),
			  GL_UNSIGNED_BYTE,
			  png->GetTexels());

	// restore previous alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	// copy data
	width = png->GetWidth();
	height = png->GetHeight();

	return true;
}

void gdl::Image::Draw2DAbsolute(short x, short y, short x2, short y2)
{
	float dx = (float)x;
	float dy = (float)y;
	float width = (float)x2-dx;
	float height = (float)y2-dy;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);

		// Lower left
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(dx, dy-height);
		// Lower right
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(dx+width, dy-height);
		// Upper right
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(dx+width, dy);
		// Upper left
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(dx, dy);

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
		y += h/2;
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
		glColor3f(1.0f, 1.0f, 1.0f);

		// Lower left
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x-aspect*hs, y-hs, z);

		// Lower right
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x+aspect*hs, y-hs, z);

		// Upper right
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x+aspect*hs, y+hs, z);

		// Upper left
		glTexCoord2f(0.0f, 0.0f);
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


