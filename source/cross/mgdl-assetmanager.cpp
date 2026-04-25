#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-alloc.h>

// TODO Singleton
static AssetManager m_manager;

void AssetManager_Init()
{
	m_manager.m_memoryInUse = 0;
	m_manager.m_textureAssets = DynamicArray_CreateTextureAsset(16);
	{
		Texture* white = Texture_GenerateColorTexture(Color_GetDefaultColor(Color_White));
		TextureAsset ta = AssetManager_CreateTextureAsset(white, nullptr);
		DynamicArray_AddTextureAsset(m_manager.m_textureAssets, ta);
	}

	m_manager.m_soundAssets = DynamicArray_CreateSoundAsset(16);
	{
		Sound* silent = Sound_Create();
		Sound_InitEmpty(silent);
		SoundAsset ta = AssetManager_CreateSoundAsset(silent, nullptr);
		DynamicArray_AddSoundAsset(m_manager.m_soundAssets, ta);
	}

	m_manager.m_imageAssets = DynamicArray_CreateImageAsset(4);
	{
		PNGFile* white = PNG_GenerateColorImage(Color_GetDefaultColor(Color_White));
		ImageAsset ta = AssetManager_CreateImageAsset(white, nullptr);
		DynamicArray_AddImageAsset(m_manager.m_imageAssets, ta);
	}

	m_manager.m_paletteAssets= DynamicArray_CreatePaletteAsset(4);
}

DYNAMIC_ARRAY_IMPL(TextureAsset)
DYNAMIC_ARRAY_IMPL(SoundAsset)
DYNAMIC_ARRAY_IMPL(ImageAsset)
DYNAMIC_ARRAY_IMPL(PaletteAsset)

TextureAsset AssetManager_CreateTextureAsset(Texture* data, const char* filename)
{
	TextureAsset ta;
	ta.data = data;
	if (filename != nullptr)
	{
		ta.filename = zstr_from(filename);
	}
	else
	{
		ta.filename = zstr_init();
	}
	return ta;
}
SoundAsset AssetManager_CreateSoundAsset(Sound* data, const char* filename)
{
	SoundAsset ta;
	ta.data = data;
	if (filename != nullptr)
	{
		ta.filename = zstr_from(filename);
	}
	else
	{
		ta.filename = zstr_init();
	}
	return ta;

}
ImageAsset AssetManager_CreateImageAsset(PNGFile* data, const char* filename)
{
	ImageAsset ta;
	ta.data = data;
	if (filename != nullptr)
	{
		ta.filename = zstr_from(filename);
	}
	else
	{
		ta.filename = zstr_init();
	}
	return ta;
}
PaletteAsset AssetManager_CreatePaletteAsset(Palette* data, const char* filename)
{
	PaletteAsset ta;
	ta.data = data;
	if (filename != nullptr)
	{
		ta.filename = zstr_from(filename);
	}
	else
	{
		ta.filename = zstr_init();
	}
	return ta;
}

void AssetManager_LoadFont(AssetManager* manager, Font* font)
{
	manager->m_memoryInUse += font->_characterCount * sizeof(vec2);
}

TextureHandle AssetManager_LoadTexture(const char* filename)
{
	mgdl_assert_print(m_manager.m_textureAssets != nullptr, "AssetManager not initialized!");
	TextureHandle handle;
	DynamicArray* array = m_manager.m_textureAssets;
	handle = 0;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountTextureAsset(array); i++)
	{
		TextureAsset* m = DynamicArray_GetTextureAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = i;
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	TextureFilterModes filterMode = TextureFilterModes::Linear;
	Texture* texture = Texture_LoadFile(filename, filterMode);
	Log_InfoF("Texture %s size is %d %d\n", filename, texture->width, texture->height);
	if (texture != nullptr)
	{
		// TODO how much memory the image data takes extracted
		// TODO Convert from image format to bytes per pixe;
		// TODO add a function to texture that tells the memory usage
		m_manager.m_memoryInUse += texture->width * texture->height *4;

		TextureAsset ta = AssetManager_CreateTextureAsset(texture, filename);
		handle = DynamicArray_AddTextureAsset(array, ta);

		Log_InfoF("Texture %s got handle %u\n", filename, handle);
	}
	return handle;
}

