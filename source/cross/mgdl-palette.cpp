#include <mgdl/mgdl-palette.h>

static gdl::Palette* defaultPalette = nullptr;
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

gdl::Palette* gdl::Palette::GetDefaultPalette()
{
	if (defaultPalette == nullptr)
	{
		defaultPalette = new gdl::Palette(blessing, 8);
	}
	return defaultPalette;
}

gdl::Palette::Palette(u32* colorsArray, u8 size)
{
	this->colors = colorsArray;
	this->size = size;
}


u32 gdl::Palette::GetColor(u8 index)
{
	index = index % size;
	return colors[index];
}

u8 gdl::Palette::GetSize()
{
	return size;
}
