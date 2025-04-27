#include <mgdl/mgdl-assetmanager.h>

void AssetManager_Init(AssetManager* manager)
{
	manager->_memoryInUse = 0;
}

void AssetManager_LoadFont(AssetManager* manager, Font* font)
{
	// TODO how much memory the coordinates take
	manager->_memoryInUse += font->_characterCount * sizeof(vec2);
}

void AssetManager_LoadImage(AssetManager* manager, Image* image)
{
	// TODO how much memory the image data takes extracted
	// TODO Convert from image format to bytes per pixe;
	manager->_memoryInUse += image->width * image->height *4;
}

void AssetManager_LoadPNG(AssetManager* manager, PNGFile* image)
{
	// TODO how much memory the image data takes extracted
	manager->_memoryInUse += image->width * image->height * image->bytesPerPixel;
}
