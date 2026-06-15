#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-dynamic_array.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-memory.h>
#include <mgdl/mgdl-fbx.h>

// TODO Singleton
static AssetManager m_manager;

void AssetManager_Init()
{
	m_manager.m_memoryInUse = 0;
	m_manager.m_textureAssets = ObjectArray_Create_TextureAsset(16);
	{
		Texture* white = Texture_GenerateColorTexture(Debug_White);
		TextureAsset ta = AssetManager_CreateTextureAsset(white, nullptr);
		ObjectArray_Add_TextureAsset(m_manager.m_textureAssets, ta);
	}

	m_manager.m_soundAssets = ObjectArray_Create_SoundAsset(16);
	{
		Sound* silent = Sound_Create();
		Sound_InitEmpty(silent);
		SoundAsset ta = AssetManager_CreateSoundAsset(silent, nullptr);
		ObjectArray_Add_SoundAsset(m_manager.m_soundAssets, ta);
	}

	m_manager.m_imageAssets = ObjectArray_Create_ImageAsset(4);
	{
		PNGFile* white = PNG_GenerateColorImage(Debug_White);
		ImageAsset ta = AssetManager_CreateImageAsset(white, nullptr);
		ObjectArray_Add_ImageAsset(m_manager.m_imageAssets, ta);
	}

	m_manager.m_paletteAssets= ObjectArray_Create_PaletteAsset(4);

	m_manager.m_sceneAssets = ObjectArray_Create_SceneAsset(2);

	m_manager.m_modelAssets = ObjectArray_Create_ModelAsset(4);
}

OBJECT_ARRAY_IMPLEMENT(TextureAsset)
OBJECT_ARRAY_IMPLEMENT(SoundAsset)
OBJECT_ARRAY_IMPLEMENT(ImageAsset)
OBJECT_ARRAY_IMPLEMENT(PaletteAsset)
OBJECT_ARRAY_IMPLEMENT(SceneAsset)
OBJECT_ARRAY_IMPLEMENT(ModelAsset)

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

SceneAsset AssetManager_CreateSceneAsset(Scene* data, const char* filename)
{
	ASSERT_DEBUG(data != nullptr);
	SceneAsset ta;
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
ModelAsset AssetManager_CreateModelAsset(Model* data, const char* filename)
{
	ASSERT_DEBUG(data != nullptr);
	ModelAsset ta;
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
	ObjectArray* array = m_manager.m_textureAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		TextureAsset* m = ObjectArray_Get_TextureAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle = Handle_CreateTexture(i);
			return handle;
		}
	}

	// TODO Read settings from asset configuration file

	TextureFilterModes filterMode = TextureFilterModes::Nearest;
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
		handle = Handle_CreateTexture((u16)ObjectArray_Add_TextureAsset(array, ta));

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
	for(sizetype i = 0; i < ObjectArray_Count(m_manager.m_textureAssets); i++)
	{
		TextureAsset* m = ObjectArray_Get_TextureAsset(m_manager.m_textureAssets, i);
		mgdl_assert_printf(m !=nullptr, "Null asset in dynamic array index %u", i);
		Texture* texture = m->data;
		mgdl_assert_print(texture !=nullptr, "Null pointer in asset struct");
		Log_InfoF("Texture %s size is %d %d\n", zstr_cstr(&m->filename), texture->width, texture->height);
	}
}

