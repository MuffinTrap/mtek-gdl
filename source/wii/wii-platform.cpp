#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>

#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-splash.h>
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-controller.h>
#include <mgdl/wii/mgdl-wii.h>
#include "mgdl/wii/mgdl-wii-globals-internal.h"

static CallbackFunction initCall = nullptr;
static CallbackFunction frameCall = nullptr;
static CallbackFunction quitCall = nullptr;

static void MainLoop();
static void SplashHoldLoop(bool SplashFlag, bool HoldAFlag);

static Platform platformWii;
static void CheckForQuit()
{
    WiiController* firstController = Platform_GetController(0);
    // Test for ESC unless game handles it
	if (Flag_IsSet(platformWii.initFlags, FlagGameHandlesHOME) == false)
	{
		if (WiiController_ButtonPress(firstController, ButtonHome))
		{
			if (quitCall != NULL)
			{
				quitCall();
			}
			Platform_DoProgramExit();
		}
	}
}


void Platform_Init(const char* windowName,
	ScreenAspect screenAspect,
	CallbackFunction initCallback,
	CallbackFunction frameCallback,
	CallbackFunction quitCallback,
	u32 initFlags)
{
	mgdl_assert_print(initCallback != nullptr, "Need to provide init callback before system init on Wii");

	initCall = initCallback;
	frameCall = frameCallback;
	quitCall = quitCallback;

	platformWii.initFlags = initFlags;

	// Convert to Wii InitAspectmode for now
	gdl::InitAspectMode mode = gdl::InitAspectMode::AspectAuto;
	switch(screenAspect)
	{
		case ScreenAuto:
			mode = gdl::InitAspectMode::AspectAuto;
			break;
		case Screen4x3:
			mode = gdl::InitAspectMode::Aspect4x3;
			break;
		case Screen16x9:
			mode = gdl::InitAspectMode::Aspect16x9;
			break;
	};
	fatInitDefault();

	// TODO set aspect ratio as requested
	gdl::InitSystem(gdl::ModeAuto, mode, gdl::HiRes, gdl::InitFlags::OpenGX);

	Platform_SetWindowNameAndAspect(windowName, platformWii.aspect);
	Platform_ResizeWindow(gdl::ScreenXres, gdl::ScreenYres);
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
	platformWii.applicationStartMs = ticks_to_millisecs(now);
	platformWii.elapsedFrames = 0;

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

    if (frameCall != nullptr)
	{
		MainLoop();
	}
}

void SplashHoldLoop(bool SplashFlag, bool HoldAFlag)
{
	float aHoldTimer = 0.0f;
	float splashProgress = 0.0f;
	bool showHoldAMessage = HoldAFlag;
	bool waiting = true;

	while(waiting)
	{
		Platform_FrameStart();
		Platform_ReadControllers();
		CheckForQuit();

		if (SplashFlag)
		{
			Platform_RenderStart();
			splashProgress = DrawSplashScreen(Platform_GetDeltaTime(), showHoldAMessage, aHoldTimer);
		}

		if (showHoldAMessage)
		{
			if (WiiController_ButtonHeld(Platform_GetController(0), WiiButtons::ButtonA))
			{
				aHoldTimer += Platform_GetDeltaTime();
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
			Platform_RenderEnd();
		}
		else
		{
			VIDEO_WaitVSync();
		}
	}
	// Reset elapsed time so game gets correct timing
	platformWii.elapsedTimeMs = 0;
}

void Platform_FrameStart()
{
	u64 now = gettime();
	/*
	// Whatever unit this is, convert to milliseconds
	float seconds = (float)now / (float)(TB_TIMER_CLOCK * 1000.0f);
	Platform_UpdateDeltaTime(seconds * 1000.0f);
	*/
	Platform_UpdateDeltaTime(ticks_to_millisecs(now));

}

void Platform_RenderStart()
{
	gdl::PrepDisplay();
}

void Platform_RenderEnd()
{
	glFlush();
	gdl::Display();
}

void Platform_FrameEnd()
{
	platformWii.elapsedFrames += 1;
}

void MainLoop()
{
	while(true)
	{
		Platform_FrameStart();
		Platform_ReadControllers();
		Audio_Update();
		CheckForQuit();
		Platform_RenderStart();
		frameCall();
		Platform_RenderEnd();
		Platform_FrameEnd();
	}
}

bool Platform_IsControllerConnected(int controllerIndex)
{
	if (controllerIndex >= 0 && controllerIndex < MGDL_MAX_CONTROLLERS)
	{
		return platformWii.controllers[controllerIndex].m_isConnected;
	}
	else
	{
		return false;
	}
}

void Platform_ReadControllers()
{
	// TODO This might have to be in a macro
	WPAD_ScanPads();  // Scan the Wiimotes
	for (int i = 0; i < MGDL_MAX_CONTROLLERS; i++)
	{
		WiiController* controller = &platformWii.controllers[i];
		WiiController_StartFrame(controller);

		WPADData *data1 = WPAD_Data(controller->m_channel);
		controller->m_isConnected = (data1->err == WPAD_ERR_NONE);

		// Read data anyway even when not connected

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

		controller->m_pressedButtons = WPAD_ButtonsDown(controller->m_channel);
		controller->m_releasedButtons = WPAD_ButtonsUp(controller->m_channel);
		controller->m_heldButtons = WPAD_ButtonsHeld(controller->m_channel);

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

void Platform_DoProgramExit()
{
	gdl::wii::DoProgramExit();
}

void Platform_ResizeWindow(int newWidth, int newHeight)
{

	int left = 0;
	int top = newHeight;
	int scaledWidth = newWidth;
	int scaledHeight = newHeight;

    // But keep showing the internal resolution scaled
    glViewport(left, top, scaledWidth, scaledHeight);
    platformWii.viewport.left = left;
    platformWii.viewport.top = top;
    platformWii.viewport.width = scaledWidth;
    platformWii.viewport.height = scaledHeight;

    // Update window size
    platformWii.windowWidth = newWidth;
    platformWii.windowHeight = newHeight;
}

void Platform_MapJoystickToController(int joystickIndex, int controllerIndex)
{
	// NOTE On wii this function has no meaning. Do some code to avoid unused paramter warning
	if (joystickIndex > 0 && controllerIndex > 0)
	{
		joystickIndex = controllerIndex;
	}
}

Platform* Platform_GetSingleton(void)
{
	return &platformWii;
}
