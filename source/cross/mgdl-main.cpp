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

void mgdl_InitSystem(const char* name,
	gdl::ScreenAspect screenAspect,
								std::function<void()> initCallback,
								std::function<void()> updateCallback,
								std::function<void()> drawCallback,
								u32 initFlags)
{
	Platform_Init(name, screenAspect, initCallback, updateCallback, drawCallback, initFlags);
	AssetManager_Init(&assetManager);
}

PNGFile* mgdl_LoadPNG(const char* filename)
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

Image* mgdl_LoadImage(const char* filename, gdl::TextureFilterModes filterMode)
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

Image* mgdl_LoadImagePNG(PNGFile* png, gdl::TextureFilterModes filterMode)
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

Sound* mgdl_LoadSound(const char* filename)
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


Music* mgdl_LoadOgg(const char* filename)
{
	Music* music = Music_LoadOgg(filename);
	if(music != nullptr)
	{
		AssetManager_LoadMusic(&assetManager, music);
		return music;
	}
	else
	{
		return nullptr;
	}
}

Music* mgdl_LoadWav(const char* filename)
{
	Music* music = Music_LoadWav(filename);
	if(music != nullptr)
	{
		AssetManager_LoadMusic(&assetManager, music);
		return music;
	}
	else
	{
		return nullptr;
	}

}

Font* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter)
{
	Image* fontImage = mgdl_LoadImage(filename, gdl::TextureFilterModes::Nearest);
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

Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow)
{
	Image* fontImage = mgdl_LoadImage(filename, gdl::TextureFilterModes::Linear);
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

Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters)
{
	Image* fontImage = mgdl_LoadImage(filename, gdl::TextureFilterModes::Linear);
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

Scene* mgdl_LoadFBX(const char* filename)
{
	Scene* s = FBX_Load(filename);

	// TODO calculate size
	return s;
}

Sprite* mgdl_LoadSprite(const char* filename, short spriteWidth, short spriteHeight)
{
	Font* spriteSheet = mgdl_LoadFont(filename, spriteWidth, spriteHeight, 0);
	return Sprite_Load(spriteSheet);
}

WiiController* mgdl_GetController( int controllerNumber)
{
	return Platform_GetController(controllerNumber);
}

void mgdl_DoProgramExit()
{
	Platform_DoProgramExit();
}

u16 mgdl_GetScreenWidth() { return Platform_GetSingleton()->screenWidth; }
u16 mgdl_GetScreenHeight(){ return Platform_GetSingleton()->screenHeight; }
float mgdl_GetAspectRatio(){ return Platform_GetSingleton()->aspectRatio; }
float mgdl_GetElapsedSeconds(){ return Platform_GetElapsedSeconds(); };
float mgdl_GetDeltaTime(){ return Platform_GetDeltaTime(); };
