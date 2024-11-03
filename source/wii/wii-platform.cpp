#include <ogc/lwp_watchdog.h>
#include <wiiuse/wpad.h>

#include <mgdl/mgdl-opengl.h>
#include <mgdl/wii/mgdl-wii-platform.h>
#include <mgdl/wii/mgdl-wii.h>

static std::function<void()> initCall = nullptr;
static std::function<void()> updateCall = nullptr;
static std::function<void()> drawCall = nullptr;

static gdl::WiiController controller;

void gdl::PlatformWii::InitSystem(gdl::ScreenAspect screenAspect,
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

	// Convert to Wii InitAspectmode for now
	gdl::InitAspectMode mode = gdl::InitAspectMode::AspectAuto;
	switch(screenAspect)
	{
		case ScreenAuto:
			mode = gdl::InitAspectMode::AspectAuto;
			break;
		case Screen4x3:
			mode = gdl::InitAspectMode::Aspect4x3;
			aspectRatio = 4.0f/3.0f;
			break;
		case Screen16x9:
			mode = gdl::InitAspectMode::Aspect16x9;
			aspectRatio = 16.0f/9.0f;
			break;
	};
	fatInitDefault();
	gdl::InitSystem(gdl::ModeAuto, mode, gdl::HiRes, gdl::InitFlags::OpenGX);
	screenWidth = gdl::ScreenXres;
	screenHeight = gdl::ScreenYres;

	ogx_initialize();
	// Init controller
	// TODO Add init parameters for what controllers to init?
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

	controller.SetChannelNumber(WPAD_CHAN_0);
	controller.ZeroAllInputs();

	gdl::ConsoleMode();
	u64 now = gettime();
	u64 deltaTimeStart = now;

	printf("Got resolution: %d x %d\n", screenWidth, screenHeight);

	initCall();

    if ((initFlags & gdl::PlatformInitFlag::FlagPauseUntilA )!= 0)
	{
		while(true)
		{
			ReadControllers();
			if (GetController(0).ButtonPress(gdl::WiiButtons::ButtonA))
			{
				break;
			}
			VIDEO_WaitVSync();
		}
	}


	while(true)
	{
		// Timing
		u64 now = gettime();
		deltaTimeS = (float)(now - deltaTimeStart) / (float)(TB_TIMER_CLOCK * 1000); // division is to convert from ticks to seconds
		deltaTimeStart = now;
		elapsedTimeS += deltaTimeS;

		controller.StartFrame();
		ReadControllers();
		updateCall();
		gdl::PrepDisplay();
		drawCall();
		glFlush();
		gdl::Display();
	}
}
gdl::WiiController& gdl::PlatformWii::GetController(int controllerNumber)
{
	if (controllerNumber == 0)
	{
		return controller;
	}
	return controller;
}

void gdl::PlatformWii::ReadControllers()
{
	// TODO This might have to be in a macro
	WPAD_ScanPads();  // Scan the Wiimotes
	WPADData *data1 = WPAD_Data(controller.channel);

	const ir_t &ir = data1->ir;
	// Multiply x and y to match them to 16:9 screen
	controller.cursorX = ir.x * 1.67f - 16.f;
	controller.cursorY = ir.y * 1.2f - 16.f;

	controller.pressedButtons = WPAD_ButtonsDown(0);
	controller.releasedButtons = WPAD_ButtonsUp(0);
	controller.heldButtons = WPAD_ButtonsHeld(0);

	controller.nunchukJoystickDirectionX=0.0f;
	controller.nunchukJoystickDirectionY=0.0f;
	const expansion_t &ex = data1->exp;
	if (ex.type == WPAD_EXP_NUNCHUK)
	{
		joystick_t n = ex.nunchuk.js;
		if (n.mag > controller.nunchukJoystickDeadzone)
		{
			// Angle is reported in degrees
			// Angle of 0 means up.
			// 90 right, 180 down, 270 left

			float rad = DegToRad(n.ang);
			float x = 0;
			float y = -1.0f;
			float dirx = cos(rad) * x - sin(rad) * y;
			float diry = sin(rad) * x + cos(rad) * y;
			controller.nunchukJoystickDirectionX = dirx * n.mag;
			controller.nunchukJoystickDirectionY = diry * n.mag;
		}
	}

	controller.roll = DegToRad(data1->orient.roll);
}

float gdl::PlatformWii::GetDeltaTime() { return deltaTimeS; }
float gdl::PlatformWii::GetElapsedSeconds() { return elapsedTimeS; }

void gdl::PlatformWii::DoProgramExit()
{
	gdl::wii::DoProgramExit();
}
