#include <mgdl/mgdl-platform.h>

#ifdef GEKKO
	#include <mgdl/wii/mgdl-wii-platform.h>
#else
	#include <mgdl/pc/mgdl-pc-platform.h>
#endif

u16 gdl::Platform::GetScreenHeight() { return screenHeight; }
u16 gdl::Platform::GetScreenWidth() {return screenWidth;}
float gdl::Platform::GetAspectRatio() { return aspectRatio; }

gdl::Platform::Platform()
{
	// nop
	screenWidth = 640;
	screenHeight = 480;
	aspect = gdl::ScreenAspect::Screen4x3;
	deltaTimeS = 0.0f;
	elapsedTimeS = 0.0f;
}

const char* gdl::Platform::GetName()
{
	return name;
}



gdl::Platform & gdl::Platform::GetPlatform()
{
#ifdef GEKKO
	static gdl::Platform* instance = new gdl::PlatformWii();
#else
	static gdl::Platform* instance = new gdl::PlatformPC();
#endif
	return *instance;
}



