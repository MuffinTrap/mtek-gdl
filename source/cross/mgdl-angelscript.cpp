#include <mgdl/mgdl-angelscript.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-memory.h>
#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-console.h>
#include <mgdl/mgdl-script-api.h>

// AngelScript add-ons
#include <scriptbuilder/scriptbuilder.h>
#include <scriptmath/scriptmath.h>
#include <scriptarray/scriptarray.h>

// Registering functions
#include <scriptopengl/ScriptOpenGL.h>
#include <scriptmgdl/scriptmgdl.h>


// static AngelScript variables
static bool s_compileErrorFlag = false;
static int s_errorCount = 0;
static const int ERROR_LIMIT = 10;

static void AngelScriptMessageCallback(const asSMessageInfo *msg)
{
	zstr filename = zstr_from(msg->section);
	zstr_view fileview = zstr_as_view(&filename);
	// Find last '/'
	int slash = -1;
	for (int i = zstr_len(&filename)-1; i>=0; i--)
	{
		char c = msg->section[i];
		if (c == '/')
		{
			slash = i+1;
			fileview = zstr_sub(fileview, slash, zstr_len(&filename)-i);
			break;
		}
	}

	zstr justfile = zstr_from_view(fileview);
	const char* file = zstr_cstr(&justfile);

    switch(msg->type)
    {
        case asMSGTYPE_ERROR:
			s_compileErrorFlag = true;
			if (s_errorCount < ERROR_LIMIT)
			{
				Log_ErrorF("%s:%d:%d %s\n", file, msg->row, msg->col, msg->message);
				s_errorCount += 1;
			}
            break;
        case asMSGTYPE_WARNING:
            Log_WarningF("%s:%d:%d %s\n", file, msg->row, msg->col, msg->message);
            break;
        case asMSGTYPE_INFORMATION:
            Console_Print(ConsoleText_White, "AngelScript:"); Log_InfoF("%s\n", msg->message);
            break;
    }
    zstr_free(&filename);
    zstr_free(&justfile);
}

static bool ReloadAngelScriptCode(mgdl_AngelScript* angel)
{
	ASSERT_DEBUG(angel != nullptr);
	const char* moduleName = zstr_cstr(&angel->moduleName);
	const char* script = zstr_cstr(&angel->mainScriptFile);
	if (script == nullptr)
	{
		Log_Error("Cannot reload AngelScript, no main file set");
	}

	// If module exists already, discard it
    asIScriptModule *mod = angel->engine->GetModule(moduleName);
    if (mod != nullptr)
    {
        mod->Discard();
    }

    CScriptBuilder builder;

	// Set preprocessor directives to angelscript
	builder.DefineWord("USE_ANGEL_AS_SCRIPT");

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
	s_errorCount = 0;
    result = builder.BuildModule();

	if (result < 0 && s_compileErrorFlag)
	{
		// Code had errors
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
#	define DMON_LOG_ERROR(s) Log_Error(s)
#	define DMON_SLEEP_INTERVAL 64
	#include <mgdl/dmon/dmon.h>

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
				//mgdl_BufferPrintf("%s%s", rootdir, filepath);
				//mgdl_GetPrintfBuffer());

				// Actually, always compile the main file
				// that should #include everything else
				ReloadAngelScriptCode(angel);
				mgdl_RunAngelScriptInit(angel);
			}
		}
	}
#endif

mgdl_AngelScript* mgdl_InitAngelCpp(AngelInitFuncType initFunc, AngelFrameFuncType frameFunc, AngelQuitFuncType quitFunc)
{
	mgdl_InitScriptApi();
	mgdl_AngelScript* angel = (mgdl_AngelScript*)mgdl_AllocateGeneralMemory(sizeof(mgdl_AngelScript));

	angel->engine = nullptr;
	angel->ctx = nullptr;
	angel->frameFunc = nullptr;
	angel->initFunc = nullptr;
	angel->quitFunc = nullptr;
	angel->dmonInitDone = false;
	mgdl_assert_print(initFunc != nullptr && frameFunc != nullptr && quitFunc != nullptr, "Must provide pointers to angelscript c++ functions when initializing AngelScript as CPP");
	angel->cxxInitFunc = initFunc;
	angel->cxxFrameFunc = frameFunc;
	angel->cxxQuitFunc = quitFunc;
	return angel;
}

mgdl_AngelScript* mgdl_InitAngelScript()
{
	mgdl_InitScriptApi();
	mgdl_AngelScript* angel =(mgdl_AngelScript*)mgdl_AllocateGeneralMemory(sizeof(mgdl_AngelScript));

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

	return angel;
}


bool mgdl_LoadAngelScriptFiles(mgdl_AngelScript* angel, const char* mainScript, const char* hotloadDirectory, const char* moduleName)
{
    angel->ctx = angel->engine->CreateContext();
	angel->frameFunc = nullptr;
	angel->initFunc = nullptr;
	angel->quitFunc = nullptr;
	angel->cxxInitFunc = nullptr;
	angel->cxxFrameFunc = nullptr;
	angel->cxxQuitFunc = nullptr;
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
		Log_InfoF("AngelScript filetype read as '%s'\n", zstr_cstr(&angel->scriptFileType));

		// file.cxx
		// 01234567
		//     ^index
		// zstr_len returns 8
		// 8-4 = 4
		// zstr_sub(4,4) -> ".cxx"
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
		dmon_watch_id watch_id = dmon_watch(hotloadDirectory, dmon_WatchCallback, DMON_WATCHFLAGS_RECURSIVE, angel);
		if (watch_id.id == 0)
		{
			Log_ErrorF("Dmon cannot watch directory %s\n", hotloadDirectory);
		}
#		endif
	}

	// Load the given file to module
	return ReloadAngelScriptCode(angel);
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
	ASSERT_DEBUG(angel != nullptr);
	if (angel->cxxInitFunc != nullptr)
	{
		angel->cxxInitFunc();
	}
	else if (s_compileErrorFlag == false)
	{
		RunAngelFunctionVoidVoid(angel, angel->initFunc);
	}
}

void mgdl_RunAngelScriptFrame(mgdl_AngelScript* angel, float deltatime)
{
	ASSERT_DEBUG(angel != nullptr);
	if (angel->cxxFrameFunc != nullptr)
	{
		angel->cxxFrameFunc(deltatime);
	}
	else if (s_compileErrorFlag == false)
	{
		RunAngelFunctionVoidFloat(angel, angel->frameFunc, deltatime);
	}
}

void mgdl_RunAngelScriptQuit(mgdl_AngelScript* angel)
{
	ASSERT_DEBUG(angel != nullptr);
	if (angel->cxxQuitFunc != nullptr)
	{
		angel->cxxQuitFunc();
	}
	else if (s_compileErrorFlag == false)
	{
		RunAngelFunctionVoidVoid(angel, angel->quitFunc);
	}
}

void mgdl_DeinitAngelScript(mgdl_AngelScript* angel)
{
	if (angel != nullptr)
	{
		angel->ctx->Release();
		angel->engine->ShutDownAndRelease();
		zstr_free(&angel->mainScriptFile);
		zstr_free(&angel->scriptFileType);
		zstr_free(&angel->moduleName);
	#	if defined(GEKKO)
			// nop
	#	else
			if (angel->dmonInitDone)
			{
				dmon_deinit();
			}
	#	endif
		mgdl_FreeGeneralMemory(angel);
	}
}

