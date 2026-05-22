#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assetmanager.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-fbx.h>

#include <mgdl/mgdl-sound.h>
#include <mgdl/mgdl-audio.h>

void mgdl_InitSystem(const char* windowName,
	ScreenAspect screenAspect,
	CallbackFunction initCallback,
	CallbackFunction frameCallback,
	CallbackFunction quitCallback,
	u32 initFlags)
{
#if defined(MGDL_DEBUG)
	Log_Warning("YOU ARE USING DEBUG BUILD OF the mgdl LIBRARY\n");
#endif
	Platform_Init(windowName, screenAspect, initCallback, frameCallback, quitCallback, initFlags);
}

WiiController* mgdl_GetController( int controllerNumber)
{
	return Platform_GetController(controllerNumber);
}

void mgdl_DoProgramExit()
{
	Platform_DoProgramExit();
}

Viewport mgdl_GetViewport() { return Platform_GetViewport(); }
int mgdl_GetScreenWidth() { return Platform_GetViewport().width;}
int mgdl_GetScreenHeight() { return Platform_GetViewport().height;}
float mgdl_GetAspectRatio(){ return Platform_GetAspectRatio(); }
float mgdl_GetElapsedSeconds(){ return Platform_GetElapsedSeconds(); };
u32 mgdl_GetElapsedFrames(){ return Platform_GetElapsedFrames(); };
float mgdl_GetDeltaTime(){ return Platform_GetDeltaTime(); };
