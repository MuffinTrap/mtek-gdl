#pragma once

#include <angelscript.h>
#include <mgdl/mgdl-color.h>
#include <mgdl/mgdl-controller.h>

#if defined(__cplusplus)
extern "C" {
#endif

void mgdl_DrawRectangle(float x, float y, float w, float h, DefaultColor color);
bool mgdl_IsButtonDown(int controller, WiiButtons button);
bool mgdl_IsButtonPressed(int controller, WiiButtons button);

// ------------------------------

	struct mgdl_AngelScriptContext
	{
		asIScriptEngine *as_engine;
		asIScriptContext* as_ctx;
		asIScriptFunction* as_initFunc;
		asIScriptFunction* as_frameFunc;
	};
	typedef struct mgdl_AngelScriptContext mgdl_AngelScriptContext;

mgdl_AngelScriptContext* mgdl_InitAngelScript();
bool mgdl_LoadAngelScript(mgdl_AngelScriptContext* context, const char* script);
void mgdl_SetDirectoryForAngelScriptHotReload(mgdl_AngelScriptContext* context, const char* directory);
void mgdl_RunAngelScriptInit(mgdl_AngelScriptContext* context);
void mgdl_RunAngelScriptFrame(mgdl_AngelScriptContext* context);
void mgdl_DeinitAngelScript(mgdl_AngelScriptContext* context);
#if defined(__cplusplus)
}
#endif
