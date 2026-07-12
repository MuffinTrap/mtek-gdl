#pragma once
#include <angelscript.h>
#include <mgdl/mgdl-types.h>

/**
 * @file mgdl-angelscript.h
 * @brief Includes functions for loading and running AngelScript files.
 */

#if defined(__cplusplus)
extern "C" {
#endif
	/**
	 * @brief Function pointer types
	 */
	typedef void (*AngelInitFuncType)(void);
	typedef void (*AngelQuitFuncType)(void);
	typedef void (*AngelFrameFuncType)(float);

	/**
	 * @brief Struct representing the AngelScript engine and functions
	 */
	struct mgdl_AngelScript
	{
		asIScriptEngine *engine;
		asIScriptContext* ctx;
		asIScriptFunction* initFunc;
		asIScriptFunction* frameFunc;
		asIScriptFunction* quitFunc;
		AngelInitFuncType cxxInitFunc;
		AngelFrameFuncType cxxFrameFunc;
		AngelQuitFuncType cxxQuitFunc;
		zstr mainScriptFile;
		zstr moduleName;
		zstr scriptFileType;
		bool dmonInitDone;
	};
	typedef struct mgdl_AngelScript mgdl_AngelScript;

	/**
	 * @brief Allocates and initializes a new AngelScript object to use with scripts.
	 * @details This allocates the object and creates the engine. It also registers mgdl functions and script add-ons. Register your own functions before calling mgdl_LoadAngelScript
	 * @returns Pointer to the initialized object
	 */
	mgdl_AngelScript* mgdl_InitAngelScript();

	/**
	 * @brief Loads the script files and compiles them.
	 * @param angel The AngelScript object from mgdl_InitAngelScript
	 * @param mainScript Name of main script file.
	 * @param hotloadDirectory Name of the directory which is watched for changing. Set to nullptr to disable hotloading.
	 * @param moduleName Optional name for the module. If set to nullptr a default name is used
	 * @returns If the loading and compilation was successfull.
	 */
	bool mgdl_LoadAngelScriptFiles(mgdl_AngelScript* angel, const char* mainScript, const char* hotloadDirectory, const char* moduleName);

	/**
	 * @brief Initializes the AngelScript to be used as cpp code.
	 * @details Use the returned pointer like when using AngelScript as script as it will tell the
	 * functions if they should call the cpp function or the script function
	 * @returns A nullptr
	 */
	mgdl_AngelScript* mgdl_InitAngelCpp(AngelInitFuncType initFunc, AngelFrameFuncType frameFunc, AngelQuitFuncType quitFunc);

	/**
	 * @brief Runs the init function in the module. This is run automatically on every code change.
	 * @param angel The AngelScript object with the module
	 */
	void mgdl_RunAngelScriptInit(mgdl_AngelScript* angel);
	/**
	 * @brief Runs the frame(float deltatime) function in the module. Call this every frame.
	 * @param deltatime Delta time forwarded to the script function
	 * @param angel The AngelScript object with the module
	 */
	void mgdl_RunAngelScriptFrame(mgdl_AngelScript* angel, float deltatime);
	/**
	 * @brief Runs the quit function in the module. Call this when shutting down the host program.
	 * @param angel The AngelScript object with the module
	 */
	void mgdl_RunAngelScriptQuit(mgdl_AngelScript* angel);

	/**
	 * @brief Deinitializes the AngelScript engine and hotloading and frees all memory
	 * @param angel The AngelScript object to deinit
	 */
	void mgdl_DeinitAngelScript(mgdl_AngelScript* angel);


#if defined(__cplusplus)
}
#endif
