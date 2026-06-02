#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-opengl_util.h>

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
void mgdl_DrawTextV(const zstr& text, const Vector2& topleft,  float fontSize, color32 color)
{
	mgdl_DrawText(topleft.x, topleft.y, zstr_cstr(&text), fontSize, color);
}

void mgdl_DrawTextEx(TextureHandle font, const zstr& text, float x, float y,  float fontSize, color32 color)
{
	Texture* fontTexture = AssetManager_GetTexture(font);
	ASSERT_DEBUG(fontTexture->spriteAtlas != nullptr);
	Texture_DrawText(fontTexture, color, x, y, fontSize, zstr_cstr(&text));
}
void mgdl_DrawTextVEx(TextureHandle font, const zstr& text, const Vector2& topleft,  float fontSize, color32 color)
{
	mgdl_DrawTextEx(font, text, topleft.x, topleft.y, fontSize, color);
}

void mgdl_DrawModel(ModelHandle handle, float x, float y, float z, float scale, color32 color)
{
	Model* model = AssetManager_GetModel(handle);
	if (model)
	{
		glTranslatef(x, y, z);
		glScalef(scale, scale, scale);
		if (model->m_material == nullptr)
		{
			mgdl_glColor32(color);
		}
		Model_Draw(model);
	}
}

void mgdl_DrawModelRotated(ModelHandle handle, const float x, float y, float z, float xdeg, float ydeg, float zdeg, float scale, color32 color)
{
	Model* model = AssetManager_GetModel(handle);
	if (model)
	{
		glTranslatef(x, y, z);
		if (xdeg != 0.0f)
		{
			glRotatef(xdeg, 1.0f, 0.0f, 0.0f);
		}
		if (ydeg != 0.0f)
		{
			glRotatef(ydeg, 0.0f, 1.0f, 0.0f);
		}
		if (zdeg != 0.0f)
		{
			glRotatef(zdeg, 0.0f, 0.0f, 1.0f);
		}
		glScalef(scale, scale, scale);
		if (model->m_material == nullptr)
		{
			mgdl_glColor32(color);
		}
		Model_Draw(model);
	}
}

// TEXTURES

TextureHandle mgdl_LoadTexture(const zstr& filename)
{
	return mgdl_LoadTexture(zstr_cstr(&filename));
}
TextureHandle mgdl_LoadTexture(const char* filename)
{
	TextureHandle handle = AssetManager_LoadTexture(filename);
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

void mgdl_CreateFont(TextureHandle texture, s16 characterWidth, s16 characterHeight, char firstCharacter)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	SpriteAtlas_MapSimple(tex, characterWidth, characterHeight, firstCharacter);
}

void mgdl_CreateSpriteAtlas(TextureHandle texture, s16 spriteWidth, s16 spriteHeight)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	SpriteAtlas_MapSimple(tex, spriteWidth, spriteHeight, 0);
}

void mgdl_DrawTexture(TextureHandle handle, float x, float y)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	Texture_Draw(tex, x, y, 1.0f);
}

void mgdl_DrawTextureV(TextureHandle handle, const Vector2& topLeft)
{
	mgdl_DrawTexture(handle, topLeft.x, topLeft.y);
}


void mgdl_DrawSprite(TextureHandle handle, u16 spriteIndex,float x, float y, color32 color)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	Texture_DrawSprite(tex, spriteIndex, x, y, 1.0f, color);
}
void mgdl_DrawSpriteEx(TextureHandle handle, u16 spriteIndex,float x, float y, float scale, color32 color)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	Texture_DrawSprite(tex, spriteIndex, x, y, scale, color);
}

void mgdl_DrawSceneV(SceneHandle handle, const Vector3& position, float scale, color32 color)
{
	mgdl_DrawScene(handle, position.x, position.y, position.z, scale, color);
}

void mgdl_DrawScene(SceneHandle handle, float x, float y, float z, float scale, color32 color)
{
	Scene* scene = AssetManager_GetScene(handle);
	if (scene != nullptr)
	{
		glTranslatef(x, y, z);
		glScalef(scale, scale, scale);
		Scene_Draw(scene);
	}
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
	if (Audio_GetSoundStatus(snd) == Audio_StatePaused)
	{
		Audio_ResumeSound(snd);
	}
	else
	{
		Audio_PlaySound(snd);
	}
}
void mgdl_PauseSound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	Audio_PauseSound(snd);
}
void mgdl_StopSound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	Audio_StopSound(snd);
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

// SCENES
SceneHandle mgdl_LoadScene(const char* filename)
{
	return AssetManager_LoadScene(filename);
}

SceneHandle mgdl_LoadScene(const zstr& filename)
{
	return mgdl_LoadScene(zstr_cstr(&filename));
}

