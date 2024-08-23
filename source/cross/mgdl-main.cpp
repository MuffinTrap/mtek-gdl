#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-png.h>
#include <mgdl/mgdl-image.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/mgdl-font.h>

#ifdef GEKKO
#include <mgdl-wii/mgdl-wii-sound.h>
#else
#include <mgdl-pc/mgdl-pc-sound.h>
#endif

void gdl::InitSystem(gdl::ScreenAspect screenAspect,
								std::function<void()> initCallback,
								std::function<void()> updateCallback,
								std::function<void()> drawCallback,
								u32 initFlags)
{
	gdl::Platform::GetPlatform().InitSystem(screenAspect, initCallback, updateCallback, drawCallback, initFlags);
}

gdl::Image* gdl::LoadImage(std::string filename, gdl::TextureFilterModes filterMode)
{
	gdl::Image* img = new gdl::Image();
	img->LoadFile(filename.c_str(), filterMode);
	return img;
}

gdl::Sound* gdl::LoadSound(std::string filename)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::SoundWii();
#else
	snd = new gdl::SoundPC();
#endif
	snd->LoadFile(filename.c_str());
	return snd;
}

gdl::PNGFile* gdl::LoadPNG(std::string filename)
{
	gdl::PNGFile* png = new gdl::PNGFile();
	png->ReadFile(filename.c_str());
	return png;
}

gdl::Font* gdl::LoadFont(std::string filename, short characterWidth, short characterHeight, char firstCharacter)
{
	gdl::Font* a = new gdl::Font();
	a->LoadFromImage(filename.c_str(), characterWidth, characterHeight, firstCharacter);
	return a;
}

void gdl::ReadController(gdl::WiiController& controllerInOut)
{
	gdl::Platform::GetPlatform().ReadControllerInput(controllerInOut);
}

u16 gdl::GetScreenWidth() { return gdl::Platform::GetPlatform().GetScreenWidth();}
u16 gdl::GetScreenHeight(){ return gdl::Platform::GetPlatform().GetScreenHeight();};
float gdl::GetAspectRatio(){ return gdl::Platform::GetPlatform().GetAspectRatio();};
float gdl::GetElapsedSeconds(){ return gdl::Platform::GetPlatform().GetElapsedSeconds();};
float gdl::GetDeltaTime(){ return gdl::Platform::GetPlatform().GetDeltaTime();};
