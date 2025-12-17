#include <mgdl/mgdl-assetmanager.h>

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
	manager->_memoryInUse += snd->sizeBytes;
}

void AssetManager_LoadMusic(AssetManager* manager, Music* music)
{
	switch(music->type)
	{
		case MusicOgg:
			manager->_memoryInUse += 1024; // TODO What is the buffer size?
			break;
		case MusicWav:
			manager->_memoryInUse += music->wav->sizeBytes;
			break;
	}
}
