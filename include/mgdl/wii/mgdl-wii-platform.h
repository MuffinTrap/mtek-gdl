#pragma once

#include <mgdl/mgdl-platform.h>

namespace gdl
{
	class PlatformWii : public gdl::Platform
	{
	public:
		void InitSystem(gdl::ScreenAspect screenAspect,
						std::function<void ()> initCallback,
						std::function<void ()> updateCallback,
						std::function<void ()> drawCallback,
						u32 initFlags) override;

		gdl::WiiController& GetController(int controllerNumber) override;
		void DoProgramExit() override;

		float GetDeltaTime() override;
		float GetElapsedSeconds() override;
	private:
		void ReadControllers();
	};
}
