#include "mgdl-angelscript.h"

#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-alloc.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-util.h>

// AngelScript add-ons
#include <scriptbuilder/scriptbuilder.h>
#include <scriptmath/scriptmath.h>
#include <scriptarray/scriptarray.h>

// Registering functions
#include <scriptopengl/ScriptOpenGL.h>
#include <scriptmgdl/scriptmgdl.h>


// static AngelScript variables
static bool s_compileErrorFlag = false;

static void AngelScriptMessageCallback(const asSMessageInfo *msg)
{
    switch(msg->type)
    {
        case asMSGTYPE_ERROR:
            Log_ErrorF("AngelScript Error: Section %s, row %d, col %d message: %s\n", msg->section, msg->row, msg->col, msg->message);
			s_compileErrorFlag = true;
            break;
        case asMSGTYPE_WARNING:
            Log_WarningF("AngelScript Warning: Section %s, row %d, col %d message: %s\n", msg->section, msg->row, msg->col, msg->message);
            break;
        case asMSGTYPE_INFORMATION:
            Log_InfoF("AngelScript Info: message: %s\n", msg->message);
            break;
    }
}

static bool LoadAngelScriptModule(mgdl_AngelScript* angel, const char* script)
{
	const char* moduleName = zstr_cstr(&angel->moduleName);
	if (script == nullptr)
	{
		script = zstr_cstr(&angel->mainScriptFile);
	}

	// If module exists already, discard it
    asIScriptModule *mod = angel->engine->GetModule(moduleName);
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

    int result = builder.StartNewModule(angel->engine, moduleName);
    mgdl_assert_printf(result >= 0, "Failed starting module %s\n", moduleName);

    result = builder.AddSectionFromFile(script);

    if (result < 0)
	{
		Log_ErrorF("Failed to add %s to angel module", script);
	}

	// If this process sends an error to angelscript message
	// callback the s_compileErrorFlag is set
	s_compileErrorFlag = false;
    result = builder.BuildModule();

	if (result < 0 && s_compileErrorFlag)
	{
		// Code had errors
		s_compileErrorFlag = false;
		return false;
	}
    else if (result < 0)
	{
		Log_ErrorF("Failed to build angel module\n");
		return false;
	}

    mod = angel->engine->GetModule(moduleName);
    mgdl_assert_print(mod != nullptr, "Failed to get module\n");

    angel->initFunc = mod->GetFunctionByDecl("void angelscript_init()");
    if( angel->initFunc == nullptr )
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        Log_Error("The script must have the function 'void angelscript_init()'. Please add it and try again.\n");
        return false;
    }
    angel->frameFunc = mod->GetFunctionByDecl("void angelscript_frame(float deltatime)");
    if( angel->frameFunc == nullptr )
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        Log_Error("The script must have the function 'void angelscript_frame(float deltatime)'. Please add it and try again.\n");
        return false;
    }
    angel->quitFunc = mod->GetFunctionByDecl("void angelscript_quit()");
    if( angel->quitFunc == nullptr )
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        Log_Error("The script must have the function 'void angelscript_quit()'. Please add it and try again.\n");
        return false;
    }
    Log_InfoF("Loaded AngelScipt file %s\n", script);
    return true;
}

#if defined(GEKKO)
	// No file watching on Wii
	// NOP
#else
	#define DMON_IMPL
	#include <dmon.h>

	void dmon_WatchCallback(dmon_watch_id watch_id, dmon_action action, const char* rootdir,
							const char* filepath, const char* oldfilepath, void* user)
	{
		mgdl_AngelScript* angel = (mgdl_AngelScript*)user;
		if (action == DMON_ACTION_MODIFY)
		{
			// Check that file has correct ending, if ending is set
			zstr_view filename = zstr_view_from(filepath);
			if (zstr_len(&angel->scriptFileType) > 0
				&& zstr_view_ends_with(filename, zstr_cstr(&angel->scriptFileType)))
			{
				Log_InfoF("Dmon callback on %s%s\n", rootdir, filepath);

				// Combine the folder and file into path
				mgdl_BufferPrintf("%s%s", rootdir, filepath);
				LoadAngelScriptModule(angel, mgdl_GetPrintfBuffer());
				mgdl_RunAngelScriptInit(angel);
			}
		}
	}
#endif

