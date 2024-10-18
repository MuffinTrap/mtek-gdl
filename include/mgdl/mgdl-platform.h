#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"

#include <functional>
// Interface class for platform

namespace gdl
{
	class WiiController;

	enum PlatformInitFlag
	{
		FlagPauseUntilA = 0x01 // Enters a loop after system init and continues when A button is pressed
	};

	class Platform
	{
		public:
			virtual void InitSystem(gdl::ScreenAspect screenAspect,
									std::function<void()> initCallback,
									std::function<void()> updateCallback,
									std::function<void()> drawCallback,
									u32 initFlags = 0 ) = 0;
			virtual gdl::WiiController& GetController(int controllerNumber) = 0;
			virtual void DoProgramExit() = 0;

			static Platform& GetPlatform();

			u16 GetScreenWidth();
			u16 GetScreenHeight();
			float GetAspectRatio();
			virtual float GetElapsedSeconds() = 0;
			virtual float GetDeltaTime() = 0;

		protected:
			Platform();

			short screenWidth;
			short screenHeight;
			float aspectRatio;
			gdl::ScreenAspect aspect;
			float deltaTimeS;
			float elapsedTimeS;
	};
}

