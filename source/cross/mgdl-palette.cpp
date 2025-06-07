#include <mgdl/mgdl-palette.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-util.h>

static Palette* defaultPalette_ = nullptr;
static u32 blessing[] =
{
	0x484848FF, // 0: Neutral almost black
	// Original colors
	0x74569bFF, // 1: Dark purple
	0x96fbcFF,  // 2: bright green
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
	Palette* palette = new Palette();
	palette->_colors = colorsArray;
	palette->size = size;
	return palette;
}


Palette* Palette_CreateEmpty(u8 size)
{
	Palette* palette = new Palette();
	palette->_colors = (u32*)malloc(sizeof(u32)*size);
	palette->size = size;
	return palette;

}

u32 Palette_GetColor(Palette* palette, u8 index)
{
	index = index % palette->size;
	return palette->_colors[index];
}

Color4f Palette_GetColor4f(Palette* palette, u8 index)
{
	index = index % palette->size;
	return ColorToFloats(palette->_colors[index]);
}

void Palette_SetColor(Palette* palette, u8 index, u32 color)
{
	index = index % palette->size;
	palette->_colors[index] = color;
}