Texture* AssetManager_GetTexture(TextureHandle handle)
{
	if (Handle_Index(handle) < ObjectArray_Count(m_manager.m_textureAssets) && Handle_Type(handle) == Type_Texture)
	{
		return ObjectArray_Get_TextureAsset(m_manager.m_textureAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetTexture got invalid handle %u\n", Handle_Index(handle));
		return ObjectArray_Get_TextureAsset(m_manager.m_textureAssets, 0)->data;
	}
}

ImageHandle AssetManager_LoadPNG(const char* filename)
{
	ImageHandle handle = Handle_CreateImage(0);
	ObjectArray* array = m_manager.m_imageAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		ImageAsset* m = ObjectArray_Get_ImageAsset(array, i);
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
		handle = Handle_CreateImage((u16)ObjectArray_Add_ImageAsset(array, ta));
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
	ObjectArray* array = m_manager.m_soundAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		SoundAsset* m = ObjectArray_Get_SoundAsset(array, i);
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
		handle = Handle_CreateSound( (u16) ObjectArray_Add_SoundAsset(array, ta));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;
}

Sound* AssetManager_GetSound(SoundHandle handle)
{
	if (Handle_Index(handle) < ObjectArray_Count(m_manager.m_soundAssets) && Handle_Type(handle) == Type_Sound)
	{
		return ObjectArray_Get_SoundAsset(m_manager.m_soundAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetSound got invalid handle %u\n", Handle_Index(handle));
		return ObjectArray_Get_SoundAsset(m_manager.m_soundAssets, 0)->data;
	}
}

PaletteHandle AssetManager_LoadPalette(const char* filename)
{
	PaletteHandle handle = Handle_CreatePalette(0);
	ObjectArray* array = m_manager.m_paletteAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		PaletteAsset* m = ObjectArray_Get_PaletteAsset(array, i);
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
		handle = Handle_CreatePalette((u16) ObjectArray_Add_PaletteAsset(array, ta));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;
}

bool AssetManager_HasPalette(const char* filename)
{
	ObjectArray* array = m_manager.m_paletteAssets;
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		PaletteAsset* m = ObjectArray_Get_PaletteAsset(array, i);
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
	return Handle_CreatePalette((u16)ObjectArray_Add_PaletteAsset(m_manager.m_paletteAssets, pa));
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
	return Handle_CreatePalette( (u16)ObjectArray_Add_PaletteAsset(m_manager.m_paletteAssets, pa));
}

Palette* AssetManager_GetPalette(PaletteHandle handle)
{
	if (Handle_Index(handle) < ObjectArray_Count(m_manager.m_paletteAssets) && Handle_Type(handle) == Type_Palette)
	{
		return ObjectArray_Get_PaletteAsset(m_manager.m_paletteAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetPalette got invalid handle %u\n", Handle_Index(handle));
		return ObjectArray_Get_PaletteAsset(m_manager.m_paletteAssets, 0)->data;
	}
}

Scene* AssetManager_GetScene(SceneHandle handle)
{
	if (Handle_Index(handle) < ObjectArray_Count(m_manager.m_sceneAssets) && Handle_Type(handle) == Type_Scene)
	{
		return ObjectArray_Get_SceneAsset(m_manager.m_sceneAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetScene got invalid handle %u\n", Handle_Index(handle));
		return nullptr; // NO default asset
	}
}

SceneHandle AssetManager_LoadScene(const char* filename)
{
	SceneHandle handle = Handle_CreateScene(0);
	ObjectArray* array = m_manager.m_sceneAssets;
	ASSERT_DEBUG(array != nullptr);

	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);
	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		SceneAsset* m = ObjectArray_Get_SceneAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle= Handle_CreateScene(i);
			return handle;
		}
	}
	Scene* scene = FBX_Load(filename);
	ASSERT_DEBUG(scene != nullptr);
	if (scene != nullptr)
	{
		// TODO
		// m_manager.m_memoryInUse += Palette_GetColorAmount(pal) * sizeof(color32) + sizeof(Palette);

		SceneAsset ta = AssetManager_CreateSceneAsset(scene, filename);
		handle = Handle_CreateScene((u16) ObjectArray_Add_SceneAsset(array, ta));
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;
}

ModelHandle AssetManager_LoadModel(const char* filename)
{
	ModelHandle handle = Handle_CreateModel(0);
	ObjectArray* array = m_manager.m_modelAssets;
	ASSERT_DEBUG(array != nullptr);
	// Check if already loaded
	zstr_view filenameView = zstr_view_from(filename);

	for(sizetype i = 0; i < ObjectArray_Count(array); i++)
	{
		ModelAsset* m = ObjectArray_Get_ModelAsset(array, i);
		zstr_view handleView = zstr_as_view(&m->filename);
		if (zstr_view_eq_view(filenameView, handleView))
		{
			handle= Handle_CreateModel(i);
			return handle;
		}
	}
	Model* model = FBX_LoadFirstModel(filename);
	ASSERT_DEBUG(model != nullptr);
	if (model != nullptr)
	{
		// TODO
		// m_manager.m_memoryInUse += Palette_GetColorAmount(pal) * sizeof(color32) + sizeof(Palette);

		ModelAsset ta = AssetManager_CreateModelAsset(model, filename);
		u16 indexInArray = (u16) ObjectArray_Add_ModelAsset(array, ta);
		handle = Handle_CreateModel(indexInArray);
	}
	else
	{
		handle = MGDL_INVALID_HANDLE;
	}
	return handle;

}
Model* AssetManager_GetModel(ModelHandle handle)
{
	if (Handle_Index(handle) < ObjectArray_Count(m_manager.m_modelAssets) && Handle_Type(handle) == Type_Model)
	{
		return ObjectArray_Get_ModelAsset(m_manager.m_modelAssets, Handle_Index(handle))->data;
	}
	else
	{
		Log_ErrorF("AssetManager_GetModel got invalid handle %u\n", Handle_Index(handle));
		return nullptr; // NO default asset
	}
}
