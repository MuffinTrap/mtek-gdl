#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-fbx.h>

#ifdef GEKKO
#include <mgdl/wii/mgdl-wii-sound.h>
#else
#include <mgdl/pc/mgdl-pc-sound.h>
#endif

static AssetManager assetManager;

void InitSystem(const char* name,
	gdl::ScreenAspect screenAspect,
								std::function<void()> initCallback,
								std::function<void()> updateCallback,
								std::function<void()> drawCallback,
								u32 initFlags)
{
	Platform_Init(name, screenAspect, initCallback, updateCallback, drawCallback, initFlags);
	AssetManager_Init(&assetManager);
}

PNGFile* LoadPNG(const char* filename)
{
	PNGFile* png = PNG_ReadFile(filename);
	if (png != nullptr)
	{
		AssetManager_LoadPNG(&assetManager, png);
		return png;
	}
	else
	{
		delete png;
		return nullptr;
	}
}

Image* LoadImage(const char* filename, gdl::TextureFilterModes filterMode)
{
	Image* img = Image_LoadFile(filename, filterMode);
	if (img != nullptr)
	{
		AssetManager_LoadImage(&assetManager, img);
		return img;
	}
	else
	{
		return nullptr;
	}
}

Image* LoadImagePNG(PNGFile* png, gdl::TextureFilterModes filterMode)
{
	Image* img = Image_LoadPNG(png, filterMode);
	if (img != nullptr)
	{
		return img;
	}
	else
	{
		delete img;
		return nullptr;
	}
}

Sound* LoadSound(const char* filename)
{
	Sound* snd = Sound_Load(filename);
	if(snd != nullptr)
	{
		AssetManager_LoadSound(&assetManager, snd);
		return snd;
	}
	else
	{
		return nullptr;
	}
}


Music* LoadOgg(const char* filename)
{
	Music* music = Music_Load(filename);
	if(music != nullptr)
	{
		return music;
	}
	else
	{
		return nullptr;
	}
}

Font* LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter)
{
	Image* fontImage = LoadImage(filename, gdl::TextureFilterModes::Linear);
	Font* font = Font_Load(fontImage, characterWidth, characterHeight, firstCharacter);
	if (font != nullptr)
	{
		AssetManager_LoadFont(&assetManager, font);
		return font;
	}
	else
	{
		return nullptr;
	}
}

Font* LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow)
{
	Image* fontImage = LoadImage(filename, gdl::TextureFilterModes::Linear);
	Font* font = Font_LoadPadded(fontImage, characterWidth, characterHeight, firstCharacter, charactersPerRow);

	if (font != nullptr)
	{
		AssetManager_LoadFont(&assetManager, font);
		return font;
	}
	else
	{
		return nullptr;
	}
}

Font* LoadFontCustom(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters)
{
	Image* fontImage = LoadImage(filename, gdl::TextureFilterModes::Linear);
	Font* font = Font_LoadSelective(fontImage, characterWidth, characterHeight, charactersPerRow, characters);
	if (font != nullptr)
	{
		AssetManager_LoadFont(&assetManager, font);
		return font;
	}
	else
	{
		return nullptr;
	}
}

Scene* LoadFBX(const char* filename)
{
	Scene* s = FBX_Load(filename);

	// TODO calculate size
	return s;
}

WiiController* GetController( int controllerNumber)
{
	return Platform_GetController(controllerNumber);
}

void DoProgramExit()
{
	Platform_DoProgramExit();
}

u16 GetScreenWidth() { return Platform_GetSingleton()->screenWidth; }
u16 GetScreenHeight(){ return Platform_GetSingleton()->screenHeight; }
float GetAspectRatio(){ return Platform_GetSingleton()->aspectRatio; }
float GetElapsedSeconds(){ return Platform_GetElapsedSeconds(); };
float GetDeltaTime(){ return Platform_GetDeltaTime(); };
