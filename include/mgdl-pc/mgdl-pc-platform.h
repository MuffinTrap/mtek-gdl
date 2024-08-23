#pragma once

#include <mgdl/mgdl-platform.h>

#include <sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>

namespace gdl
{
	class PlatformPC : public gdl::Platform
	{
	public:
		void InitSystem(gdl::ScreenAspect screenAspect,
						std::function<void ()> initCallback,
						std::function<void ()> updateCallback,
						std::function<void ()> drawCallback,
						u32 initFlags) override;
						void ReadControllerInput(gdl::WiiController& controllerInOut) override;
						void DoProgramExit() override;
	private:
		void InitAudio();
		// OpenAL sound
		ALCdevice* device;
		ALCcontext* context;
	};
}
