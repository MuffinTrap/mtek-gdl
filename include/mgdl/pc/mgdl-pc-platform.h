#pragma once

#include <sndfile.h>

#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-openal.h>

namespace gdl
{
	class PlatformPC : public gdl::Platform
	{
	public:
		void InitSystem(const char* name,
						gdl::ScreenAspect screenAspect,
						std::function<void ()> initCallback,
						std::function<void ()> updateCallback,
						std::function<void ()> drawCallback,
						u32 initFlags) override;
						gdl::WiiController& GetController(int controllerNumber) override;
						void DoProgramExit() override;

		float GetDeltaTime() override;
		float GetElapsedSeconds() override;
	private:
		void InitAudio();
		// OpenAL sound
		ALCdevice* device;
		ALCcontext* context;
	};
}
