
#include <mgdl.h>
#include "../angelscript.h"
#include "scriptmgdl.h"
#include "ZStrFactory.h"
#include <mgdl/mgdl-types.h>
#include <mgdl/mgdl-script-api.h>

static void RegisterOpenGLUtilsAndGlut(asIScriptEngine* as_engine)
{
	// GLU functions
	as_engine->RegisterGlobalFunction("void gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upz, GLdouble upz)", asFUNCTION(gluLookAt), asCALL_CDECL);

	as_engine->RegisterGlobalFunction("void gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFarz)", asFUNCTION(gluPerspective), asCALL_CDECL);


	// mgdl utils
	as_engine->RegisterGlobalFunction("void mgdl_InitOrthoProjection()", asFUNCTION(mgdl_InitOrthoProjection), asCALL_CDECL);
}




static ZStrFactory stringFactory;

static void RegisterTypes(asIScriptEngine* as_engine)
{

	// Register typedefs
	as_engine->RegisterTypedef("u8", "uint8");
	as_engine->RegisterTypedef("s8", "int8");

	as_engine->RegisterTypedef("u16", "uint16");
	as_engine->RegisterTypedef("s16", "int16");

	as_engine->RegisterTypedef("u32", "uint32");
	as_engine->RegisterTypedef("s32", "int32");

	as_engine->RegisterTypedef("u64", "uint64");
	as_engine->RegisterTypedef("s64", "int64");

#ifdef GEKKO
	as_engine->RegisterTypedef("sizetype", "uint32");
	as_engine->RegisterTypedef("Handle", "uint32");
#else
	as_engine->RegisterTypedef("sizetype", "uint64");
	as_engine->RegisterTypedef("Handle", "uint64");
#endif

	// Register zstr as the string type
	as_engine->RegisterObjectType("zstr", sizeof(zstr), asOBJ_VALUE|asOBJ_POD| asOBJ_APP_CLASS);
	as_engine->RegisterStringFactory("zstr", &stringFactory);

	// Register handle type
	//as_engine->RegisterObjectType("Handle", sizeof(Handle), asOBJ_VALUE|asOBJ_POD);
}

static void RegisterMain(asIScriptEngine* as_engine)
{
	// Get status functions
	as_engine->RegisterGlobalFunction("int mgdl_GetScreenHeight()", asFUNCTION(mgdl_GetScreenHeight), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("int mgdl_GetScreenWidth()", asFUNCTION(mgdl_GetScreenWidth), asCALL_CDECL);

	// Asset handling
	as_engine->RegisterGlobalFunction("Handle mgdl_LoadTexture(const zstr &in filename)", asFUNCTION(mgdl_LoadTexture), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("Handle mgdl_LoadPalette(const zstr &in filename)", asFUNCTION(mgdl_LoadPalette), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("Handle mgdl_LoadSound(const zstr &in filename)", asFUNCTION(mgdl_LoadSound), asCALL_CDECL);
}

static void RegisterDrawing(asIScriptEngine* as_engine)
{
	as_engine->RegisterGlobalFunction("Handle  mgdl_GetDefaultPalette()", asFUNCTION(mgdl_GetDefaultPalette), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("Handle  mgdl_GetDebugPalette()", asFUNCTION(mgdl_GetDebugPalette), asCALL_CDECL);
	as_engine->RegisterGlobalFunction("void  mgdl_SetPalette(Handle palette)", asFUNCTION(mgdl_SetPalette), asCALL_CDECL);

	// Register drawing functions
	as_engine->RegisterGlobalFunction("void  mgdl_DrawRectangle(s16 x, s16 y, s16 w, s16 h, u8 paletteIndex)", asFUNCTION(mgdl_DrawRectangle), asCALL_CDECL);


	as_engine->RegisterGlobalFunction("void  mgdl_DrawTexture(Handle textureHandle, s16 y, s16 w)", asFUNCTION(mgdl_DrawTexture), asCALL_CDECL);
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


void RegisterMGDL(asIScriptEngine* as_engine)
{
	mgdl_InitScriptApi();
	RegisterTypes(as_engine);
	RegisterMain(as_engine);
	RegisterDrawing(as_engine);
	RegisterController(as_engine);
	RegisterOpenGLUtilsAndGlut(as_engine);
}
