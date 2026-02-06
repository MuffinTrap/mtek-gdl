#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-splash.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/wii/mgdl-wii.h>
#include "mgdl/wii/mgdl-wii-globals-internal.h"

static CallbackFunction initCall = nullptr;
static CallbackFunction frameCall = nullptr;
static CallbackFunction quitCall = nullptr;

static void ReadControllers();
static void MainLoop();
static void SplashHoldLoop(bool SplashFlag, bool HoldAFlag);
static u64 deltaTimeStart;
static u32 frameCount;

static Platform platformWii;

void Platform_Init(const char* windowName,
	ScreenAspect screenAspect,
	CallbackFunction initCallback,
	CallbackFunction frameCallback,
	CallbackFunction quitCallback,
	u32 initFlags)
{
	mgdl_assert_print(initCallback != nullptr, "Need to provide init callback before system init on Wii");
	mgdl_assert_print(frameCallback != nullptr, "Need to provide update callback before system init on Wii");

	initCall = initCallback;
	frameCall = frameCallback;
	quitCall = quitCallback;

	platformWii.initFlags = initFlags;

	platformWii.windowName = windowName;

	// Convert to Wii InitAspectmode for now
	gdl::InitAspectMode mode = gdl::InitAspectMode::AspectAuto;
	switch(screenAspect)
	{
		case ScreenAuto:
			mode = gdl::InitAspectMode::AspectAuto;
			platformWii.aspect = ScreenAspect::ScreenAuto;
			break;
		case Screen4x3:
			mode = gdl::InitAspectMode::Aspect4x3;
			platformWii.aspectRatio = 4.0f/3.0f;
			platformWii.aspect = ScreenAspect::Screen4x3;
			break;
		case Screen16x9:
			mode = gdl::InitAspectMode::Aspect16x9;
			platformWii.aspectRatio = 16.0f/9.0f;
			platformWii.aspect = ScreenAspect::Screen16x9;
			break;
	};
	fatInitDefault();

	// TODO set aspect ratio as requested
	gdl::InitSystem(gdl::ModeAuto, mode, gdl::HiRes, gdl::InitFlags::OpenGX);

    platformWii.screenWidth = gdl::ScreenXres;
    platformWii.screenHeight = gdl::ScreenYres;
	if (screenAspect == ScreenAuto)
	{
		platformWii.aspectRatio = platformWii.screenWidth / platformWii.screenHeight;
		// TODO read system aspect ratio
		if (gdl::wii::WidescreenMode)
		{
			platformWii.aspect = ScreenAspect::Screen16x9;
		}
		else
		{
			platformWii.aspect = ScreenAspect::Screen4x3;
		}
	}

	ogx_initialize();
	// Init controller
	// TODO Add init parameters for what controllers to init?
	WPAD_Init();

	for (int i = 0; i < MGDL_MAX_CONTROLLERS; i++)
	{
		WPAD_SetDataFormat(WPAD_CHAN_0 + i, WPAD_FMT_BTNS_ACC_IR);
		WiiController_Init(&platformWii.controllers[i], WPAD_CHAN_0 + i);
		WiiController_ZeroAllInputs(&platformWii.controllers[i]);
	}

	Audio_Init(nullptr);

	gdl::ConsoleMode();

	// printf("Got resolution: %d x %d\n", screenWidth, screenHeight);
	initCall();
	u64 now = gettime();
	deltaTimeStart = now;
	frameCount = 0;

	const bool SplashFlag = (initFlags & PlatformInitFlag::FlagSplashScreen)!= 0;
	const bool HoldAFlag = (initFlags & PlatformInitFlag::FlagPauseUntilA)!= 0;
    if (SplashFlag || HoldAFlag)
    {
		if (!SplashFlag && HoldAFlag)
		{
			printf("Hold A to start");
		}
		SplashHoldLoop(SplashFlag, HoldAFlag);
    }

    MainLoop();
}

void SplashHoldLoop(bool SplashFlag, bool HoldAFlag)
{
	float aHoldTimer = 0.0f;
	float splashProgress = 0.0f;
	bool showHoldAMessage = HoldAFlag;
	bool waiting = true;

	while(waiting)
	{
		u64 now = gettime();
		platformWii.deltaTimeS = (float)(now - deltaTimeStart) / (float)(TB_TIMER_CLOCK * 1000); // division is to convert from ticks to seconds
		deltaTimeStart = now;
		platformWii.elapsedTimeS += platformWii.deltaTimeS;
		ReadControllers();

		if (SplashFlag)
		{
			gdl::PrepDisplay();
			splashProgress = DrawSplashScreen(platformWii.deltaTimeS, showHoldAMessage, aHoldTimer);
		}

		if (showHoldAMessage)
		{
			if (WiiController_ButtonHeld(Platform_GetController(0), WiiButtons::ButtonA))
			{
				aHoldTimer += platformWii.deltaTimeS;
				if (aHoldTimer >= 1.0f)
				{
					waiting = false;
				}
			}
			else
			{
				aHoldTimer = 0.0f;
			}
		}
		else
		{
			waiting = (splashProgress <= 1.0f);
		}

		if (SplashFlag)
		{
			glFlush();
			gdl::Display();
		}
		else
		{
			VIDEO_WaitVSync();
		}
	}
	// Reset elapsed time so game gets correct timing
	platformWii.elapsedTimeS = 0.0f;
}



