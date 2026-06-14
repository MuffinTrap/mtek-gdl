
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
	as_engine->RegisterGlobalFunction("void gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upz, GLdouble upz)", asFUNCTION(gluLookAt), asCALL_CDECL);

	as_engine->RegisterGlobalFunction("void gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFarz)", asFUNCTION(gluPerspective), asCALL_CDECL);


	// mgdl utils
	as_engine->RegisterGlobalFunction("void mgdl_InitOrthoProjection()", asFUNCTION(mgdl_InitOrthoProjection), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_glSetAlphaTest(bool enabled)", asFUNCTION(mgdl_glSetAlphaTest), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_glColor32(color32 color)", asFUNCTION(mgdl_glColor32), asCALL_CDECL);

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

	// Register constants
	    // Register a boolean flag that can be read, but not modified by the script.
}

static void RegisterMain(asIScriptEngine* as_engine)
{
	// Get status functions
	as_engine->RegisterGlobalFunction("int mgdl_GetScreenHeight()", asFUNCTION(mgdl_GetScreenHeight), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("int mgdl_GetScreenWidth()", asFUNCTION(mgdl_GetScreenWidth), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("float mgdl_GetElapsedSeconds()", asFUNCTION(mgdl_GetElapsedSeconds), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("float mgdl_GetDeltaTime()", asFUNCTION(mgdl_GetDeltaTime), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("float mgdl_DoProgramExit()", asFUNCTION(mgdl_DoProgramExit), asCALL_CDECL);



	// Asset handling
	as_engine->RegisterGlobalFunction("TextureHandle mgdl_LoadTexture(const zstr &in filename)", asFUNCTIONPR(mgdl_LoadTexture, (const zstr&), TextureHandle), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("PaletteHandle mgdl_LoadPalette(const zstr &in filename)", asFUNCTIONPR(mgdl_LoadPalette, (const zstr&), PaletteHandle), asCALL_CDECL);;
	as_engine->RegisterGlobalFunction("SoundHandle mgdl_LoadSound(const zstr &in filename)", asFUNCTIONPR(mgdl_LoadSound, (const zstr&), SoundHandle), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("SceneHandle mgdl_LoadScene(const zstr &in filename)", asFUNCTIONPR(mgdl_LoadScene, (const zstr&), SceneHandle), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("ModelHandle mgdl_LoadModel(const zstr &in filename)", asFUNCTIONPR(mgdl_LoadModel, (const zstr&), ModelHandle), asCALL_CDECL);
}

static void RegisterDrawing(asIScriptEngine* as_engine)
{
	// Default debug colors
	as_engine->RegisterGlobalProperty("const color32 Debug_Black", (void*)&Debug_Black);
	as_engine->RegisterGlobalProperty("const color32 Debug_DarkGray", (void*)&Debug_DarkGray);
	as_engine->RegisterGlobalProperty("const color32 Debug_Blue", (void*)&Debug_Blue);
	as_engine->RegisterGlobalProperty("const color32 Debug_Green", (void*)&Debug_Green);
	as_engine->RegisterGlobalProperty("const color32 Debug_Red", (void*)&Debug_Red);
	as_engine->RegisterGlobalProperty("const color32 Debug_Magenta", (void*)&Debug_Magenta);
	as_engine->RegisterGlobalProperty("const color32 Debug_Yellow", (void*)&Debug_Yellow);
	as_engine->RegisterGlobalProperty("const color32 Debug_White", (void*)&Debug_White);


	// Font and Sprite Atlas
	as_engine->RegisterGlobalFunction("void mgdl_CreateFont(TextureHandle texture, s16 characterWidth, s16 characterHeight, char firstCharacter)", asFUNCTION(mgdl_CreateFont), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_CreateSpriteAtlas(TextureHandle texture, s16 spriteWidth, s16 spriteHeight)", asFUNCTION(mgdl_CreateSpriteAtlas), asCALL_CDECL);


	as_engine->RegisterGlobalFunction("void mgdl_DrawSprite(TextureHandle texture, u16 spriteIndex, float x, float y, color32 color)", asFUNCTION(mgdl_DrawSprite), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawSprite(TextureHandle texture, u16 spriteIndex, float x, float y, float scale, color32 color)", asFUNCTION(mgdl_DrawSpriteEx), asCALL_CDECL);

	// Palette functions
	as_engine->RegisterGlobalFunction("PaletteHandle mgdl_GetDefaultPalette()", asFUNCTION(mgdl_GetDefaultPalette), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("PaletteHandle mgdl_GetDebugPalette()", asFUNCTION(mgdl_GetDebugPalette), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_SetPalette(PaletteHandle palette)", asFUNCTION(mgdl_SetPalette), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("color32 mgdl_GetPaletteColor(PaletteHandle palette, u8 colorIndex)", asFUNCTION(mgdl_GetPaletteColor), asCALL_CDECL);

	// Register drawing functions
	as_engine->RegisterGlobalFunction("void mgdl_DrawTexture(TextureHandle textureHandle, float x, float y)", asFUNCTION(mgdl_DrawTexture), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawTextureEx(TextureHandle textureHandle, float x, float y, float rotationDeg, float scale)", asFUNCTION(mgdl_DrawTextureEx), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawRectangle(float x, float y, float w, float h, color32 color)", asFUNCTION(mgdl_DrawRectangle), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawText(const zstr &in text, float x, float y, float fontSize, color32 color)", asFUNCTIONPR(mgdl_DrawText, (const zstr&, float, float, float, color32), void), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawTextEx(TextureHandle font,  const zstr &in text, float x, float y, float fontSize, color32 color)", asFUNCTIONPR(mgdl_DrawTextEx, (TextureHandle, const zstr&, float, float, float, color32), void), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawScene(SceneHandle font, float x, float y, float z, float scale, color32 color)", asFUNCTIONPR(mgdl_DrawScene, (SceneHandle, float, float, float, float, color32), void), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_DrawModel(ModelHandle font, float x, float y, float z, float scale, color32 color)", asFUNCTIONPR(mgdl_DrawModel, (ModelHandle, float, float, float, float, color32), void), asCALL_CDECL);

	as_engine->RegisterGlobalFunction(
		"void mgdl_DrawModelRotated(ModelHandle font, float x, float y, float z, float xdeg, float ydeg, float zdeg, float scale, color32 color)",
		asFUNCTIONPR(mgdl_DrawModelRotated,
					 (ModelHandle,
					  float, float, float,
					  float, float, float,
					float, color32), void), asCALL_CDECL);

}

static void RegisterController(asIScriptEngine* as_engine)
{
	as_engine->RegisterEnum("WiiButtons");
	as_engine->RegisterEnumValue("WiiButtons", "ButtonNone", 	0x0000);
	as_engine->RegisterEnumValue("WiiButtons", "Button2", 		0x0001);
	as_engine->RegisterEnumValue("WiiButtons", "Button1", 		0x0002);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonB", 		0x0004);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonA", 		0x0008);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonMinus", 	0x0010);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonHome", 	0x0080);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonLeft", 	0x0100);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonRight", 	0x0200);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonDown", 	0x0400);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonUp", 		0x0800);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonPlus", 	0x1000);
	as_engine->RegisterEnumValue("WiiButtons", "ButtonZ"	,		(0x0001 << 16));
	as_engine->RegisterEnumValue("WiiButtons", "ButtonC"	,		(0x0002 << 16));
	as_engine->RegisterEnumValue("WiiButtons", "ButtonAny", 	0xFFF);

	as_engine->RegisterGlobalFunction("bool mgdl_IsButtonDown(int controller, WiiButtons button)", asFUNCTION(mgdl_IsButtonDown), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("bool mgdl_IsButtonPressed(int controller, WiiButtons button)", asFUNCTION(mgdl_IsButtonPressed), asCALL_CDECL);
}

static void RegisterSound(asIScriptEngine* as_engine)
{
	as_engine->RegisterGlobalFunction("void mgdl_PlaySound(SoundHandle soundHandle)", asFUNCTION(mgdl_PlaySound), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_PauseSound(SoundHandle soundHandle)", asFUNCTION(mgdl_PauseSound), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void mgdl_StopSound(SoundHandle soundHandle)", asFUNCTION(mgdl_StopSound), asCALL_CDECL);

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
