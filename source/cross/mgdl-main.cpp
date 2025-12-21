#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-fbx.h>

#include <mgdl/mgdl-font.h>
#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-audio.h>

static AssetManager assetManager;

void mgdl_InitSystem(const char* windowName,
	ScreenAspect screenAspect,
	CallbackFunction initCallback,
	CallbackFunction frameCallback,
	CallbackFunction quitCallback,
	u32 initFlags)
{
	Platform_Init(windowName, screenAspect, initCallback, frameCallback, quitCallback, initFlags);
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

Texture* mgdl_LoadTexture(const char* filename, TextureFilterModes filterMode)
{
	Texture* img = Texture_LoadFile(filename, filterMode);
	if (img != nullptr)
	{
		AssetManager_LoadTexture(&assetManager, img);
		return img;
	}
	else
	{
		return nullptr;
	}
}

Texture* mgdl_LoadTexturePNG(PNGFile* png, TextureFilterModes filterMode)
{
	Texture* img = Texture_LoadPNG(png, filterMode);
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

Sound* mgdl_LoadSoundWav(const char* filename)
{
	Sound* snd = Audio_LoadSound(filename, SoundWav);
	if(snd != nullptr)
	{
		AssetManager_LoadSound(&assetManager, snd);
	}
	return snd;
}


Sound* mgdl_LoadSoundOgg(const char* filename)
{
	Sound* snd = Audio_LoadSound(filename, SoundOgg);
	if(snd != nullptr)
	{
		AssetManager_LoadSound(&assetManager, snd);
	}
	return snd;
}

Font* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter)
{
	Texture* fontTexture = mgdl_LoadTexture(filename, TextureFilterModes::Nearest);
	Font* font = Font_Load(fontTexture, characterWidth, characterHeight, firstCharacter);
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
	Texture* fontTexture = mgdl_LoadTexture(filename, TextureFilterModes::Linear);
	Font* font = Font_LoadPadded(fontTexture, characterWidth, characterHeight, firstCharacter, charactersPerRow);

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
	Texture* fontTexture = mgdl_LoadTexture(filename, TextureFilterModes::Linear);
	Font* font = Font_LoadSelective(fontTexture, characterWidth, characterHeight, charactersPerRow, characters);
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
u32 mgdl_GetElapsedFrames(){ return Platform_GetElapsedUpdates(); };
float mgdl_GetDeltaTime(){ return Platform_GetDeltaTime(); };
