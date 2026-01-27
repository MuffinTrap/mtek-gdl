#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-util.h>

static Palette* defaultPalette_ = nullptr;
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

Palette* Palette_GetDefault()
{
	if (defaultPalette_ == nullptr)
	{
		defaultPalette_ = Palette_Create(blessing, 8);
	}
	return defaultPalette_;
}

Palette* Palette_Create(u32* colorsArray, u8 size)
{
	Palette* palette = (Palette*)malloc(sizeof(Palette));
	palette->_colors = (Color4f*)malloc(sizeof(Color4f) * size);
	for(int i = 0; i < size; i++)
	{
		palette->_colors[i] = Color_HexToFloats(colorsArray[i]);
	}
	palette->size = size;
	return palette;
}

Palette* Palette_Create4f(Color4f* colorsArray, u8 size)
{
	Palette* palette = (Palette*)malloc(sizeof(Palette));
	palette->_colors = (Color4f*)malloc(sizeof(Color4f) * size);
	for(int i = 0; i < size; i++)
	{
		palette->_colors[i] = colorsArray[i];
	}
	palette->size = size;
	return palette;
}


Palette* Palette_CreateEmpty(u8 size)
{
	Palette* palette = (Palette*)malloc(sizeof(Palette));
	palette->_colors = (Color4f*)malloc(sizeof(u32)*size);
	palette->size = size;
	return palette;

}

u32 Palette_GetColor(Palette* palette, u8 index)
{
	index = index % palette->size;
	return Color_FloatsToHex(palette->_colors[index]);
}

Color4f Palette_GetColor4f(Palette* palette, u8 index)
{
	index = index % palette->size;
	return palette->_colors[index];
}

void Palette_SetColor(Palette* palette, u8 index, Color4f* color)
{
	index = index % palette->size;
	palette->_colors[index] = Color_CreateFromPointer4f(color);
}

void Palette_Delete(Palette* palette)
{
	if (palette != NULL)
	{
		if (palette->_colors != NULL)
		{
			free(palette->_colors);
			palette->_colors = NULL;
		}
		free(palette);
	}
}

u8 Palette_GetColorAmount(Palette* palette)
{
	return palette->size;
}
