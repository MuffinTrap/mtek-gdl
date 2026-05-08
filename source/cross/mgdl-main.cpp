#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-fbx.h>

#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-audio.h>

void mgdl_InitSystem(const char* windowName,
	ScreenAspect screenAspect,
	CallbackFunction initCallback,
	CallbackFunction frameCallback,
	CallbackFunction quitCallback,
	u32 initFlags)
{
	Platform_Init(windowName, screenAspect, initCallback, frameCallback, quitCallback, initFlags);

}


// DIRECT API

PNGFile* mgdl_LoadPNGFile(const char* filename)
{
	PNGFile* png = PNG_ReadFile(filename);
	return png;
}


Texture* mgdl_LoadTexturePNG(PNGFile* png, TextureFilterModes filterMode)
{
	Texture* img = Texture_LoadPNG(png, filterMode);
	return img;
}

Sound* mgdl_LoadSoundWav(const char* filename)
{
	Sound* snd = Audio_LoadSound(filename, SoundWav);
	return snd;
}


Sound* mgdl_LoadSoundOgg(const char* filename)
{
	Sound* snd = Audio_LoadSound(filename, SoundOgg);
	return snd;
}
Sound* mgdl_LoadSoundMp3(const char* filename)
{
	Sound* snd = Audio_LoadSound(filename, SoundMp3);
	return snd;
}

Texture* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter)
{
	Texture* fontTexture = Texture_LoadFile(filename, TextureFilterModes::Nearest);
	SpriteAtlas_MapSimple(fontTexture, characterWidth, characterHeight, firstCharacter);
	return fontTexture;
}


Texture* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters)
{
	Texture* fontTexture = Texture_LoadFile(filename, TextureFilterModes::Linear);
	SpriteAtlas_MapSelective(fontTexture, characterWidth, characterHeight, characters);
	return fontTexture;
}

Scene* mgdl_LoadFBX(const char* filename)
{
	Scene* s = FBX_Load(filename);

	// TODO calculate size
	return s;
}

WiiController* mgdl_GetController( int controllerNumber)
{
	return Platform_GetController(controllerNumber);
}

void mgdl_DoProgramExit()
{
	Platform_DoProgramExit();
}

Viewport mgdl_GetViewport() { return Platform_GetViewport(); }
int mgdl_GetScreenWidth() { return Platform_GetViewport().width;}
int mgdl_GetScreenHeight() { return Platform_GetViewport().height;}
float mgdl_GetAspectRatio(){ return Platform_GetAspectRatio(); }
float mgdl_GetElapsedSeconds(){ return Platform_GetElapsedSeconds(); };
u32 mgdl_GetElapsedFrames(){ return Platform_GetElapsedFrames(); };
float mgdl_GetDeltaTime(){ return Platform_GetDeltaTime(); };