void AssetManager_PrintLoadedTextures()
{
	for(sizetype i = 0; i < DynamicArray_CountTextureAsset(m_manager.m_textureAssets); i++)
	{
		TextureAsset* m = DynamicArray_GetTextureAsset(m_manager.m_textureAssets, i);
		mgdl_assert_printf(m !=nullptr, "Null asset in dynamic array index %u", i);
		Texture* texture = m->data;
		mgdl_assert_print(texture !=nullptr, "Null pointer in asset struct");
		Log_InfoF("Texture %s size is %d %d\n", zstr_cstr(&m->filename), texture->width, texture->height);
	}
}

Texture* AssetManager_GetTexture(TextureHandle handle)
{
	if (handle < DynamicArray_CountTextureAsset(m_manager.m_textureAssets))
	{
		return DynamicArray_GetTextureAsset(m_manager.m_textureAssets, handle)->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetTexture got invalid handle %u\n", handle);
		return DynamicArray_GetTextureAsset(m_manager.m_textureAssets, 0)->data;
	}
}

ImageHandle AssetManager_LoadPNG(const char* filename)
{
	ImageHandle handle;
	handle = 0;
	DynamicArray* array = m_manager.m_imageAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountImageAsset(array); i++)
	{
		ImageAsset* m = DynamicArray_GetImageAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = i;
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	PNGFile* image = PNG_ReadFile(filename);
	if (image != nullptr)
	{
		m_manager.m_memoryInUse += image->width * image->height * image->bytesPerPixel;

		ImageAsset ta = AssetManager_CreateImageAsset(image, filename);
		handle = DynamicArray_AddImageAsset(array, ta);
	}
	return handle;
}

SoundHandle AssetManager_LoadSound(const char* filename, SoundFileType fileType)
{
	SoundHandle handle;
	handle = 0;
	DynamicArray* array = m_manager.m_soundAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountSoundAsset(array); i++)
	{
		SoundAsset* m = DynamicArray_GetSoundAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = i;
			return handle;
		}
	}
	Sound* snd = Audio_LoadSound(filename, fileType);
	if (snd != nullptr)
	{
		m_manager.m_memoryInUse += Audio_GetSoundSizeBytes(snd);

		SoundAsset ta = AssetManager_CreateSoundAsset(snd, filename);
		handle = DynamicArray_AddSoundAsset(array, ta);
	}
	return handle;
}

Sound* AssetManager_GetSound(SoundHandle handle)
{
	return DynamicArray_GetSoundAsset(m_manager.m_soundAssets, handle)->data;
}

PaletteHandle AssetManager_LoadPalette(const char* filename)
{
	PaletteHandle handle = 0;
	DynamicArray* array = m_manager.m_paletteAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountPaletteAsset(array); i++)
	{
		PaletteAsset* m = DynamicArray_GetPaletteAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = i;
			return handle;
		}
	}
	Palette* pal = Palette_FromPNG(filename);
	if (pal != nullptr)
	{
		m_manager.m_memoryInUse += Palette_GetColorAmount(pal) * sizeof(Color4f) + sizeof(Palette);

		PaletteAsset ta = AssetManager_CreatePaletteAsset(pal, filename);
		handle = DynamicArray_AddPaletteAsset(array, ta);
	}
	return handle;
}

PaletteHandle AssetManager_LoadDefaultPalette()
{
	Palette* defaultPal = Palette_GetDefault();
	PaletteAsset pa = AssetManager_CreatePaletteAsset(defaultPal, "Default");
	return DynamicArray_AddPaletteAsset(m_manager.m_paletteAssets, pa);
}
PaletteHandle AssetManager_LoadDebugPalette()
{
	Palette* debugPal = Palette_GetDebug();
	PaletteAsset pa = AssetManager_CreatePaletteAsset(debugPal, "Debug");
	return DynamicArray_AddPaletteAsset(m_manager.m_paletteAssets, pa);
}

Palette* AssetManager_GetPalette(PaletteHandle handle)
{
	return DynamicArray_GetPaletteAsset(m_manager.m_paletteAssets, handle)->data;
}
