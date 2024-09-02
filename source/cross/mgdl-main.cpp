#include <mgdl/mgdl-main.h>
#include <mgdl/mgdl-platform.h>

#ifdef GEKKO
#include <mgdl/wii/mgdl-wii-sound.h>
#else
#include <mgdl/pc/mgdl-pc-sound.h>
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
	if(img->LoadFile(filename.c_str(), filterMode))
	{
		return img;
	}
	else
	{
		delete img;
		return nullptr;
	}
}

gdl::Image* gdl::LoadImage(gdl::PNGFile* png, gdl::TextureFilterModes filterMode)
{
	gdl::Image* img = new gdl::Image();
	if (img->LoadPNG(png, filterMode))
	{
		return img;
	}
	else
	{
		delete img;
		return nullptr;
	}
}

gdl::Sound* gdl::LoadSound(std::string filename)
{
	gdl::Sound* snd = nullptr;
#ifdef GEKKO
	snd = new gdl::SoundWii();
#else
	snd = new gdl::SoundPC();
#endif
	if(snd->LoadFile(filename.c_str()))
	{
		return snd;
	}
	else
	{
		delete snd;
		return nullptr;
	}
}

gdl::PNGFile* gdl::LoadPNG(std::string filename)
{
	gdl::PNGFile* png = new gdl::PNGFile();
	if (png->ReadFile(filename.c_str()))
	{
		return png;
	}
	else
	{
		delete png;
		return nullptr;
	}
}

gdl::Font* gdl::LoadFont(std::string filename, short characterWidth, short characterHeight, char firstCharacter)
{
	gdl::Font* font = new gdl::Font();
	if (font->LoadFromImage(filename.c_str(), characterWidth, characterHeight, firstCharacter))
	{
		return font;
	}
	else
	{
		delete font;
		return nullptr;
	}
}

gdl::WiiController& gdl::GetController( int controllerNumber)
{
	return gdl::Platform::GetPlatform().GetController(controllerNumber);
}

void gdl::DoProgramExit()
{
	gdl::Platform::GetPlatform().DoProgramExit();
}

u16 gdl::GetScreenWidth() { return gdl::Platform::GetPlatform().GetScreenWidth();}
u16 gdl::GetScreenHeight(){ return gdl::Platform::GetPlatform().GetScreenHeight();};
float gdl::GetAspectRatio(){ return gdl::Platform::GetPlatform().GetAspectRatio();};
float gdl::GetElapsedSeconds(){ return gdl::Platform::GetPlatform().GetElapsedSeconds();};
float gdl::GetDeltaTime(){ return gdl::Platform::GetPlatform().GetDeltaTime();};
