#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-png.h>

static Palette* s_defaultPalette = nullptr;
static u32 blessing[] =
{
	0x484848FF, // 0: Neutral almost black
	// Original colors
	0x74569bFF, // 1: Dark purple
	0x96fbc7FF,  // 2: bright green
	0xf7ffaeFF, // 3: neon yellow
	0xffb3cbFF, // 4: pink
	0xd8bfd8FF, // 5: light purple/violet
	// Additions
	0xa9afa6FF, // 6: Light greenish gray
	0x7d7a76FF, // 7: Dark orange gray
};

static Palette* s_debugPalette = nullptr;
static u32 brightDos[] =
{
	0x000000FF, // 0 Black
	0x555555FF, // 1 Dark Gray
	0x5555FFFF, // 2 Blue
	0x55FF55FF, // 3 Green

	0xFF5555FF, // 4 Red
	0xFF55FFFF, // 5 Magenta
	0xFFFF55FF, // 6 Yellow
	0xFFFFFFFF  // 7 White
};


Palette* Palette_GetDefault()
{
	if (s_defaultPalette == nullptr)
	{
		s_defaultPalette = Palette_Create(blessing, 8);
	}
	return s_defaultPalette;
}

Palette* Palette_GetDebug(void)
{
	if (s_debugPalette == nullptr)
	{
		s_debugPalette = Palette_Create(brightDos, 8);
	}
	return s_debugPalette;
}

Palette* Palette_Create(u32* colorsArray, u8 size)
{
	Palette* palette = (Palette*)malloc(sizeof(Palette));
	palette->m_colors = (Color4f*)malloc(sizeof(Color4f) * size);
	for(int i = 0; i < size; i++)
	{
		palette->m_colors[i] = Color_HexToFloats(colorsArray[i]);
	}
	palette->m_size = size;
	return palette;
}

Palette* Palette_Create4f(Color4f* colorsArray, u8 size)
{
	Palette* palette = (Palette*)malloc(sizeof(Palette));
	palette->m_colors = (Color4f*)malloc(sizeof(Color4f) * size);
	for(int i = 0; i < size; i++)
	{
		palette->m_colors[i] = colorsArray[i];
	}
	palette->m_size = size;
	return palette;
}


Palette* Palette_CreateEmpty(u8 size)
{
	Palette* palette = (Palette*)malloc(sizeof(Palette));
	palette->m_colors = (Color4f*)malloc(sizeof(Color4f)*size);
	palette->m_size = size;
	return palette;

}

u32 Palette_GetColor(Palette* palette, u8 index)
{
	index = index % palette->m_size;
	return Color_FloatsToHex(palette->m_colors[index]);
}

Color4f Palette_GetColor4f(Palette* palette, u8 index)
{
	index = index % palette->m_size;
	return palette->m_colors[index];
}

Color4f* Palette_GetColor4fPtr(Palette* palette, u8 index)
{
	index = index % palette->m_size;
	return &palette->m_colors[index];
}
void Palette_SetColor4f(Palette* palette, u8 index, Color4f color)
{
	index = index % palette->m_size;
	palette->m_colors[index] = color;
}

void Palette_SetColor4fPtr(Palette* palette, u8 index, Color4f* color)
{
	index = index % palette->m_size;
	palette->m_colors[index] = Color_CreateFromPointer4f(color);
}

void Palette_SetColor(Palette* palette, u8 index, u32 color)
{
	index = index % palette->m_size;
	palette->m_colors[index] = Color_HexToFloats(color);
}

void Palette_Delete(Palette* palette)
{
	if (palette != NULL)
	{
		if (palette->m_colors != NULL)
		{
			free(palette->m_colors);
			palette->m_colors = NULL;
		}
		free(palette);
	}
}

u8 Palette_GetColorAmount(Palette* palette)
{
	return palette->m_size;
}

Palette* Palette_FromPNG(const char* filename)
{
	PNGFile* png = PNG_ReadFile(filename);
	if (png != nullptr)
	{
		Palette* pal = Palette_CreateEmpty(png->width);
		for (GLsizei i = 0; i < png->width; i++)
		{
			Color4b col = PNG_GetRGBA(png, i, 0);
			Palette_SetColor4f(pal, i, Color_CreateFrom4b(col));
		}
		PNG_DeleteData(png);
		return pal;
	}
	return nullptr;
}
