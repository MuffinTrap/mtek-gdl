#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-audio.h>

void AssetManager_Init(AssetManager* manager)
{
	manager->_memoryInUse = 0;
}

void AssetManager_LoadFont(AssetManager* manager, Font* font)
{
	manager->_memoryInUse += font->_characterCount * sizeof(vec2);
}

void AssetManager_LoadTexture(AssetManager* manager, Texture* image)
{
	// TODO how much memory the image data takes extracted
	// TODO Convert from image format to bytes per pixe;
	manager->_memoryInUse += image->width * image->height *4;
}

void AssetManager_LoadPNG(AssetManager* manager, PNGFile* image)
{
	manager->_memoryInUse += image->width * image->height * image->bytesPerPixel;
}

void AssetManager_LoadSound(AssetManager* manager, Sound* snd)
{
	manager->_memoryInUse += Audio_GetSoundSizeBytes(snd);
}

