#include "mgdl-scripting.h"
#include <scriptbuilder/scriptbuilder.h>
#include <scriptmath/scriptmath.h>
#include <scriptopengl/ScriptOpenGL.h>
#include <scriptarray/scriptarray.h>

#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-draw2d.h>
#include <mgdl/mgdl-opengl_util.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/ccVector/ccVector.h>

// TODO Functions for drawing
// Enum names for default colors

void mgdl_DrawRectangle(float x, float y, float w, float h, DefaultColor color)
{
	Draw2D_RectWH(x, y, w, h, Color_GetDefaultColor(color));
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


#if defined(GEKKO)
	// No file watching on Wii
	// NOP
#else
	#define DMON_IMPL
	#include "dmon/dmon.h"
	static bool dmon_InitDone = false;

	void dmon_WatchCallback(dmon_watch_id watch_id, dmon_action action, const char* rootdir,
							const char* filepath, const char* oldfilepath, void* user)
	{
		// Only add .angel or .c files
		zstr_view filename = zstr_view_from(filepath);
		if (action == DMON_ACTION_MODIFY)
		{
			if (zstr_view_ends_with(filename, ".c")
				|| zstr_view_ends_with(filename, ".angel")
				|| zstr_view_ends_with(filename, ".as")
			)
			{
				Log_InfoF("Dmon callback on %s%s\n", rootdir, filepath);
				mgdl_BufferPrintf("%s%s", rootdir, filepath);
				mgdl_LoadAngelScript((mgdl_AngelScriptContext*)user, mgdl_GetPrintfBuffer());
				mgdl_RunAngelScriptInit((mgdl_AngelScriptContext*)user);
			}
		}
	}
#endif

static bool compileErrorFlag = false;

static void AngelScriptMessageCallback(const asSMessageInfo *msg)
{
    switch(msg->type)
    {
        case asMSGTYPE_ERROR:
            Log_ErrorF("Error: AngelScript: Section %s, row %d, col %d message: %s\n", msg->section, msg->row, msg->col, msg->message);
			compileErrorFlag = true;
            break;
        case asMSGTYPE_WARNING:
            Log_WarningF("Warning: AngelScript: Section %s, row %d, col %d message: %s\n", msg->section, msg->row, msg->col, msg->message);
            break;
        case asMSGTYPE_INFORMATION:
            Log_InfoF("Info: AngelScript: Section %s, row %d, col %d message: %s\n", msg->section, msg->row, msg->col, msg->message);
            break;
    }
}


void RegisterOpenGLFunctionsAndTypes(asIScriptEngine* as_engine)
{
	// Basic OpenGL functions
	RegisterOpenGL(as_engine);

	// GLU functions

	int result = as_engine->RegisterGlobalFunction("void gluLookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez, GLdouble centerx, GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upz, GLdouble upz)", asFUNCTION(gluLookAt), asCALL_CDECL);

	result = as_engine->RegisterGlobalFunction("void gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear, GLdouble zFarz)", asFUNCTION(gluPerspective), asCALL_CDECL);


	// mgdl utils
	result = as_engine->RegisterGlobalFunction("void mgdl_InitOrthoProjection()", asFUNCTION(mgdl_InitOrthoProjection), asCALL_CDECL);
}

void RegisterMain(asIScriptEngine* as_engine)
{
	int result = as_engine->RegisterGlobalFunction("int mgdl_GetScreenHeight()", asFUNCTION(mgdl_GetScreenHeight), asCALL_CDECL);
	result = as_engine->RegisterGlobalFunction("int mgdl_GetScreenWidth()", asFUNCTION(mgdl_GetScreenWidth), asCALL_CDECL);
}

void RegisterDrawing(asIScriptEngine* as_engine)
{
	// Register DefaultColor
	int result = as_engine->RegisterEnum("DefaultColor");
	result = as_engine->RegisterEnumValue("DefaultColor" , "Color_White", (int)Color_White);
	result = as_engine->RegisterEnumValue("DefaultColor" , "Color_Black", (int)Color_Black);
	result = as_engine->RegisterEnumValue("DefaultColor" , "Color_Red", (int)Color_Red);
	result = as_engine->RegisterEnumValue("DefaultColor" , "Color_Green", (int)Color_Green);
	result = as_engine->RegisterEnumValue("DefaultColor" , "Color_Blue", (int)Color_Blue);

	// Register drawing functions
	result = as_engine->RegisterGlobalFunction("void  mgdl_DrawRectangle(float x, float y, float w, float h, DefaultColor color)", asFUNCTION(mgdl_DrawRectangle), asCALL_CDECL);
}

void RegisterController(asIScriptEngine* as_engine)
{
	int result = as_engine->RegisterEnum("WiiButtons");
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonNone", 	0x0000);
	result = as_engine->RegisterEnumValue("WiiButtons", "Button2", 		0x0001);
	result = as_engine->RegisterEnumValue("WiiButtons", "Button1", 		0x0002);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonB", 		0x0004);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonA", 		0x0008);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonMinus", 	0x0010);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonHome", 	0x0080);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonLeft", 	0x0100);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonRight", 	0x0200);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonDown", 	0x0400);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonUp", 		0x0800);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonPlus", 	0x1000);
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonZ"	,		(0x0001 << 16));
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonC"	,		(0x0002 << 16));
	result = as_engine->RegisterEnumValue("WiiButtons", "ButtonAny", 	0xFFF);

	result = as_engine->RegisterGlobalFunction("bool mgdl_IsButtonDown(int controller, WiiButtons button)", asFUNCTION(mgdl_IsButtonDown), asCALL_CDECL);
	result = as_engine->RegisterGlobalFunction("bool mgdl_IsButtonPressed(int controller, WiiButtons button)", asFUNCTION(mgdl_IsButtonPressed), asCALL_CDECL);
}