mgdl_AngelScript* mgdl_InitAngelScript(const char* mainScript, const char* hotloadDirectory, const char* moduleName)
{
	mgdl_AngelScript* angel = (mgdl_AngelScript*)mgdl_AllocateGeneralMemory(sizeof(mgdl_AngelScript));
    angel->engine = asCreateScriptEngine();
    int result = angel->engine->SetMessageCallback(asFUNCTION(AngelScriptMessageCallback), 0, asCALL_CDECL);
    mgdl_assert_print(result >= 0, "Failed setting AngelScript message callback\n");

	// Arrays with int[] array declaration
	RegisterScriptArray(angel->engine, true);
	// Add std math functions
	RegisterScriptMath(angel->engine);
	// Add OpenGL 1.1 functions and types
	RegisterOpenGL(angel->engine);
	// Register mgdl functions
	RegisterMGDL(angel->engine);

    angel->ctx = angel->engine->CreateContext();
	angel->frameFunc = nullptr;
	angel->initFunc = nullptr;
	angel->quitFunc = nullptr;
	angel->dmonInitDone = false;
	mgdl_assert_print(mainScript != nullptr, "Must specify main angel script file.");
	angel->mainScriptFile = zstr_from(mainScript);

	if (moduleName != nullptr)
	{
		angel->moduleName  = zstr_from(moduleName);
	}
	else
	{
		angel->moduleName = zstr_from("AngelModule");
	}

	// Extract file type from script file name
	ptrdiff_t index = zstr_find(&angel->mainScriptFile, ".");
	if (index > 0)
	{
		zstr_view nameView = zstr_as_view(&angel->mainScriptFile);
		// Take also the point => '.axx' : read length should be 4
		size_t len = zstr_len(&angel->mainScriptFile) - index;
		zstr_view filetype = zstr_sub(nameView, index, len);
		angel->scriptFileType = zstr_from_view(filetype);
		Log_InfoF("AngelScript filetype read as '%s\n'", zstr_cstr(&angel->scriptFileType));

		// file.axx
		// 01234567
		//     ^index
		// zstr_len returns 8
		// 8-4 = 4
		// zstr_sub(4,4) -> ".axx"
	}
	else
	{
		angel->scriptFileType = zstr_init();
	}

	// Set up hotloading
	if (hotloadDirectory != nullptr)
	{
#		if defined(GEKKO)
		// NOP
#		else
		dmon_init();
		angel->dmonInitDone = true;
		dmon_watch(hotloadDirectory, dmon_WatchCallback, DMON_WATCHFLAGS_RECURSIVE, angel);
#		endif
	}

	// Load the given file to module
	LoadAngelScriptModule(angel, nullptr);

	return angel;
}


static void RunAngelFunctionVoidVoid(mgdl_AngelScript* angel, asIScriptFunction* function)
{
	if (function != nullptr)
	{
		angel->ctx->Prepare(function);
		int runResult = angel->ctx->Execute();
		if (runResult != asEXECUTION_FINISHED)
		{
			if (runResult == asEXECUTION_EXCEPTION)
			{
				// An exception occurred, let the script writer know what happened so it can be corrected.
				Log_ErrorF("An exception '%s' occurred. Please correct the code and try again.\n", angel->ctx->GetExceptionString());
			}
		}
	}
}

static void RunAngelFunctionVoidFloat(mgdl_AngelScript* angel, asIScriptFunction* function, float param0)
{
	if (function != nullptr)
	{
		angel->ctx->Prepare(function);
		// pass deltatime
		angel->ctx->SetArgFloat(0, param0);
		int runResult = angel->ctx->Execute();
		if (runResult != asEXECUTION_FINISHED)
		{
			if (runResult == asEXECUTION_EXCEPTION)
			{
				// An exception occurred, let the script writer know what happened so it can be corrected.
				Log_ErrorF("An exception '%s' occurred when running angelscript function. Please correct the code and try again.\n", angel->ctx->GetExceptionString());
			}
		}
	}
}

void mgdl_RunAngelScriptInit(mgdl_AngelScript* angel)
{
#if defined(USE_ANGEL_AS_CPP)
	angelscript_init();
#elif defined(USE_ANGEL_AS_SCRIPT)
	RunAngelFunctionVoidVoid(angel, angel->initFunc);
#endif
}

void mgdl_RunAngelScriptFrame(mgdl_AngelScript* angel, float deltatime)
{
#if defined(USE_ANGEL_AS_CPP)
	angelscript_frame(deltatime);
#elif defined(USE_ANGEL_AS_SCRIPT)
	RunAngelFunctionVoidFloat(angel, angel->frameFunc, deltatime);
#endif
}

void mgdl_RunAngelScriptQuit(mgdl_AngelScript* angel)
{
#if defined(USE_ANGEL_AS_CPP)
	angelscript_quit();
#elif defined(USE_ANGEL_AS_SCRIPT)
	RunAngelFunctionVoidVoid(angel, angel->quitFunc);
#endif
}

void mgdl_DeinitAngelScript(mgdl_AngelScript* angel)
{
    angel->ctx->Release();
    angel->engine->ShutDownAndRelease();
	zstr_free(&angel->mainScriptFile);
	zstr_free(&angel->scriptFileType);
	zstr_free(&angel->moduleName);
#if defined(GEKKO)
	// nop
#else
	if (angel->dmonInitDone)
	{
		dmon_deinit();
	}
#endif
	mgdl_FreeGeneralMemory(angel);
}

