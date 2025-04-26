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

gdl::PNGFile* LoadPNG(std::string filename)
{
	gdl::PNGFile* png = new gdl::PNGFile();
	if (png->ReadFile(filename.c_str()))
	{
		return png;
	}
	else
	{
		delete png;
		return nullptr;
	}
}

gdl::Image* LoadImageFile(std::string filename, gdl::TextureFilterModes filterMode)
{
	gdl::Image* img = Image_LoadFile(filename.c_str(), filterMode);
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

gdl::Image* LoadImagePNG(gdl::PNGFile* png, gdl::TextureFilterModes filterMode)
{
	gdl::Image* img = new gdl::Image();
	if (img->LoadPNG(png, filterMode))
	{
		return img;
	}
	else
	{
		delete img;
		return nullptr;
	}
}

gdl::Sound* LoadSound(std::string filename)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::SoundWii();
#else
	snd = new gdl::SoundPC();
#endif
	if(snd->LoadFile(filename.c_str()))
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

gdl::Sound* LoadOgg(std::string filename)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::MusicWii();
#else
	snd = new gdl::MusicPC();
#endif
	if(snd->LoadFile(filename.c_str()))
	{
		return snd;
	}
	else
	{
		delete snd;
		return nullptr;
	}
}


Font* LoadFontFile(std::string filename, short characterWidth, short characterHeight, char firstCharacter)
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

Font* LoadFontCustom(std::string filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow)
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

Font* LoadFontCustom(std::string filename, short characterWidth, short characterHeight, short charactersPerRow, std::string characters)
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
