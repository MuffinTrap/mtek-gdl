#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-memory.h>

// TODO Singleton
static AssetManager m_manager;

void AssetManager_Init()
{
	m_manager.m_memoryInUse = 0;
	m_manager.m_textureAssets = DynamicArray_CreateTextureAsset(16);
	{
		Texture* white = Texture_GenerateColorTexture(Debug_White);
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
		PNGFile* white = PNG_GenerateColorImage(Debug_White);
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
	ASSERT_DEBUG(data != nullptr);
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
	ASSERT_DEBUG(data != nullptr);
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
	ASSERT_DEBUG(data != nullptr);
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
	ASSERT_DEBUG(data != nullptr);
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

void AssetManager_LoadFont(AssetManager* manager, Texture* texture)
{
	ASSERT_DEBUG(texture != nullptr);
	manager->m_memoryInUse += texture->spriteAtlas->characterCount * sizeof(Vector2);
}

TextureHandle AssetManager_LoadTexture(const char* filename)
{
	mgdl_assert_print(m_manager.m_textureAssets != nullptr, "AssetManager not initialized!");
	TextureHandle handle = Handle_CreateTexture(0);
	DynamicArray* array = m_manager.m_textureAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountTextureAsset(array); i++)
	{
		TextureAsset* m = DynamicArray_GetTextureAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = Handle_CreateTexture(i);
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	TextureFilterModes filterMode = TextureFilterModes::Linear;
	Texture* texture = Texture_LoadFile(filename, filterMode);
	ASSERT_DEBUG(texture != nullptr);
	if (texture != nullptr)
	{
		Log_InfoF("Texture %s size is %d %d\n", filename, texture->width, texture->height);
		// TODO how much memory the image data takes extracted
		// TODO Convert from image format to bytes per pixe;
		// TODO add a function to texture that tells the memory usage
		m_manager.m_memoryInUse += texture->width * texture->height *4;

		TextureAsset ta = AssetManager_CreateTextureAsset(texture, filename);
		handle = Handle_CreateTexture((u16)DynamicArray_AddTextureAsset(array, ta));

		Log_InfoF("Texture %s got handle %u\n", filename, Handle_Index(handle));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
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
	if (Handle_Index(handle) < DynamicArray_CountTextureAsset(m_manager.m_textureAssets) && Handle_Type(handle) == Type_Texture)
	{
		return DynamicArray_GetTextureAsset(m_manager.m_textureAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetTexture got invalid handle %u\n", Handle_Index(handle));
		return DynamicArray_GetTextureAsset(m_manager.m_textureAssets, 0)->data;
	}
}

ImageHandle AssetManager_LoadPNG(const char* filename)
{
	ImageHandle handle = Handle_CreateImage(0);
	DynamicArray* array = m_manager.m_imageAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountImageAsset(array); i++)
	{
		ImageAsset* m = DynamicArray_GetImageAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = Handle_CreateImage(i);
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	PNGFile* image = PNG_ReadFile(filename);
	ASSERT_DEBUG(image != nullptr);
	if (image != nullptr)
	{
		m_manager.m_memoryInUse += image->width * image->height * image->bytesPerPixel;

		ImageAsset ta = AssetManager_CreateImageAsset(image, filename);
		handle = Handle_CreateImage((u16)DynamicArray_AddImageAsset(array, ta));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;
}

SoundHandle AssetManager_LoadSound(const char* filename, SoundFileType fileType)
{
	SoundHandle handle = Handle_CreateSound(0);
	DynamicArray* array = m_manager.m_soundAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountSoundAsset(array); i++)
	{
		SoundAsset* m = DynamicArray_GetSoundAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = Handle_CreateSound(i);
			return handle;
		}
	}
	Sound* snd = Audio_LoadSound(filename, fileType);
	ASSERT_DEBUG(snd != nullptr);
	if (snd != nullptr)
	{
		m_manager.m_memoryInUse += Audio_GetSoundSizeBytes(snd);

		SoundAsset ta = AssetManager_CreateSoundAsset(snd, filename);
		handle = Handle_CreateSound( (u16) DynamicArray_AddSoundAsset(array, ta));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;
}

Sound* AssetManager_GetSound(SoundHandle handle)
{
	if (Handle_Index(handle) < DynamicArray_CountSoundAsset(m_manager.m_soundAssets) && Handle_Type(handle) == Type_Sound)
	{
		return DynamicArray_GetSoundAsset(m_manager.m_soundAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetSound got invalid handle %u\n", Handle_Index(handle));
		return DynamicArray_GetSoundAsset(m_manager.m_soundAssets, 0)->data;
	}
}

PaletteHandle AssetManager_LoadPalette(const char* filename)
{
	PaletteHandle handle = Handle_CreatePalette(0);
	DynamicArray* array = m_manager.m_paletteAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountPaletteAsset(array); i++)
	{
		PaletteAsset* m = DynamicArray_GetPaletteAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle= Handle_CreatePalette(i);
			return handle;
		}
	}
	Palette* pal = Palette_FromPNG(filename);
	ASSERT_DEBUG(pal != nullptr);
	if (pal != nullptr)
	{
		m_manager.m_memoryInUse += Palette_GetColorAmount(pal) * sizeof(color32) + sizeof(Palette);

		PaletteAsset ta = AssetManager_CreatePaletteAsset(pal, filename);
		handle = Handle_CreatePalette((u16) DynamicArray_AddPaletteAsset(array, ta));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;
}

bool AssetManager_HasPalette(const char* filename)
{
	DynamicArray* array = m_manager.m_paletteAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < DynamicArray_CountPaletteAsset(array); i++)
	{
		PaletteAsset* m = DynamicArray_GetPaletteAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			return true;
		}
	}
	return false;
}

PaletteHandle AssetManager_LoadDefaultPalette()
{
	static const char* DefaultPaletteName = "Default";
	if (AssetManager_HasPalette(DefaultPaletteName))
	{
		return AssetManager_LoadPalette(DefaultPaletteName);
	}
	Palette* defaultPal = Palette_GetDefault();
	PaletteAsset pa = AssetManager_CreatePaletteAsset(defaultPal, "Default");
	return Handle_CreatePalette((u16)DynamicArray_AddPaletteAsset(m_manager.m_paletteAssets, pa));
}
PaletteHandle AssetManager_LoadDebugPalette()
{
	static const char* DebugPaletteName = "Debug";
	if (AssetManager_HasPalette(DebugPaletteName))
	{
		return AssetManager_LoadPalette(DebugPaletteName);
	}
	Palette* debugPal = Palette_GetDebug();
	PaletteAsset pa = AssetManager_CreatePaletteAsset(debugPal, "Debug");
	return Handle_CreatePalette( (u16)DynamicArray_AddPaletteAsset(m_manager.m_paletteAssets, pa));
}

Palette* AssetManager_GetPalette(PaletteHandle handle)
{
	if (Handle_Index(handle) < DynamicArray_CountPaletteAsset(m_manager.m_paletteAssets) && Handle_Type(handle) == Type_Palette)
	{
		return DynamicArray_GetPaletteAsset(m_manager.m_paletteAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetPalette got invalid handle %u\n", Handle_Index(handle));
		return DynamicArray_GetPaletteAsset(m_manager.m_paletteAssets, 0)->data;
	}
}
