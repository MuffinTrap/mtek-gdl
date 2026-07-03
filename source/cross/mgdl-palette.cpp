#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-memory.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-assert.h>

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
	Debug_Black,
	Debug_DarkGray,
	Debug_Blue,
	Debug_Green,
	Debug_Red,
	Debug_Magenta,
	Debug_Yellow,
	Debug_White
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
	ASSERT_DEBUG(colorsArray !=  nullptr);
	Palette* palette = Palette_CreateEmpty(size);
	for(int i = 0; i < size; i++)
	{
		palette->m_colors[i] = colorsArray[i];
	}
	palette->m_size = size;
	return palette;
}


Palette* Palette_CreateEmpty(u8 size)
{
	Palette* palette = (Palette*)mgdl_AllocateGeneralMemory(sizeof(Palette));
	palette->m_colors = (color32*)mgdl_AllocateGeneralMemory(sizeof(color32)*size);
	palette->m_size = size;
	return palette;

}

color32 Palette_GetColor(Palette* palette, u8 index)
{
	ASSERT_DEBUG(palette !=  nullptr);
	index = index % palette->m_size;
	return (palette->m_colors[index]);
}


void Palette_SetColor(Palette* palette, u8 index, color32 color)
{
	ASSERT_DEBUG(palette !=  nullptr);
	index = index % palette->m_size;
	palette->m_colors[index] = (color);
}


void Palette_Delete(Palette* palette)
{
	if (palette != nullptr)
	{
		if (palette->m_colors != nullptr)
		{
			free(palette->m_colors);
			palette->m_colors = nullptr;
		}
		free(palette);
	}
}

u8 Palette_GetColorAmount(Palette* palette)
{
	ASSERT_DEBUG(palette !=  nullptr);
	return palette->m_size;
}

Palette* Palette_FromPNG(const char* filename)
{
	PNGFile* png = PNG_ReadFile(filename);
	ASSERT_DEBUG(png !=  nullptr);
	sizetype colorIndex = 0;
	bool duplicate = false;
	if (png != nullptr)
	{
		Palette* pal = Palette_CreateEmpty(png->width * png->width);
		for (GLsizei row = 0; row < png->height; row++)
		{
			for (GLsizei i = 0; i < png->width; i++)
			{
				color32 col = PNG_GetRGBA(png, i, row);

				// Check for duplicates
				for (sizetype index = 0; index < colorIndex; index++)
				{
					if (Palette_GetColor(pal, index) == col)
					{
						duplicate = true;
						break;
					}
				}
				if (duplicate == false)
				{
					Palette_SetColor(pal, colorIndex, col);
					colorIndex += 1;
				}
				duplicate = false;
			}
		}
		PNG_DeleteData(png);
		pal->m_size = colorIndex;
		return pal;
	}
	return nullptr;
}
