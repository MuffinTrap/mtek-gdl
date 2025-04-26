#pragma once

#include "mgdl-types.h"
#include "mgdl-controller.h"

#include <cstring>
#include <string.h>
#include <string>
#include <functional>
// Interface class for platform

namespace gdl
{
	class Platform
	{
		public:
			virtual void InitSystem(const char* name,
									gdl::ScreenAspect screenAspect,
									std::function<void()> initCallback,
									std::function<void()> updateCallback,
									std::function<void()> drawCallback,
									u32 initFlags = 0 ) = 0;
			virtual WiiController* GetController(int controllerNumber) = 0;
			virtual void DoProgramExit() = 0;

			static Platform& GetPlatform();

			const char* GetName();
			u16 GetScreenWidth();
			u16 GetScreenHeight();
			float GetAspectRatio();
			virtual float GetElapsedSeconds() = 0;
			virtual float GetDeltaTime() = 0;

		protected:
			Platform();

			const char* name;
			short screenWidth;
			short screenHeight;
			float aspectRatio;
			gdl::ScreenAspect aspect;
			float deltaTimeS;
			float elapsedTimeS;
	};
}

