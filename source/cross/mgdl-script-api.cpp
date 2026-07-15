#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-scene.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-defaultfont.h>

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
void mgdl_DrawText(const char* text, float x, float y, float fontSize, color32 color)
{
	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawText(db, color, x, y, fontSize, text);
}

void mgdl_DrawTextV(const char* text, const Vector2& topleft, float fontSize, color32 color)
{
	mgdl_DrawText(text, topleft.x, topleft.y, fontSize, color);
}

void mgdl_DrawTextEx(TextureHandle font, const char* text, float x, float y,  float fontSize, color32 color)
{
	Texture* fontTexture = AssetManager_GetTexture(font);
	ASSERT_DEBUG(fontTexture->spriteAtlas != nullptr);
	Texture_DrawText(fontTexture, color, x, y, fontSize, text);
}

void mgdl_DrawTextVEx(TextureHandle font, const char* text, const Vector2 & topleft,  float fontSize, color32 color)
{
	mgdl_DrawTextEx(font, text, topleft.x, topleft.y, fontSize, color);
}

void mgdl_DrawText(const zstr& text, float x, float y,  float fontSize, color32 color)
{
	mgdl_DrawText(zstr_cstr(&text), x, y, fontSize, color);
}
void mgdl_DrawTextV(const zstr& text, const Vector2& topleft,  float fontSize, color32 color)
{
	mgdl_DrawText(zstr_cstr(&text), topleft.x, topleft.y,  fontSize, color);
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

void mgdl_DrawTextInt(const zstr& text, int number, float x, float y, float fontSize, color32 color)
{

	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawTextF(db, color, x, y, fontSize, "%s %d", zstr_cstr(&text), number);

}

void mgdl_DrawTextFloat(const zstr& text, float number, float x, float y, float fontSize, color32 color)
{
	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawTextF(db, color, x, y, fontSize, "%s %.4f", zstr_cstr(&text), number);

}
void mgdl_DrawInt( int number, float x, float y, float fontSize, color32 color)
{
	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawTextF(db, color, x, y, fontSize, "%d", number);

}
void mgdl_DrawFloat( float number, float x, float y, float fontSize, color32 color)
{
	Texture* db = DefaultFont_GetDefaultFont();
	Texture_DrawTextF(db, color, x, y, fontSize, "%.4f", number);

}

inline static void OpenGLRect(float x, float y, float x2, float y2, color32 color)
{
	mgdl_glColor32(color);
	// TOP LEFT
	glVertex2f(x, y);

	// LOW LEFT!
	glVertex2f(x, y2);

	// LOW RIGHT
	glVertex2f(x2, y2);

	// TOP RIGHT
	glVertex2f(x2, y);
}

void mgdl_DrawRectangle(float x, float y, float w, float h, color32 color)
{
	glBegin(GL_QUADS);
	OpenGLRect(x,y,x+w,y-h,color);
	glEnd();
}
void mgdl_DrawRectangleV(Vector2 topleft, Vector2 size, color32 color)
{
	glBegin(GL_QUADS);
	OpenGLRect(topleft.x, topleft.y, topleft.x+size.x, topleft.y-size.y, color);
	glEnd();
}

void mgdl_DrawRectangleLines(float x, float y, float w, float h, color32 color)
{
	glBegin(GL_LINE_LOOP);
	OpenGLRect(x,y,x+w,y-h,color);
	glEnd();
}

void mgdl_DrawRectangleLinesEx(RectF rect, float lineThickness, color32 color)
{
	glLineWidth(lineThickness);
	glBegin(GL_LINE_LOOP);
	OpenGLRect(rect.x, rect.y, rect.x+rect.w, rect.y-rect.h, color);
	glEnd();
	glLineWidth(1.0f);
}

void mgdl_DrawLine ( float x, float y, float x2, float y2, color32 color )
{
	glBegin(GL_LINES);
	mgdl_glColor32(color);
	glVertex2f(x, y);
	glVertex2f(x2, y2);
	glEnd();
}

void mgdl_DrawLineGradient( float x1, float y1, float x2, float y2, color32 color1 , color32 color2)
{
	glBegin(GL_LINES);
	mgdl_glColor32(color1);
	glVertex2f(x1, y1);
	mgdl_glColor32(color2);
	glVertex2f(x2, y2);
	glEnd();

}
void mgdl_DrawLineV(Vector2 start, Vector2 end, color32 color )
{
	glBegin(GL_LINES);
	mgdl_glColor32(color);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();
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

TextureHandle mgdl_LoadTexture(const zstr& filename, bool generateMipMaps)
{
	return mgdl_LoadTexture(zstr_cstr(&filename), generateMipMaps);
}
TextureHandle mgdl_LoadTexture(const char* filename, bool generateMipMaps)
{
	TextureHandle handle = AssetManager_LoadTexture(filename, generateMipMaps);
	return handle;
}

void mgdl_SetTextureFilterMag(TextureHandle texture, TextureFilterModes mode)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	Texture_SetFilterModeMag(tex, mode);
}
void mgdl_SetTextureFilterMin(TextureHandle texture, TextureFilterModes mode)
{
	Texture* tex = AssetManager_GetTexture(texture);
	ASSERT_DEBUG(tex != nullptr);
	Texture_SetFilterModeMin(tex, mode);
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

void mgdl_DrawTextureEx(TextureHandle handle, float x, float y, float rotationDeg, float scale)
{
	Texture* tex = AssetManager_GetTexture(handle);
	const float wh = tex->width/2;
	const float hh = tex->height/2;
	ASSERT_DEBUG(tex != nullptr);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(rotationDeg, 0.0f, 0.0f, 1.0f);
	Texture_Draw(tex, -wh, hh, scale);
	glPopMatrix();
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
		glPushMatrix();
		glTranslatef(x, y, z);
		glScalef(scale, scale, scale);
		Scene_Draw(scene);
		glPopMatrix();
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
	if (Sound_GetStatus(snd) == Audio_StatePaused)
	{
		Sound_Resume(snd);
	}
	else
	{
		Sound_Play(snd);
	}
}
void mgdl_PauseSound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	Sound_Pause(snd);
}
void mgdl_StopSound(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	Sound_Stop(snd);
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

Vector2 mgdl_GetJoystick(int controller, WiiJoystick joystick)
{

	if (controller >=0 && controller < MGDL_MAX_CONTROLLERS)
	{
		WiiController* c = mgdl_GetController(controller);
		ASSERT_DEBUG(c != nullptr);
		if (c != nullptr)
		{
			if (WiiController_HasNunchuk(c))
			{
				return WiiController_GetNunchukJoystickDirection(c);
			}
			else if (WiiController_HasClassicController(c))
			{
				if(joystick == Joystick_LeftStick)
				{
					return WiiController_GetLeftStickDirection(c);
				}
				else if(joystick == Joystick_RightStick)
				{
					return WiiController_GetRightStickDirection(c);
				}
			}
		}
	}
	return Vector2Zero();
}

float mgdl_GetJoystickX(int controller, WiiJoystick joystick)
{
	if (controller >=0 && controller < MGDL_MAX_CONTROLLERS)
	{
		WiiController* c = mgdl_GetController(controller);
		ASSERT_DEBUG(c != nullptr);
		if (c != nullptr)
		{
			if (WiiController_HasNunchuk(c))
			{
				return WiiController_GetNunchukJoystickDirectionX(c);
			}
			else if (WiiController_HasClassicController(c))
			{
				if(joystick == Joystick_LeftStick)
				{
					return WiiController_GetLeftStickDirectionX(c);
				}
				else if(joystick == Joystick_RightStick)
				{
					return WiiController_GetRightStickDirectionX(c);
				}
			}
		}
	}
	return 0.0f;

}
float mgdl_GetJoystickY(int controller, WiiJoystick joystick)
{
	if (controller >=0 && controller < MGDL_MAX_CONTROLLERS)
	{
		WiiController* c = mgdl_GetController(controller);
		ASSERT_DEBUG(c != nullptr);
		if (c != nullptr)
		{
			if (WiiController_HasNunchuk(c))
			{
				return WiiController_GetNunchukJoystickDirectionY(c);
			}
			else if (WiiController_HasClassicController(c))
			{
				if(joystick == Joystick_LeftStick)
				{
					return WiiController_GetLeftStickDirectionY(c);
				}
				else if(joystick == Joystick_RightStick)
				{
					return WiiController_GetRightStickDirectionY(c);
				}
			}
		}
	}
	return 0.0f;

}

float mgdl_GetSpriteAspect(TextureHandle handle)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr && tex->spriteAtlas != nullptr);
	return tex->spriteAtlas ->aspectRatio;
}

int mgdl_GetTextureWidth(TextureHandle handle)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	return tex->width;
}
int mgdl_GetTextureHeight(TextureHandle handle)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	return tex->height;
}

int mgdl_GetSpriteWidth(TextureHandle handle)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	if (tex->spriteAtlas != nullptr)
	{
		return tex->spriteAtlas->spriteWidth;
	}
	return 0;
}
int mgdl_GetSpriteHeight(TextureHandle handle)
{
	Texture* tex = AssetManager_GetTexture(handle);
	ASSERT_DEBUG(tex != nullptr);
	if (tex->spriteAtlas != nullptr)
	{
		return tex->spriteAtlas->spriteHeight;
	}
	return 0;
}

int mgdl_GetSoundElapsedMs(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	return Sound_GetElapsedMs(snd);
}

mgdlAudioStateEnum mgdl_GetSoundStatus(SoundHandle handle)
{
	Sound* snd = AssetManager_GetSound(handle);
	ASSERT_DEBUG(snd != nullptr);
	return Sound_GetStatus(snd);
}
