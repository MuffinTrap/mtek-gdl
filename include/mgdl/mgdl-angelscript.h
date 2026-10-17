#pragma once
#include <angelscript.h>
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-angelscript.h
 * @brief Includes functions for loading and running AngelScript files.
 */


// Platform defines for calling convetions
// TODO Method link, OBJ_FIRST function link

#if defined(_WIN32)
#   define asGLFunctionCall asCALL_STDCALL
#   define asGLFunctionLink asFUNCTION
#	define asGLFunctionLinkPR asFUNCTIONPR

#elif defined(GEKKO)
    // On the Wii need to use Generic calling convention for time being
    // https://www.angelcode.com/angelscript/sdk/docs/manual/doc_addon_autowrap.html
#   include <autowrapper/aswrappedcall.h>
#   define asGLFunctionCall asCALL_GENERIC
#   define asGLFunctionLink WRAP_FN
#	define asGLFunctionLinkPR WRAP_FN_PR
#else
#   define asGLFunctionCall asCALL_CDECL
#   define asGLFunctionLink asFUNCTION
#	define asGLFunctionLinkPR asFUNCTIONPR
#endif

#if defined(__cplusplus)
extern "C" {
#endif

	enum AngelScriptStatus
	{
		Angel_Running,
		Angel_CompileError,
		Angel_SourceChanged
	};
	typedef enum AngelScriptStatus AngelScriptStatus;

	enum AngelScriptFunctionStatus
	{
		Angel_NotPrepared,
		Angel_Prepared
	};
	typedef enum AngelScriptFunctionStatus AngelScriptFunctionStatus;

	/**
	 * @brief Struct representing the AngelScript engine and functions
	 */
	struct mgdl_AngelScript
	{
		asIScriptEngine *engine;
		asIScriptContext* ctx;
		zstr mainScriptFile;
		zstr moduleName;
		zstr scriptFileType;
		bool dmonInitDone;
		CallbackFunction scriptModifiedCallback;
		AngelScriptStatus status;
		AngelScriptFunctionStatus functionStatus;
	};
	typedef struct mgdl_AngelScript mgdl_AngelScript;

	/**
	 * @brief Allocates and initializes a new AngelScript object to use with scripts.
	 * @details This allocates the object and creates the engine. It also registers mgdl functions and script add-ons. Register your own functions before calling mgdl_LoadAngelScript
	 * @returns Pointer to the initialized object
	 */
	mgdl_AngelScript* mgdl_InitAngelScript();

	/**
	 * @brief Register a callback to be called when angelscript files change in the watched directory.
	 * @note When the callback is called, you can call mgdl_RecompileAngelScript at the end of frame. The Callback can come any time, even during execution!
	 * @param angel AngelScript
	 * @param callback The function to call. Must be type void(void)
	 */
	void mgdl_RegisterAngelScriptModifiedCallback(mgdl_AngelScript* angel, CallbackFunction callback);
	/**
	 * @brief Recompiles the main script file.
	 * @param angel AngelScript
	 * @returns True if compilation succeeded
	 */
	bool mgdl_RecompileAngelScript(mgdl_AngelScript* angel);

	/**
	 * @brief Loads the script file and compiles it.
	 * @note If the main script includes other files, they are compiled as well.
	 * @param angel The AngelScript object from mgdl_InitAngelScript
	 * @param mainScript Name of main script file.
	 * @param hotloadDirectory Name of the directory which is watched for changing. Set to nullptr to disable hotloading.
	 * @param moduleName Optional name for the module. If set to nullptr a default name is used
	 * @returns If the loading and compilation was successfull.
	 */
	bool mgdl_LoadAngelScriptFiles(mgdl_AngelScript* angel, const char* mainScript, const char* hotloadDirectory, const char* moduleName);

	// Custom functions for games and demos
	/**
	 * @brief Look for a function in anglescript by declaration
	 * @param angel The AngelScript object with the module
	 * @param functionDeclaration The function declaration
	 * @returns Function pointer or nullptr if not found
	 */
	asIScriptFunction* mgdl_FindAngelScriptFunction(mgdl_AngelScript* angel, const char* functionDeclaration);

	/**
	 * @brief Prepares AngelScript function for calling.
	 * @note Call this before settings parameters and committing the call
	 * @param angel The AngelScript object with the module
	 * @param function The function to prepare
	 */
	void mgdl_PrepareAngelScriptFunctionCall(mgdl_AngelScript* angel, asIScriptFunction* function);
	/**
	 * @brief Sets an integer parameter value for the prepared function call
	 * @param angel The AngelScript object with the module
	 * @param function The function to prepare
	 * @param parameterIndex Index of the parameter
	 * @param value Value of the parameter
	 */
	void mgdl_SetAngelScriptFunctionParameterInt(mgdl_AngelScript* angel, asIScriptFunction* function, int parameterIndex, int value);
	/**
	 * @brief Sets a floating point parameter value for the prepared function call
	 * @param angel The AngelScript object with the module
	 * @param function The function to prepare
	 * @param parameterIndex Index of the parameter
	 * @param value Value of the parameter
	 */
	void mgdl_SetAngelScriptFunctionParameterFloat(mgdl_AngelScript* angel, asIScriptFunction* function, int parameterIndex, float value);
	/**
	 * @brief Sets a boolean point parameter value for the prepared function call
	 * @param angel The AngelScript object with the module
	 * @param function The function to prepare
	 * @param parameterIndex Index of the parameter
	 * @param value Value of the parameter
	 */
	void mgdl_SetAngelScriptFunctionParameterBool(mgdl_AngelScript* angel, asIScriptFunction* function, int parameterIndex, bool value);
	/**
	 * @brief Commit a prepared function call
	 * @param angel The AngelScript object with the module
	 * @param angel The function to call
	 */
	void mgdl_CommitAngelScriptFunctionCall(mgdl_AngelScript* angel, asIScriptFunction* function);


	/**
	 * @brief Deinitializes the AngelScript engine and hotloading and frees all memory
	 * @param angel The AngelScript object to deinit
	 */
	void mgdl_DeinitAngelScript(mgdl_AngelScript* angel);


#if defined(__cplusplus)
}
#endif
