#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-splash.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/wii/mgdl-wii.h>

static Platform* platformWii;

static std::function<void()> initCall = nullptr;
static std::function<void()> updateCall = nullptr;
static std::function<void()> drawCall = nullptr;

static WiiController controller;

static void ReadControllers();
static void MainLoop();
static void SplashHoldLoop(bool SplashFlag, bool HoldAFlag);
static u64 deltaTimeStart;

void Platform_Init(const char* name,
	gdl::ScreenAspect screenAspect,
								  std::function<void ()> initCallback,
								  std::function<void ()> updateCallback,
								  std::function<void ()> drawCallback,
								  u32 initFlags)
{
	gdl_assert_print(initCallback != nullptr, "Need to provide init callback before system init on PC");
	gdl_assert_print(updateCallback != nullptr, "Need to provide update callback before system init on PC");
	gdl_assert_print(drawCallback != nullptr, "Need to provide draw callback before system init on PC");

	initCall = initCallback;
	drawCall = drawCallback;
	updateCall = updateCallback;

	platformWii = new Platform();

	platformWii->name = name;

	// Convert to Wii InitAspectmode for now
	gdl::InitAspectMode mode = gdl::InitAspectMode::AspectAuto;
	switch(screenAspect)
	{
		case gdl::ScreenAuto:
			mode = gdl::InitAspectMode::AspectAuto;
			break;
		case gdl::Screen4x3:
			mode = gdl::InitAspectMode::Aspect4x3;
			platformWii->aspectRatio = 4.0f/3.0f;
			break;
		case gdl::Screen16x9:
			mode = gdl::InitAspectMode::Aspect16x9;
			platformWii->aspectRatio = 16.0f/9.0f;
			break;
	};
	fatInitDefault();
	gdl::InitSystem(gdl::ModeAuto, mode, gdl::HiRes, gdl::InitFlags::OpenGX);

    platformWii->screenWidth = gdl::ScreenXres;
    platformWii->screenHeight = gdl::ScreenYres;
	if (screenAspect == gdl::ScreenAuto)
	{
		platformWii->aspectRatio = platformWii->screenWidth / platformWii->screenHeight;
	}

	ogx_initialize();
	// Init controller
	// TODO Add init parameters for what controllers to init?
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

	WiiController_Init(&controller, WPAD_CHAN_0);
	WiiController_ZeroAllInputs(&controller);

	gdl::ConsoleMode();

	// printf("Got resolution: %d x %d\n", screenWidth, screenHeight);
	initCall();
	u64 now = gettime();
	deltaTimeStart = now;

	const bool SplashFlag = (initFlags & gdl::PlatformInitFlag::FlagSplashScreen)!= 0;
	const bool HoldAFlag = (initFlags & gdl::PlatformInitFlag::FlagPauseUntilA)!= 0;
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
		platformWii->_deltaTimeS = (float)(now - deltaTimeStart) / (float)(TB_TIMER_CLOCK * 1000); // division is to convert from ticks to seconds
		deltaTimeStart = now;
		platformWii->_elapsedTimeS += platformWii->_deltaTimeS;
		WiiController_StartFrame(&controller);
		ReadControllers();

		if (SplashFlag)
		{
			gdl::PrepDisplay();
			splashProgress = gdl::DrawSplashScreen(platformWii->_deltaTimeS, showHoldAMessage, aHoldTimer);
		}

		if (showHoldAMessage)
		{
			if (WiiController_ButtonHeld(&controller, WiiButtons::ButtonA))
			{
				aHoldTimer += platformWii->_deltaTimeS;
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
	platformWii->_elapsedTimeS = 0.0f;
}



void MainLoop()
{
	while(true)
	{
		// Timing
		u64 now = gettime();
		platformWii->_deltaTimeS = (float)(now - deltaTimeStart) / (float)(TB_TIMER_CLOCK * 1000); // division is to convert from ticks to seconds
		deltaTimeStart = now;
		platformWii->_elapsedTimeS += platformWii->_deltaTimeS;

		WiiController_StartFrame(&controller);
		ReadControllers();
		updateCall();
		gdl::PrepDisplay();
		drawCall();
		glFlush();
		gdl::Display();
	}
}


WiiController* Platform_GetController(int controllerNumber)
{
	if (controllerNumber == 0)
	{
		return &controller;
	}
	return &controller;
}

void ReadControllers()
{
	// TODO This might have to be in a macro
	WPAD_ScanPads();  // Scan the Wiimotes
	WPADData *data1 = WPAD_Data(controller._channel);

	const ir_t &ir = data1->ir;
	// Multiply x and y to match them to 16:9 screen
	controller._cursorX = ir.x * 1.67f - 16.f;
	controller._cursorY = ir.y * 1.2f - 16.f;

	controller._pressedButtons = WPAD_ButtonsDown(0);
	controller._releasedButtons = WPAD_ButtonsUp(0);
	controller._heldButtons = WPAD_ButtonsHeld(0);

	controller._nunchukJoystickDirectionX=0.0f;
	controller._nunchukJoystickDirectionY=0.0f;
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
		controller._nunchukJoystickDirectionX = dirx * n.mag;
		controller._nunchukJoystickDirectionY = diry * n.mag;
	}

	controller._roll = DegToRad(data1->orient.roll);
}

Platform* Platform_GetSingleton() { return platformWii; }

float Platform_GetDeltaTime() { return platformWii->_deltaTimeS; }
float Platform_GetElapsedSeconds() { return platformWii->_elapsedTimeS; }

void Platform_DoProgramExit()
{
	delete platformWii;
	gdl::wii::DoProgramExit();
}