void MainLoop()
{
	while(true)
	{
		// Timing
		// TODO how is gdl::Delta different from this?
		u64 now = gettime();
		platformWii.deltaTimeS = (float)(now - deltaTimeStart) / (float)(TB_TIMER_CLOCK * 1000); // division is to convert from ticks to seconds
		deltaTimeStart = now;
		platformWii.elapsedTimeS += platformWii.deltaTimeS;

		ReadControllers();
		gdl::PrepDisplay();
		frameCall();
		glFlush();
		gdl::Display();

		if (Flag_IsSet(platformWii.initFlags, FlagGameHandlesHOME) == false)
		{
			if (WiiController_ButtonPress(Platform_GetController(0), ButtonHome))
			{
				if (quitCall != NULL)
				{
					quitCall();
				}
				Platform_DoProgramExit();
			}
		}

		frameCount++;
	}
}

// TODO How do we know?
bool Platform_IsControllerConnected(int controllerIndex)
{
	if (controllerIndex >= 0 && controllerIndex < MGDL_MAX_CONTROLLERS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

WiiController* Platform_GetController(int controllerNumber)
{
	return &platformWii.controllers[controllerNumber];
}

void ReadControllers()
{
	// TODO This might have to be in a macro
	WPAD_ScanPads();  // Scan the Wiimotes
	for (int i = 0; i < MGDL_MAX_CONTROLLERS; i++)
	{
		WiiController* controller = &platformWii.controllers[i];
		WiiController_StartFrame(controller);

		WPADData *data1 = WPAD_Data(controller->m_channel);

		const ir_t &ir = data1->ir;
		controller->m_cursorX = ir.x;
		float y = platformWii.screenHeight - ir.y;
		controller->m_cursorY = y;

		if(platformWii.aspect == Screen16x9)
		{
			// Multiply x and y to match them to 16:9 screen
			controller->m_cursorX *= 1.67f - 16.f;
			controller->m_cursorY *= 1.2f - 16.f;
		}

		controller->m_pressedButtons = WPAD_ButtonsDown(0);
		controller->m_releasedButtons = WPAD_ButtonsUp(0);
		controller->m_heldButtons = WPAD_ButtonsHeld(0);

		controller->m_nunchukJoystickDirectionX=0.0f;
		controller->m_nunchukJoystickDirectionY=0.0f;
		const expansion_t &ex = data1->exp;
		if (ex.type == WPAD_EXP_NUNCHUK)
		{
			joystick_t n = ex.nunchuk.js;
			// Angle is reported in degrees
			// Angle of 0 means up.
			// 90 right, 180 down, 270 left

			float rad = DegToRad(n.ang);
			float x = 0;
			float y = -1.0f;
			float dirx = cos(rad) * x - sin(rad) * y;
			float diry = sin(rad) * x + cos(rad) * y;
			controller->m_nunchukJoystickDirectionX = dirx * n.mag;
			controller->m_nunchukJoystickDirectionY = diry * n.mag;
		}

		controller->m_roll = DegToRad(data1->orient.roll);
		controller->m_pitch = DegToRad(data1->orient.pitch);
		controller->m_yaw = DegToRad(data1->orient.yaw);
	}
}

Platform* Platform_GetSingleton() { return &platformWii; }

float Platform_GetDeltaTime() { return platformWii.deltaTimeS; }
float Platform_GetElapsedSeconds() { return platformWii.elapsedTimeS; }
u32 Platform_GetElapsedUpdates() { return frameCount;}

void Platform_DoProgramExit()
{
	gdl::wii::DoProgramExit();
}

void Platform_MapJoystickToController(int joystickIndex, int controllerIndex)
{
	if (joystickIndex > 0 && controllerIndex > 0)
	{
		joystickIndex = controllerIndex;
	}

}

int Platform_GetScreenWidth()
{
	return platformWii.screenWidth;
}
int Platform_GetScreenHeight()
{
	return platformWii.screenHeight;
}