mgdl_AngelScriptContext* mgdl_InitAngelScript()
{
	mgdl_AngelScriptContext* context = (mgdl_AngelScriptContext*)mgdl_AllocateGeneralMemory(sizeof(mgdl_AngelScriptContext));
    context->as_engine = asCreateScriptEngine();
    int result = context->as_engine->SetMessageCallback(asFUNCTION(AngelScriptMessageCallback), 0, asCALL_CDECL);
    mgdl_assert_print(result >= 0, "Failed setting AngelScript message callback\n");

	// Arrays with int[] array declaration
	RegisterScriptArray(context->as_engine, true);
	// Add std math functions
	RegisterScriptMath(context->as_engine);
	// Add OpenGL 1.1 functions and types
	RegisterOpenGLFunctionsAndTypes(context->as_engine);
	// Register mgdl functions
	RegisterMain(context->as_engine);
	RegisterController(context->as_engine);
	RegisterDrawing(context->as_engine);

    context->as_ctx = context->as_engine->CreateContext();

	context->as_frameFunc = nullptr;
	context->as_initFunc = nullptr;

	return context;
}

void mgdl_SetDirectoryForAngelScriptHotReload(mgdl_AngelScriptContext* context, const char* directory)
{
#if defined(GEKKO)
	// NOP
#else
	if (dmon_InitDone == false)
	{
		dmon_init();
		dmon_InitDone = true;
	}
	dmon_watch(directory, dmon_WatchCallback, DMON_WATCHFLAGS_RECURSIVE, context);
#endif
}

bool mgdl_LoadAngelScript(mgdl_AngelScriptContext* context, const char* script)
{
    asIScriptModule *mod = context->as_engine->GetModule("MyModule");
    if (mod != nullptr)
    {
        mod->Discard();
    }

    CScriptBuilder builder;

	// Set preprocessor directives to angelscript
#if defined(USE_ANGEL_AS_SCRIPT)
	builder.DefineWord("USE_ANGEL_AS_SCRIPT");
#elif defined(USE_ANGEL_AS_CPP)
	builder.DefineWord("USE_ANGEL_AS_CPP");
#endif
    int result = builder.StartNewModule(context->as_engine, "MyModule");
    mgdl_assert_print(result >= 0, "Failed starting module\n");

    result = builder.AddSectionFromFile(script);
    mgdl_assert_print(result >= 0, "Failed to add test.angel\n");

    result = builder.BuildModule();
	if (result < 0 && compileErrorFlag)
	{
		// Code had errors
		compileErrorFlag = false;
		return false;
	}
    mgdl_assert_print(result >= 0, "Failed to build module\n");

    mod = context->as_engine->GetModule("MyModule");
    mgdl_assert_print(mod != nullptr, "Failed to get module\n");

    context->as_initFunc = mod->GetFunctionByDecl("void angelscript_init()");
    if( context->as_initFunc == 0 )
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        printf("The script must have the function 'void angelscript_init(float deltatime)'. Please add it and try again.\n");
        return false;
    }
    context->as_frameFunc = mod->GetFunctionByDecl("void angelscript_frame(float deltatime)");
    if( context->as_frameFunc == 0 )
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        printf("The script must have the function 'void angelscript_frame(float deltatime)'. Please add it and try again.\n");
        return false;
    }
    printf("Loaded AngelScipt file %s\n", script);
    return true;

}

void mgdl_RunAngelScriptInit(mgdl_AngelScriptContext* context)
{
#if defined(USE_ANGEL_AS_CPP)
	angelscript_init();
#elif defined(USE_ANGEL_AS_SCRIPT)
	if (context->as_initFunc != nullptr)
	{
		context->as_ctx->Prepare(context->as_initFunc);
		int runResult = context->as_ctx->Execute();
		if (runResult != asEXECUTION_FINISHED)
		{
			if (runResult == asEXECUTION_EXCEPTION)
			{
				// An exception occurred, let the script writer know what happened so it can be corrected.
				Log_ErrorF("An exception '%s' occurred. Please correct the code and try again.\n", context->as_ctx->GetExceptionString());
			}

		}
	}
#endif
}
void mgdl_RunAngelScriptFrame(mgdl_AngelScriptContext* context)
{
#if defined(USE_ANGEL_AS_CPP)
	angelscript_frame(mgdl_GetDeltaTime());
#elif defined(USE_ANGEL_AS_SCRIPT)
	if (context->as_frameFunc != nullptr)
	{
		context->as_ctx->Prepare(context->as_frameFunc);
		// pass deltatime
		context->as_ctx->SetArgFloat(0, mgdl_GetDeltaTime());
		int runResult = context->as_ctx->Execute();
		if (runResult != asEXECUTION_FINISHED)
		{
			if (runResult == asEXECUTION_EXCEPTION)
			{
				// An exception occurred, let the script writer know what happened so it can be corrected.
				Log_ErrorF("An exception '%s' occurred. Please correct the code and try again.\n", context->as_ctx->GetExceptionString());
			}

		}
	}
#endif
}

void mgdl_DeinitAngelScript(mgdl_AngelScriptContext* context)
{
    context->as_ctx->Release();
    context->as_engine->ShutDownAndRelease();
	mgdl_FreeGeneralMemory(context);
#if defined(GEKKO)
	// nop
#else
	if (dmon_InitDone)
	{
		dmon_deinit();
	}
#endif
}

