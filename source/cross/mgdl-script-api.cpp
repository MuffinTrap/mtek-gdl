#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-draw2d.h>

static Palette* s_activePalette = nullptr;
static PaletteHandle s_defaultPaletteHandle = 0;
static PaletteHandle s_debugPaletteHandle = 0;

void mgdl_InitScriptApi()
{
	// TODO Connect to AngelScript
	s_defaultPaletteHandle = AssetManager_LoadDefaultPalette();
	s_debugPaletteHandle = AssetManager_LoadDebugPalette();

	s_activePalette = AssetManager_GetPalette(s_defaultPaletteHandle);
}

Handle mgdl_LoadTexture(const zstr& filename)
{
	Log_InfoF("Angel Load texture from file %s\n", zstr_cstr(&filename));
	TextureHandle handle = AssetManager_LoadTexture(zstr_cstr(&filename));
	Log_InfoF("Angel got texture handle %u\n", handle);
	return handle;
}

void mgdl_SetTextureFilter(Handle texture, TextureFilterModes mode)
{
	Texture* tex = AssetManager_GetTexture(texture);
	Texture_SetFilterMode(tex, mode);
}
void mgdl_SetTextureWrap(Handle texture, TextureWrapModes mode)
{
	Texture* tex = AssetManager_GetTexture(texture);
	Texture_SetWrapMode(tex, mode);
}

void mgdl_DrawTexture(TextureHandle handle, s16 x, s16 y)
{
	Texture* tex = AssetManager_GetTexture(handle);
	Texture_Draw2DAligned(tex, x, y, 1.0f, LJustify, LJustify);
}

SoundHandle mgdl_LoadSound(const zstr& filename)
{
	const char* file = zstr_cstr(&filename);
	if (zstr_ends_with(&filename, "wav"))
	{
		return AssetManager_LoadSound(file, SoundWav);
	}
	else if (zstr_ends_with(&filename, "ogg"))
	{
		return AssetManager_LoadSound(file, SoundOgg);
	}
	else if (zstr_ends_with(&filename, "mp3"))
	{
		return AssetManager_LoadSound(file, SoundMp3);
	}
	return 0;
}

void mgdl_PlaySound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	Audio_PlaySound(snd);
}

Handle mgdl_LoadPalette(const zstr& image)
{
	return AssetManager_LoadPalette(zstr_cstr(&image));
}

Handle mgdl_GetDefaultPalette()
{
	return s_defaultPaletteHandle;
}

Handle mgdl_GetDebugPalette()
{
	return s_debugPaletteHandle;
}

void mgdl_SetPalette(Handle palette)
{
	s_activePalette = AssetManager_GetPalette(palette);
}

bool mgdl_IsButtonDown(int controller, WiiButtons button)
{
	WiiController* c = mgdl_GetController(controller);
	if (c != nullptr)
	{
		return WiiController_ButtonHeld(c, button);
	}
	return false;

}
bool mgdl_IsButtonPressed(int controller, WiiButtons button)
{
	WiiController* c = mgdl_GetController(controller);
	if (c != nullptr)
	{
		return WiiController_ButtonPress(c, button);
	}
	return false;
}


void mgdl_DrawRectangle(s16 x, s16 y, s16 w, s16 h, u8 paletteIndex)
{
	Draw2D_RectWH(x, y, w, h, Palette_GetColor4fPtr(s_activePalette, paletteIndex));
}
