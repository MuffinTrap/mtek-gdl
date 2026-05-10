#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-assert.h>

static Palette* s_activePalette = nullptr;
static PaletteHandle s_defaultPaletteHandle = Handle_CreatePalette(0);
static PaletteHandle s_debugPaletteHandle = Handle_CreatePalette(0);

void mgdl_InitScriptApi()
{
	// TODO Connect to AngelScript
	s_defaultPaletteHandle = AssetManager_LoadDefaultPalette();
	s_debugPaletteHandle = AssetManager_LoadDebugPalette();

	s_activePalette = AssetManager_GetPalette(s_defaultPaletteHandle);
}

// DRAWING

void mgdl_DrawText(const zstr& text, float x, float y,  float fontSize, color32 color)
{
	mgdl_DrawText(x, y, zstr_cstr(&text), fontSize, color);
}
void mgdl_DrawTextV(const zstr& text, Vector2 topleft,  float fontSize, color32 color)
{
	mgdl_DrawText(topleft.x, topleft.y, zstr_cstr(&text), fontSize, color);
}

void mgdl_DrawTextEx(TextureHandle font, const zstr& text, float x, float y,  float fontSize, color32 color)
{
	Texture* fontTexture = AssetManager_GetTexture(font);
	ASSERT_DEBUG(fontTexture->spriteAtlas != nullptr);
	Texture_DrawText(fontTexture, color, x, y, fontSize, zstr_cstr(&text));
}
void mgdl_DrawTextVEx(TextureHandle font, const zstr& text, Vector2 topleft,  float fontSize, color32 color)
{
	mgdl_DrawTextEx(font, text, topleft.x, topleft.y, fontSize, color);
}

// TEXTURES

TextureHandle mgdl_LoadTexture(const zstr& filename)
{
	return mgdl_LoadTexture(zstr_cstr(&filename));
}
TextureHandle mgdl_LoadTexture(const char* filename)
{
	Log_InfoF("Angel Load texture from file %s\n", filename);
	TextureHandle handle = AssetManager_LoadTexture(filename);
	Log_InfoF("Angel got texture handle %u\n", handle);
	return handle;
}

void mgdl_SetTextureFilter(TextureHandle texture, TextureFilterModes mode)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	Texture_SetFilterMode(tex, mode);
}
void mgdl_SetTextureWrap(TextureHandle texture, TextureWrapModes mode)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	Texture_SetWrapMode(tex, mode);
}

void mgdl_CreateFontUVs(TextureHandle texture, s16 characterWidth, s16 characterHeight, char firstCharacter)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	SpriteAtlas_MapSimple(tex, characterWidth, characterHeight, firstCharacter);
}

void mgdl_DrawTexture(TextureHandle handle, float x, float y)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	Texture_Draw(tex, x, y, 1.0f);
}

void mgdl_DrawTextureV(TextureHandle handle, vec2 position)
{
	mgdl_DrawTexture(handle, position.x, position.y);
}

// IMAGES

ImageHandle mgdl_LoadPNG(const char* filename)
{
	return AssetManager_LoadPNG(filename);
}

// SOUNDS

void mgdl_PlaySound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	Audio_PlaySound(snd);
}


SoundHandle mgdl_LoadSound(const zstr& filename)
{
	return mgdl_LoadSound(zstr_cstr(&filename));

}
SoundHandle mgdl_LoadSound(const char* filename)
{
	z_str::string file = z_str::string(filename);
	z_str::view fileview = file;

	if (fileview.ends_with("wav"))
	{
		return AssetManager_LoadSound(filename, SoundWav);
	}
	else if (fileview.ends_with("ogg"))
	{
		return AssetManager_LoadSound(filename, SoundOgg);
	}
	else if (fileview.ends_with("mp3"))
	{
		return AssetManager_LoadSound(filename, SoundMp3);
	}
	Log_ErrorF("LoadSound unsupported file type on %s\n", filename);
	return SoundHandle{0};
}

// PALETTES

PaletteHandle mgdl_LoadPalette(const zstr& image)
{
	return mgdl_LoadPalette(zstr_cstr(&image));
}

PaletteHandle mgdl_LoadPalette(const char* image)
{
	return AssetManager_LoadPalette(image);
}

PaletteHandle mgdl_GetDefaultPalette()
{
	return s_defaultPaletteHandle;
}

PaletteHandle mgdl_GetDebugPalette()
{
	return s_debugPaletteHandle;
}

void mgdl_SetPalette(PaletteHandle palette)
{
	s_activePalette = AssetManager_GetPalette(palette);
}

color32 mgdl_GetPaletteColor(PaletteHandle palette, u8 colorIndex)
{
	return Palette_GetColor(AssetManager_GetPalette(palette), colorIndex);
}

// INPUT

bool mgdl_IsButtonDown(int controller, WiiButtons button)
{
	if (controller >=0 && controller < MGDL_MAX_CONTROLLERS)
	{
		WiiController* c = mgdl_GetController(controller);
		ASSERT_DEBUG(c != nullptr);
		if (c != nullptr)
		{
			return WiiController_ButtonHeld(c, button);
		}
	}
	return false;

}
bool mgdl_IsButtonPressed(int controller, WiiButtons button)
{
	if (controller >=0 && controller < MGDL_MAX_CONTROLLERS)
	{
		WiiController* c = mgdl_GetController(controller);
		ASSERT_DEBUG(c != nullptr);
		if (c != nullptr)
		{
			return WiiController_ButtonPress(c, button);
		}
	}
	return false;
}
