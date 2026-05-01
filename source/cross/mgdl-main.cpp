#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-fbx.h>

#include <mgdl/mgdl-font.h>
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

// HANDLE API
void mgdl_DrawTexture(TextureHandle handle, s16 x, s16 y)
{
	Texture* texture = AssetManager_GetTexture(handle);
	Texture_Draw2DAligned(texture, x, y, 1.0f, LJustify, LJustify);
}
void mgdl_DrawTextureV(TextureHandle handle, vec2 position)
{
	mgdl_DrawTexture(handle, position.x, position.y);
}

SoundHandle mgdl_LoadSound(const char* filename, SoundFileType soundType)
{
	return AssetManager_LoadSound(filename, soundType);
}

void mgdl_PlaySound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	Audio_PlaySound(snd);
}

ImageHandle mgdl_LoadPNG(const char* filename)
{
	return AssetManager_LoadPNG(filename);
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

Font* mgdl_LoadFont(const char* filename, short characterWidth, short characterHeight, char firstCharacter)
{
	Texture* fontTexture = Texture_LoadFile(filename, TextureFilterModes::Nearest);
	Font* font = Font_Load(fontTexture, characterWidth, characterHeight, firstCharacter);
	return font;
}

Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, char firstCharacter, short charactersPerRow)
{
	Texture* fontTexture = Texture_LoadFile(filename, TextureFilterModes::Linear);
	Font* font = Font_LoadPadded(fontTexture, characterWidth, characterHeight, firstCharacter, charactersPerRow);
	return font;
}

Font* mgdl_LoadFontCustom(const char* filename, short characterWidth, short characterHeight, short charactersPerRow, const char* characters)
{
	Texture* fontTexture = Texture_LoadFile(filename, TextureFilterModes::Linear);
	Font* font = Font_LoadSelective(fontTexture, characterWidth, characterHeight, charactersPerRow, characters);
	return font;
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

Viewport mgdl_GetViewport() { return Platform_GetViewport(); }
int mgdl_GetScreenWidth() { return Platform_GetViewport().width;}
int mgdl_GetScreenHeight() { return Platform_GetViewport().height;}
float mgdl_GetAspectRatio(){ return Platform_GetAspectRatio(); }
float mgdl_GetElapsedSeconds(){ return Platform_GetElapsedSeconds(); };
u32 mgdl_GetElapsedFrames(){ return Platform_GetElapsedFrames(); };
float mgdl_GetDeltaTime(){ return Platform_GetDeltaTime(); };
