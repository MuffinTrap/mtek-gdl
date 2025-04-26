#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assetmanager.h>

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
	gdl::Platform::GetPlatform().InitSystem(name, screenAspect, initCallback, updateCallback, drawCallback, initFlags);
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

gdl::Image* LoadImageFile(const char* filename, gdl::TextureFilterModes filterMode)
{
	gdl::Image* img = Image_LoadFile(filename, filterMode);
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

gdl::Image* LoadImagePNG(PNGFile* png, gdl::TextureFilterModes filterMode)
{
	gdl::Image* img = Image_LoadPNG(png, filterMode);
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

gdl::Sound* LoadSound(const char* filename)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::SoundWii();
#else
	snd = new gdl::SoundPC();
#endif
	if(snd->LoadFile(filename))
	{
		return snd;
	}
	else
	{
		delete snd;
		return nullptr;
	}
}

gdl::Sound* LoadSound(const u8* buffer, size_t size)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::SoundWii();
#else
	snd = new gdl::SoundPC();
#endif
	if(snd->LoadBuffer(buffer, size))
	{
		return snd;
	}
	else
	{
		delete snd;
		return nullptr;
	}
}

gdl::Sound* LoadOgg(const char* filename)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::MusicWii();
#else
	snd = new gdl::MusicPC();
#endif
	if(snd->LoadFile(filename))
	{
		return snd;
	}
	else
	{
		delete snd;
		return nullptr;
	}
}

Font* LoadFontFile(const char* filename, short characterWidth, short characterHeight, char firstCharacter)
{
	gdl::Image* fontImage = LoadImageFile(filename, gdl::TextureFilterModes::Linear);
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
	gdl::Image* fontImage = LoadImageFile(filename, gdl::TextureFilterModes::Linear);
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
	gdl::Image* fontImage = LoadImageFile(filename, gdl::TextureFilterModes::Linear);
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

WiiController* GetController( int controllerNumber)
{
	return gdl::Platform::GetPlatform().GetController(controllerNumber);
}

void DoProgramExit()
{
	gdl::Platform::GetPlatform().DoProgramExit();
}

u16 GetScreenWidth() { return gdl::Platform::GetPlatform().GetScreenWidth();}
u16 GetScreenHeight(){ return gdl::Platform::GetPlatform().GetScreenHeight();};
float GetAspectRatio(){ return gdl::Platform::GetPlatform().GetAspectRatio();};
float GetElapsedSeconds(){ return gdl::Platform::GetPlatform().GetElapsedSeconds();};
float GetDeltaTime(){ return gdl::Platform::GetPlatform().GetDeltaTime();};
