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
		void ReadControllerInput(gdl::WiiController& controllerInOut) override;
		void DoProgramExit() override;
};

}
