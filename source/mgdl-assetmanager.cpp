#include <mgdl/mgdl-assetmanager.h>

void AssetManager_Init(AssetManager* manager)
{
	manager->_memoryInUse = 0;
}

void AssetManager_LoadFont(AssetManager* manager, Font* font)
{
	// TODO how much memory the coordinates take
}

void AssetManager_LoadFont(AssetManager* manager, gdl::Image* image)
{
	// TODO how much memory the image data takes extracted
}
