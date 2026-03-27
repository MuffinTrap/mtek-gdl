#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-alloc.h>

// TODO Singleton
static AssetManager m_manager;

void AssetManager_Init()
{
	m_manager.m_memoryInUse = 0;
	m_manager.m_textureAssets = DynamicArray_CreateTextureAsset(16);
	{
		Texture* white = Texture_GenerateColorTexture(Color_GetDefaultColor(Color_White));
		TextureAsset ta;
		ta.data = white;
		DynamicArray_AddTextureAsset(m_manager.m_textureAssets, &ta);
	}

	m_manager.m_soundAssets = DynamicArray_CreateSoundAsset(16);
	{
		Sound* silent = Sound_Create();
		Sound_InitEmpty(silent);
		SoundAsset ta; ta.data = silent;
		DynamicArray_AddSoundAsset(m_manager.m_soundAssets, &ta);
	}

	m_manager.m_imageAssets = DynamicArray_CreateImageAsset(4);
	{
		PNGFile* white = PNG_GenerateColorImage(Color_GetDefaultColor(Color_White));
		ImageAsset ta; ta.data = white;
		DynamicArray_AddImageAsset(m_manager.m_imageAssets, &ta);
	}
}

DYNAMIC_ARRAY_IMPL(TextureAsset)
DYNAMIC_ARRAY_IMPL(SoundAsset)
DYNAMIC_ARRAY_IMPL(ImageAsset)

void AssetManager_LoadFont(AssetManager* manager, Font* font)
{
	manager->m_memoryInUse += font->_characterCount * sizeof(vec2);
}

TextureHandle AssetManager_LoadTexture(const char* filename, TextureFilterModes filterMode)
{
	TextureHandle handle;
	handle.index = 0;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountTextureAsset(m_manager.m_textureAssets); i++)
	{
		TextureAsset* m = DynamicArray_GetTextureAsset(m_manager.m_textureAssets, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle.index = i;
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	Texture* texture = Texture_LoadFile(filename, filterMode);
	if (texture != nullptr)
	{
		// TODO how much memory the image data takes extracted
		// TODO Convert from image format to bytes per pixe;
		// TODO add a function to texture that tells the memory usage
		m_manager.m_memoryInUse += texture->width * texture->height *4;

		TextureAsset ta;
		ta.data = texture;
		ta.filename = zstr_from(filename);
		handle.index = DynamicArray_AddTextureAsset(m_manager.m_textureAssets, &ta);
	}
	return handle;
}

Texture* AssetManager_GetTexture(TextureHandle handle)
{
	return DynamicArray_GetTextureAsset(m_manager.m_textureAssets, handle.index)->data;
}

ImageHandle AssetManager_LoadPNG(const char* filename)
{
	ImageHandle handle;
	handle.index = 0;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountImageAsset(m_manager.m_textureAssets); i++)
	{
		ImageAsset* m = DynamicArray_GetImageAsset(m_manager.m_textureAssets, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle.index = i;
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	PNGFile* image = PNG_ReadFile(filename);
	if (image != nullptr)
	{
		m_manager.m_memoryInUse += image->width * image->height * image->bytesPerPixel;

		ImageAsset ta;
		ta.data = image;
		ta.filename = zstr_from(filename);
		handle.index = DynamicArray_AddImageAsset(m_manager.m_textureAssets, &ta);
	}
	return handle;
}

SoundHandle AssetManager_LoadSound(const char* filename, SoundFileType fileType)
{
	SoundHandle handle;
	handle.index = 0;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountSoundAsset(m_manager.m_soundAssets); i++)
	{
		SoundAsset* m = DynamicArray_GetSoundAsset(m_manager.m_soundAssets, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle.index = i;
			return handle;
		}
	}
	Sound* snd = Audio_LoadSound(filename, fileType);
	if (snd != nullptr)
	{
		m_manager.m_memoryInUse += Audio_GetSoundSizeBytes(snd);

		SoundAsset ta;
		ta.data = snd;
		ta.filename = zstr_from(filename);
		handle.index = DynamicArray_AddSoundAsset(m_manager.m_textureAssets, &ta);
	}
	return handle;
}

Sound* AssetManager_GetSound(SoundHandle handle)
{
	return DynamicArray_GetSoundAsset(m_manager.m_soundAssets, handle.index)->data;
}
