
#include <mgdl.h>
#include <angelscript.h>
#include "scriptmgdl.h"
#include "ZStrFactory.h"
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-script-api.h>
#include <mgdl/mgdl-vector.h>

static void RegisterOpenGLUtilsAndGlut(asIScriptEngine* as_engine)
{
	// GLU functions
	as_engine->RegisterGlobalFunction("void gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upz, GLdouble upz)", asGLFunctionLink(gluLookAt), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("void gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFarz)", asGLFunctionLink(gluPerspective), asGLFunctionCall);


	// mgdl utils
	as_engine->RegisterGlobalFunction("void mgdl_InitOrthoProjection(float yDirection)", asGLFunctionLink(mgdl_InitOrthoProjection), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_glSetAlphaTest(bool enabled)", asGLFunctionLink(mgdl_glSetAlphaTest), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_glColor32(color32 color)", asGLFunctionLink(mgdl_glColor32), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("void mgdl_SetGlobalAmbientColor32(color32 color, float strength)", asGLFunctionLink(mgdl_SetGlobalAmbientColor32), asGLFunctionCall);



}


static ZStrFactory stringFactory;

static void RegisterTypes(asIScriptEngine* as_engine)
{

	// Register typedefs
	as_engine->RegisterTypedef("u8", "uint8");
	as_engine->RegisterTypedef("s8", "int8");

	// AngelScript does not have char
	as_engine->RegisterTypedef("char", "uint8");

	as_engine->RegisterTypedef("u16", "uint16");
	as_engine->RegisterTypedef("s16", "int16");

	as_engine->RegisterTypedef("u32", "uint32");
	as_engine->RegisterTypedef("s32", "int32");

	as_engine->RegisterTypedef("u64", "uint64");
	as_engine->RegisterTypedef("s64", "int64");

#ifdef GEKKO
	as_engine->RegisterTypedef("sizetype", "uint32");
#else
	as_engine->RegisterTypedef("sizetype", "uint64");
#endif

	// Register zstr as the string type
	as_engine->RegisterObjectType("zstr", sizeof(zstr), asOBJ_VALUE|asOBJ_POD| asOBJ_APP_CLASS);
	as_engine->RegisterStringFactory("zstr", &stringFactory);

	as_engine->RegisterTypedef("PaletteHandle", "uint32");
	as_engine->RegisterTypedef("TextureHandle", "uint32");
	as_engine->RegisterTypedef("SoundHandle", "uint32");
	as_engine->RegisterTypedef("ImageHandle", "uint32");
	as_engine->RegisterTypedef("SceneHandle", "uint32");
	as_engine->RegisterTypedef("ModelHandle", "uint32");

	as_engine->RegisterTypedef("color32", "uint32");

	as_engine->RegisterEnum("TextureFilterModes");
	as_engine->RegisterEnumValue("TextureFilterModes", "Linear", (int)Linear);
	as_engine->RegisterEnumValue("TextureFilterModes", "Nearest", (int)Nearest);
	as_engine->RegisterEnumValue("TextureFilterModes", "MipmapNearest", (int)MipmapNearest);
	as_engine->RegisterEnumValue("TextureFilterModes", "MipmapLinear", (int)MipmapLinear);
	as_engine->RegisterEnumValue("TextureFilterModes", "MipmapAvgNearest", (int)MipmapAvgNearest);
	as_engine->RegisterEnumValue("TextureFilterModes", "MipmapTrilinear", (int)MipmapTrilinear);

	as_engine->RegisterEnum("TextureWrapModes");
	as_engine->RegisterEnumValue("TextureWrapModes", "Wrap_Clamp", (int)Wrap_Clamp);
	as_engine->RegisterEnumValue("TextureWrapModes", "Wrap_Repeat", (int)Wrap_Repeat);
	as_engine->RegisterEnumValue("TextureWrapModes", "Wrap_Mirror", (int)Wrap_Mirror);

	// Register constants
	    // Register a boolean flag that can be read, but not modified by the script.
}

static void RegisterMain(asIScriptEngine* as_engine)
{
	// Get status functions
	as_engine->RegisterGlobalFunction("int mgdl_GetScreenHeight()", asGLFunctionLink(mgdl_GetScreenHeight), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("int mgdl_GetScreenWidth()", asGLFunctionLink(mgdl_GetScreenWidth), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("float mgdl_GetElapsedSeconds()", asGLFunctionLink(mgdl_GetElapsedSeconds), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("float mgdl_GetDeltaTime()", asGLFunctionLink(mgdl_GetDeltaTime), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("float mgdl_DoProgramExit()", asGLFunctionLink(mgdl_DoProgramExit), asGLFunctionCall);


	// Logging
	as_engine->RegisterGlobalFunction("void mgdl_LogText(const zstr &in text)", asGLFunctionLink(mgdl_LogText), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_LogTextInt(const zstr &in text, int number)", asGLFunctionLink(mgdl_LogTextInt), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_LogTextFloat(const zstr &in text, float number)", asGLFunctionLink(mgdl_LogTextFloat), asGLFunctionCall);


	// Asset handling
	as_engine->RegisterGlobalFunction("TextureHandle mgdl_LoadTexture(const zstr &in filename, bool generateMipMaps)", asGLFunctionLinkPR(mgdl_LoadTexture, (const zstr&, bool), TextureHandle), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("PaletteHandle mgdl_LoadPalette(const zstr &in filename)", asGLFunctionLinkPR(mgdl_LoadPalette, (const zstr&), PaletteHandle), asGLFunctionCall);;
	as_engine->RegisterGlobalFunction("SoundHandle mgdl_LoadSound(const zstr &in filename)", asGLFunctionLinkPR(mgdl_LoadSound, (const zstr&), SoundHandle), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("SceneHandle mgdl_LoadScene(const zstr &in filename)", asGLFunctionLinkPR(mgdl_LoadScene, (const zstr&), SceneHandle), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("ModelHandle mgdl_LoadModel(const zstr &in filename)", asGLFunctionLinkPR(mgdl_LoadModel, (const zstr&), ModelHandle), asGLFunctionCall);
}

static void RegisterDrawing(asIScriptEngine* as_engine)
{
	// Default debug colors
	as_engine->RegisterGlobalProperty("const color32 Debug_Black", (void*)&Debug_Black);
	as_engine->RegisterGlobalProperty("const color32 Debug_Red", (void*)&Debug_Red);
	as_engine->RegisterGlobalProperty("const color32 Debug_Green", (void*)&Debug_Green);
	as_engine->RegisterGlobalProperty("const color32 Debug_Blue", (void*)&Debug_Blue);
	as_engine->RegisterGlobalProperty("const color32 Debug_Cyan", (void*)&Debug_Cyan);
	as_engine->RegisterGlobalProperty("const color32 Debug_Magenta", (void*)&Debug_Magenta);
	as_engine->RegisterGlobalProperty("const color32 Debug_Yellow", (void*)&Debug_Yellow);
	as_engine->RegisterGlobalProperty("const color32 Debug_White", (void*)&Debug_White);

	as_engine->RegisterGlobalProperty("const color32 Debug_Gray", (void*)&Debug_Gray);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightRed", (void*)&Debug_LightRed);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightGreen", (void*)&Debug_LightGreen);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightBlue", (void*)&Debug_LightBlue);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightCyan", (void*)&Debug_LightCyan);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightMagenta", (void*)&Debug_Magenta);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightYellow", (void*)&Debug_LightYellow);
	as_engine->RegisterGlobalProperty("const color32 Debug_LightGray", (void*)&Debug_LightGray);



	// Font and Sprite Atlas
	as_engine->RegisterGlobalFunction("void mgdl_CreateFont(TextureHandle texture, s16 characterWidth, s16 characterHeight, char firstCharacter)", asGLFunctionLink(mgdl_CreateFont), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_CreateSpriteAtlas(TextureHandle texture, s16 spriteWidth, s16 spriteHeight)", asGLFunctionLink(mgdl_CreateSpriteAtlas), asGLFunctionCall);


	as_engine->RegisterGlobalFunction("void mgdl_DrawSprite(TextureHandle texture, u16 spriteIndex, float x, float y, color32 color)", asGLFunctionLink(mgdl_DrawSprite), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawSprite(TextureHandle texture, u16 spriteIndex, float x, float y, float scale, color32 color)", asGLFunctionLink(mgdl_DrawSpriteEx), asGLFunctionCall);

	// Palette functions
	as_engine->RegisterGlobalFunction("PaletteHandle mgdl_GetDefaultPalette()", asGLFunctionLink(mgdl_GetDefaultPalette), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("PaletteHandle mgdl_GetDebugPalette()", asGLFunctionLink(mgdl_GetDebugPalette), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_SetPalette(PaletteHandle palette)", asGLFunctionLink(mgdl_SetPalette), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("color32 mgdl_GetPaletteColor(PaletteHandle palette, u8 colorIndex)", asGLFunctionLink(mgdl_GetPaletteColor), asGLFunctionCall);

	// Register drawing functions
	as_engine->RegisterGlobalFunction("void mgdl_DrawTexture(TextureHandle textureHandle, float x, float y)", asGLFunctionLink(mgdl_DrawTexture), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawTextureEx(TextureHandle textureHandle, float x, float y, float rotationDeg, float scale)", asGLFunctionLink(mgdl_DrawTextureEx), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawLine(float x1, float y2, float x2, float y2, color32 color)", asGLFunctionLink(mgdl_DrawLine), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("void mgdl_DrawLineGradient( float x1, float y1, float x2, float y2, color32 color1 , color32 color2)", asGLFunctionLink(mgdl_DrawLineGradient), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("void mgdl_DrawRectangle(float x, float y, float w, float h, color32 color)", asGLFunctionLink(mgdl_DrawRectangle), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawRectangleLines(float x, float y, float w, float h, color32 color)", asGLFunctionLink(mgdl_DrawRectangleLines), asGLFunctionCall);


	// NOTE Text drawing

	as_engine->RegisterGlobalFunction("void mgdl_DrawText(const zstr &in text, float x, float y, float fontSize, color32 color)", asGLFunctionLinkPR(mgdl_DrawText, (const zstr&, float, float, float, color32), void), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawTextEx(TextureHandle font,  const zstr &in text, float x, float y, float fontSize, color32 color)", asGLFunctionLinkPR(mgdl_DrawTextEx, (TextureHandle, const zstr&, float, float, float, color32), void), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("void mgdl_DrawTextInt(const zstr &in text, int number, float x, float y, float fontSize, color32 color)", asGLFunctionLink(mgdl_DrawTextInt), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawTextFloat(const zstr &in text, float number, float x, float y, float fontSize, color32 color)", asGLFunctionLink(mgdl_DrawTextFloat), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawInt( int number, float x, float y, float fontSize, color32 color)", asGLFunctionLink(mgdl_DrawInt), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawFloat( float number, float x, float y, float fontSize, color32 color)", asGLFunctionLink(mgdl_DrawFloat), asGLFunctionCall);

	// NOTE Models

	as_engine->RegisterGlobalFunction("void mgdl_DrawScene(SceneHandle font, float x, float y, float z, float scale, color32 color)", asGLFunctionLinkPR(mgdl_DrawScene, (SceneHandle, float, float, float, float, color32), void), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_DrawModel(ModelHandle font, float x, float y, float z, float scale, color32 color)", asGLFunctionLinkPR(mgdl_DrawModel, (ModelHandle, float, float, float, float, color32), void), asGLFunctionCall);

	as_engine->RegisterGlobalFunction(
		"void mgdl_DrawModelRotated(ModelHandle font, float x, float y, float z, float xdeg, float ydeg, float zdeg, float scale, color32 color)",
		asGLFunctionLinkPR(mgdl_DrawModelRotated,
					 (ModelHandle,
					  float, float, float,
					  float, float, float,
					float, color32), void), asGLFunctionCall);

	// TEXTURE AND SPRITE

	as_engine->RegisterGlobalFunction("int mgdl_GetTextureWidth(TextureHandle handle)", asGLFunctionLink(mgdl_GetTextureWidth), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("int mgdl_GetTextureHeight(TextureHandle handle)", asGLFunctionLink(mgdl_GetTextureHeight), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("int mgdl_GetSpriteWidth(TextureHandle handle)", asGLFunctionLink(mgdl_GetSpriteWidth), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("int mgdl_GetSpriteHeight(TextureHandle handle)", asGLFunctionLink(mgdl_GetSpriteHeight), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("float mgdl_GetSpriteAspect(TextureHandle handle)", asGLFunctionLink(mgdl_GetSpriteAspect), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_SetTextureFilterMag(TextureHandle handle, TextureFilterModes mode)", asGLFunctionLink(mgdl_SetTextureFilterMag),asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_SetTextureFilterMin(TextureHandle handle, TextureFilterModes mode)", asGLFunctionLink(mgdl_SetTextureFilterMin),asGLFunctionCall);
}

static void RegisterController(asIScriptEngine* as_engine)
{
	as_engine->RegisterEnum("WiiButtons");
	as_engine->RegisterEnumValue("WiiButtons", "ButtonNone", 	(int)ButtonNone);
	as_engine->RegisterEnumValue("WiiButtons", "Button2", 	(int)Button2);
	as_engine->RegisterEnumValue("WiiButtons", "Button1", 	(int)Button1);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonB", 	(int)ButtonB);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonA", 	(int)ButtonA);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonMinus", (int)ButtonMinus);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonHome", 	(int)ButtonHome);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonLeft", 	(int)ButtonLeft);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonRight", (int)ButtonRight);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonDown", 	(int)ButtonDown);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonUp", 	(int)ButtonUp);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonPlus", 	(int)ButtonPlus);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonZ"	,	(int)ButtonZ);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonC"	,	(int)ButtonC);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonAny", 	0xFFFFFFu);

	as_engine->RegisterEnum("WiiJoystick");
	as_engine->RegisterEnumValue("WiiJoystick", "Joystick_Nunchuk",(int)Joystick_Nunchuk);
	as_engine->RegisterEnumValue("WiiJoystick", "Joystick_LeftStick",(int)Joystick_LeftStick);
	as_engine->RegisterEnumValue("WiiJoystick", "Joystick_RightStick",(int)Joystick_RightStick);


	as_engine->RegisterGlobalFunction("bool mgdl_IsButtonDown(int controller, WiiButtons button)", asGLFunctionLink(mgdl_IsButtonDown), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("bool mgdl_IsButtonPressed(int controller, WiiButtons button)", asGLFunctionLink(mgdl_IsButtonPressed), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("bool mgdl_IsButtonReleased(int controller, WiiButtons button)", asGLFunctionLink(mgdl_IsButtonReleased), asGLFunctionCall);

	as_engine->RegisterGlobalFunction("float mgdl_GetJoystickX(int controller, WiiJoystick joystick)", asGLFunctionLink(mgdl_GetJoystickX), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("float mgdl_GetJoystickY(int controller, WiiJoystick joystick)", asGLFunctionLink(mgdl_GetJoystickY), asGLFunctionCall);
}

static void RegisterSound(asIScriptEngine* as_engine)
{
	as_engine->RegisterEnum("mgdlAudioStateEnum");
	as_engine->RegisterEnumValue("mgdlAudioStateEnum", "Audio_StatePlaying", 	(int)Audio_StatePlaying);
	as_engine->RegisterEnumValue("mgdlAudioStateEnum", "Audio_StatePaused",	(int)Audio_StatePaused);
	as_engine->RegisterEnumValue("mgdlAudioStateEnum", "Audio_StateStopped",	(int)Audio_StateStopped);
	as_engine->RegisterEnumValue("mgdlAudioStateEnum", "Audio_StateInvalid",	(int)Audio_StateInvalid);

	as_engine->RegisterGlobalFunction("void mgdl_PlaySound(SoundHandle soundHandle)", asGLFunctionLink(mgdl_PlaySound), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_PauseSound(SoundHandle soundHandle)", asGLFunctionLink(mgdl_PauseSound), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("void mgdl_StopSound(SoundHandle soundHandle)", asGLFunctionLink(mgdl_StopSound), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("mgdlAudioStateEnum mgdl_GetSoundStatus(SoundHandle handle)", asGLFunctionLink(mgdl_GetSoundStatus), asGLFunctionCall);
	as_engine->RegisterGlobalFunction("int mgdl_GetSoundElapsedMs(SoundHandle handle)", asGLFunctionLink(mgdl_GetSoundElapsedMs), asGLFunctionCall);

}


void RegisterMGDL(asIScriptEngine* as_engine)
{
	RegisterTypes(as_engine);
	RegisterMain(as_engine);
	RegisterDrawing(as_engine);
	RegisterSound(as_engine);
	RegisterController(as_engine);
	RegisterOpenGLUtilsAndGlut(as_engine);
}
