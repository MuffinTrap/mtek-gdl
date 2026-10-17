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
		angel->status = Angel_CompileError;
		return false;
	}
    else if (result < 0)
	{
		Log_ErrorF("Failed to build angel module\n");
		return false;
	}

    mod = angel->engine->GetModule(moduleName);
    mgdl_assert_print(mod != nullptr, "Failed to get module\n");

    Log_InfoF("Loaded AngelScipt file %s\n", script);
	angel->status = Angel_Running;
    return true;
}


asIScriptFunction* mgdl_FindAngelScriptFunction(mgdl_AngelScript* angel, const char* functionDeclaration)
{
	const char* moduleName = zstr_cstr(&angel->moduleName);
    asIScriptModule* mod = angel->engine->GetModule(moduleName);
    mgdl_assert_print(mod != nullptr, "Failed to get module\n");

    asIScriptFunction* function = mod->GetFunctionByDecl(functionDeclaration);
    if( function == nullptr )
    {
        // The function couldn't be found. Instruct the script writer
        // to include the expected function in the script.
        Log_ErrorF("The script does not have the function '%s'. Please add it and try again.\n", functionDeclaration);
    }
    return function;
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
				angel->status = Angel_SourceChanged;
				if (angel->scriptModifiedCallback != nullptr)
				{
					angel->scriptModifiedCallback();
				}
			}
		}
	}
#endif

mgdl_AngelScript* mgdl_InitAngelScript()
{
	mgdl_InitScriptApi();
	mgdl_AngelScript* angel =(mgdl_AngelScript*)mgdl_AllocateGeneralMemory(sizeof(mgdl_AngelScript));

    angel->engine = asCreateScriptEngine();
    int result = angel->engine->SetMessageCallback(asFUNCTION(AngelScriptMessageCallback), 0, asCALL_CDECL);
    mgdl_assert_print(result >= 0, "Failed setting AngelScript message callback\n");
	angel->status = Angel_Running;
	angel->functionStatus = Angel_NotPrepared;
	angel->scriptModifiedCallback = nullptr;

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

void mgdl_RegisterAngelScriptModifiedCallback(mgdl_AngelScript* angel, CallbackFunction callback)
{
	angel->scriptModifiedCallback = callback;
}

bool mgdl_RecompileAngelScript(mgdl_AngelScript* angel)
{
	return ReloadAngelScriptCode(angel);
}


bool mgdl_LoadAngelScriptFiles(mgdl_AngelScript* angel, const char* mainScript, const char* hotloadDirectory, const char* moduleName)
{
    angel->ctx = angel->engine->CreateContext();
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


void mgdl_PrepareAngelScriptFunctionCall(mgdl_AngelScript* angel, asIScriptFunction* function)
{
	if (function != nullptr)
	{
		if (angel->functionStatus == Angel_NotPrepared)
		{
			angel->ctx->Prepare(function);
			angel->functionStatus = Angel_Prepared;
		}
		else
		{
			Log_Warning("AngelScript function was already prepared\n");
		}
	}
}

void mgdl_SetAngelScriptFunctionParameterFloat(mgdl_AngelScript* angel, asIScriptFunction* function, int parameterIndex, float value)
{
	if (function != nullptr)
	{
		if (angel->functionStatus == Angel_Prepared)
		{
			angel->ctx->SetArgFloat(parameterIndex, value);
		}
		else
		{
			Log_ErrorF("Cannot set AngelScript function parameter value. Function not prepared\n");
		}
	}
}

static void PrintExceptionInfo(asIScriptContext *ctx)
{
  const asIScriptFunction *function = ctx->GetExceptionFunction();
  int exceptionColumn = 0;
  int exceptionLine = ctx->GetExceptionLineNumber(&exceptionColumn);
  Log_ErrorF("Description: %s\nModule: %s\n, Function: %s Line: %d:%d\n",
			 ctx->GetExceptionString(),
			function->GetModuleName(),
			function->GetDeclaration(),
			 exceptionLine, exceptionColumn
			);
}

void mgdl_CommitAngelScriptFunctionCall(mgdl_AngelScript* angel, asIScriptFunction* function)
{
	if (function != nullptr)
	{
		if (s_compileErrorFlag == false)
		{
			if (angel->functionStatus == Angel_Prepared)
			{
				int runResult = angel->ctx->Execute();
				if (runResult == asEXECUTION_EXCEPTION)
				{
					Log_Error("AngelScript function execution resulted in exception\n");
					PrintExceptionInfo(angel->ctx);
				}
				else if (runResult == asEXECUTION_ERROR)
				{
					Log_Error("AngelScript function execution resulted in error\n");
				}
				else
				{
					angel->functionStatus = Angel_NotPrepared;
				}
			}
			else
			{
				Log_Error("Cannot call AngelScrip function. Call not prepared");
			}
		}
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

