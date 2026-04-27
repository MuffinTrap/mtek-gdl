#pragma once
#include <angelscript.h>
#include <mgdl/zstrh/zstr.h>

#if defined(__cplusplus)
extern "C" {
#endif

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
		zstr mainScriptFile;
		zstr moduleName;
		zstr scriptFileType;
		bool dmonInitDone;
	};
	typedef struct mgdl_AngelScript mgdl_AngelScript;

	/**
	 * @brief Allocates and initializes a new AngelScript object.
	 * @param mainScript Name of main script file.
	 * @param hotloadDirectory Name of the directory which is watched for changing. Set to nullptr to disable hotloading.
	 * @param moduleName Optional name for the module. If set to nullptr a default name is used
	 * @returns Pointer to the initialized object
	 */
	mgdl_AngelScript* mgdl_InitAngelScript(const char* mainScript, const char* hotloadDirectory, const char* moduleName);

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

	// When angelscript is used as C++ code, the script functions are declared here
	#if defined(USE_ANGEL_AS_CPP)
	void angelscript_init();
	void angelscript_frame(float deltatime);
	void angelscript_quit();
	#endif

#if defined(__cplusplus)
}
#endif