// MODELS
ModelHandle mgdl_LoadModel(const char* filename)
{
	return AssetManager_LoadModel(filename);
}

ModelHandle mgdl_LoadModel(const zstr& filename)
{
	return mgdl_LoadModel(zstr_cstr(&filename));
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

bool mgdl_GetBool(Handle handle, mgdlParameter parameter)
{
	switch(parameter)
	{
		case MGDL_SOUND_LOOPING:
		{
			Sound* snd = AssetManager_GetSound(handle);
			if (snd != nullptr)
			{
				return Sound_GetLooping(snd);
			}
		}
			break;
		case MGDL_SOUND_PAUSED:
		{
			Sound* snd = AssetManager_GetSound(handle);
			if (snd != nullptr)
			{
				return Audio_GetSoundStatus(snd) == Audio_StatePaused;
			}
		}
			break;
		default:
			Log_ErrorF("mgdl_GetBool no result for parameter %d\n", parameter);
			return false;
	}
	return false;
}
void mgdl_SetBool(Handle handle, mgdlParameter parameter, bool value)
{
	switch(parameter)
	{
		case MGDL_SOUND_LOOPING:
		{
			Sound* snd = AssetManager_GetSound(handle);
			ASSERT_DEBUG(snd != nullptr);
			Sound_SetLooping(snd, value);
		}
		break;
		default:
			Log_ErrorF("mgdl_SetBool no action for parameter %d\n", parameter);
			break;
	}
}

float mgdl_GetFloat(Handle handle, mgdlParameter parameter)
{
	switch(parameter)
	{
		case MGDL_SPRITE_ASPECT:
		{
			Texture* tex = AssetManager_GetTexture(handle);
			ASSERT_DEBUG(tex != nullptr && tex->spriteAtlas != nullptr);
			return tex->spriteAtlas ->aspectRatio;
		}
		break;
		default:
			Log_ErrorF("mgdl_GetFloat no result for parameter %d\n", parameter);
			break;
	}
	return 0.0f;
}

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-parameter"

void mgdl_SetFloat(Handle handle, mgdlParameter parameter, float value)
{

}

int mgdl_GetInt(Handle handle, mgdlParameter parameter)
{
	switch(parameter)
	{
		case MGDL_TEXTURE_WIDTH:
		{
			Texture* tex = AssetManager_GetTexture(handle);
			ASSERT_DEBUG(tex != nullptr);
			return tex->width;
		}
		break;
		case MGDL_TEXTURE_HEIGHT:
		{
			Texture* tex = AssetManager_GetTexture(handle);
			ASSERT_DEBUG(tex != nullptr);
			return tex->height;
		}
		break;
		case MGDL_SPRITE_WIDTH:
		{
			Texture* tex = AssetManager_GetTexture(handle);
			ASSERT_DEBUG(tex != nullptr);
			if (tex->spriteAtlas != nullptr)
			{
				return tex->spriteAtlas->spriteWidth;
			}
		}
		break;
		case MGDL_SPRITE_HEIGHT:
		{
			Texture* tex = AssetManager_GetTexture(handle);
			ASSERT_DEBUG(tex != nullptr);
			if (tex->spriteAtlas != nullptr)
			{
				return tex->spriteAtlas->spriteHeight;
			}
		}
		break;
		case MGDL_SOUND_ELAPSED_MS:
		{
			Sound* snd = AssetManager_GetSound(handle);
			ASSERT_DEBUG(snd != nullptr);
			return Audio_GetSoundElapsedMs(snd);
		}
		case MGDL_SOUND_STATUS_ENUM:
		{
			Sound* snd = AssetManager_GetSound(handle);
			ASSERT_DEBUG(snd != nullptr);
			return (int)Audio_GetSoundStatus(snd);
		}
		default:
			Log_ErrorF("mgdl_GetInt no result for parameter %d\n", parameter);
			break;

	}
	return 0;
}
void mgdl_SetInt(Handle handle, mgdlParameter parameter, int value)
{

	switch(parameter)
	{
		case MGDL_SOUND_ELAPSED_MS:
		{
			Sound* snd = AssetManager_GetSound(handle);
			ASSERT_DEBUG(snd != nullptr);
			return Audio_SetSoundElapsedMs(snd, value);
		}
		break;
		default:
			Log_ErrorF("mgdl_SetInt no action for parameter %d\n", parameter);
			break;

	}
}

bool mgdl_GetVector2(Handle handle, mgdlParameter parameter, Vector2& vOut){
	return false;
}
void mgdl_SetVector2(Handle handle, mgdlParameter parameter, const Vector2& value)
{

}

	#pragma GCC diagnostic pop
