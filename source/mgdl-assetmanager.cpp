#include <mgdl/mgdl-assetmanager.h>

void AssetManager_Init(AssetManager* manager)
{
	manager->_memoryInUse = 0;
}

void AssetManager_LoadFont(AssetManager* manager, Font* font)
{
	// TODO how much memory the coordinates take
}

void AssetManager_LoadFont(AssetManager* manager, Image* image)
{
	// TODO how much memory the image data takes extracted
}

void AssetManager_LoadPNG(AssetManager* manager, PNGFile* image)
{
	// TODO how much memory the image data takes extracted
}
