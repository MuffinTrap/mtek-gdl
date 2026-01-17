#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_MSYS2)
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-splash.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/pc/mgdl-joystick.h>
#include <stdio.h>

static Platform platformPC;
// Declarations

static int glutElapsedStartMS;
static int glutElapsedMS;
static int glutDeltaMS;

static int glutWindowId = 0;

static CallbackFunction initCall = nullptr;
static CallbackFunction frameCall = nullptr;
static CallbackFunction quitCall = nullptr;


// Main glut functions
static void UpdateDeltaTime();

static void UpdateLoop(int value);
static void RenderLoop();

// For measuring A hold and splash screen

// Used by all
static void UpdateEnd();


// Definitions

static void UpdateDeltaTime()
{
    // Update elapsed time
    glutElapsedMS = glutGet(GLUT_ELAPSED_TIME);

    // Remove waiting time from elapsed so that game starts from 0 elapsed
    // TODO Bugged if splascreen is used
    // glutElapsedMS = glutElapsedMS - waitElapsedMS;

    // Calculate elapsed time
    platformPC.elapsedTimeS = float(glutElapsedMS)/1000.0f;

    // Calculate delta time
    glutDeltaMS = glutElapsedMS - glutElapsedStartMS;
    platformPC.deltaTimeS = float(glutDeltaMS) / 1000.0f;

    Platform_ReadControllers();
}


static void RenderSplash()
{
    Platform_RenderSplash(&platformPC);
}

// Needs the int parameter to work with glutTimerFunc
void Platform_UpdateSplash(int value)
{
    // Calculate how much time rendering took
    UpdateDeltaTime();
    bool waitIsOver = false;
    if (platformPC.showHoldAMessage)
    {
        waitIsOver = Platform_IncreaseAHoldAndTest(&platformPC);
    }
    else
    {
        waitIsOver = (platformPC.splashProgress > 1.0f);
    }

    if (waitIsOver)
    {
        // Record waiting time
        platformPC.waitElapsedMS = glutElapsedMS;
        // Change to main Update function and render
        glutTimerFunc(16, UpdateLoop, value);
        glutDisplayFunc(RenderLoop);
    }
    else
    {
        // Keep waiting
        glutTimerFunc(16, Platform_UpdateSplash, value);
    }
    UpdateEnd();
}

void Platform_UpdateAHold(int value)
{
    // Calculate how much time rendering took
    UpdateDeltaTime();
    if (Platform_IncreaseAHoldAndTest(&platformPC))
    {
        // Record waiting time
        platformPC.waitElapsedMS = glutElapsedMS;
        // Change to main update and render
        glutDisplayFunc(RenderLoop);
        glutTimerFunc(16, UpdateLoop, value);
    }
    else
    {
        // Keep waiting
        glutTimerFunc(16, Platform_UpdateAHold, value);
    }
    UpdateEnd();
}

static void UpdateLoop(int value)
{
    // Calculate how much time rendering took
    UpdateDeltaTime();
    glutTimerFunc(16, UpdateLoop, value);
    UpdateEnd();
}

static void UpdateEnd()
{
    platformPC.elapsedUpdates += 1;
    // Tell glut that the window needs to be
    // redrawn.
    glutPostRedisplay();
    // start new frame
    glutElapsedStartMS = glutElapsedMS;
}

void Platform_ReadControllers()
{
    WiiController* firstController = Platform_GetController(0);

    // Read mouse and keyboard
    WiiController_ReplaceWith(firstController, &kbmController);

    // Test for ESC
    if (WiiController_ButtonPress(firstController, ButtonHome))
    {
        if (quitCall != NULL)
        {
            quitCall();
        }
        Platform_DoProgramExit();
    }

    // Update state of all joysticks
    Joystick_ReadInputs();
    // If first joystick is connected, add it 
    // to controller 0
    if (Joystick_IsConnected(0))
    {
        Joystick_AddToController(firstController, 0);
    }

    // other controllers read from joysticks 1-3
    for (int i = 1; i < 4; i++)
    {
        if (Joystick_IsConnected(i))
        {
            Joystick_ReplaceController(Platform_GetController(i), i);
        }
    }
}

void Platform_StartNextFrameControllers()
{ 
	// Start recording input for next update
	WiiController_StartFrame(&kbmController);
    Joystick_StartFrame();

	for (int i = 0; i < 4; i++)
	{
		WiiController* controller = Platform_GetController(i);
		// Reset controller for next frame
		WiiController_StartFrame(controller);
	}
}

void RenderLoop()
{
    Audio_Update();

	frameCall();

    Platform_RenderEnd();
}

void Platform_RenderEnd()
{
    Platform_StartNextFrameControllers();
    // End drawing and process all commands
    // Wait for v sync and swap
    // glutSwapBuffers() will call glFlush();
    glutSwapBuffers();
}


void Platform_Init(const char* windowName,
                   ScreenAspect screenAspect,
                   CallbackFunction initCallback,
                   CallbackFunction frameCallback,
                   CallbackFunction quitCallback,
                   u32 initFlags)
{
	mgdl_assert_print(initCallback != nullptr, "Need to provide init callback before system init on PC");
	mgdl_assert_print(frameCallback != nullptr, "Need to provide frame callback before system init on PC");
    initCall = initCallback;
	frameCall = frameCallback;
    quitCall = quitCallback;



    // Store these for the reshape callback
    Platform_SetWindowNameAndSize(&platformPC, windowName, screenAspect);

	Audio_Init(NULL);

    // fake command line arguments
    int argumentCount = 0;
    char* argumentVector = nullptr;

    Log_Info("GlutInit\n");
    glutInit(&argumentCount, &argumentVector);
    Log_Info("glutInitDisplayMode\n");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(MGDL_WII_WIDTH, MGDL_WII_HEIGHT);
    glutWindowId = glutCreateWindow(windowName);
    if ((initFlags & PlatformInitFlag::FlagFullScreen) != 0)
    {
        glutFullScreen();
    }


    glutReshapeFunc(Platform_ResizeWindow);

    Platform_InitControllers();

    initCall();
    glutElapsedStartMS = 0;
    platformPC.elapsedUpdates = 0;


    const bool SplashFlag = Flag_IsSet(initFlags, PlatformInitFlag::FlagSplashScreen);
    const bool HoldAFlag = Flag_IsSet(initFlags, PlatformInitFlag::FlagPauseUntilA);
    // Set up A hold variables
    Platform_ResetTime(&platformPC);

    if (HoldAFlag||SplashFlag)
    {
        platformPC.showHoldAMessage = HoldAFlag;
    }

    // Select display and update functions
    if (SplashFlag)
    {
        glutDisplayFunc(RenderSplash);
        glutTimerFunc(16, Platform_UpdateSplash, 0);
    }
    else if (HoldAFlag)
    {
        glutDisplayFunc(Platform_RenderAHold);
        printf("\n>> MGDL INIT COMPLETE\n");
        printf(">> Hold A button to continue\n");
        glutTimerFunc(16, Platform_UpdateAHold, 0);
    }
    else
    {
        glutDisplayFunc(RenderLoop);
        glutTimerFunc(16, UpdateLoop, 0);
    }

	glutMainLoop();
}

void Platform_InitControllers()
{
    Log_Info("Platform Init Controllers\n");
	InitPCInput();
	Joystick_Init();
	Joystick_ZeroInputs();

	for (int i = 0; i < 4; i++)
	{
		WiiController* c = &platformPC.controllers[i];
		WiiController_Init(c, i);
		WiiController_ZeroAllInputs(c);
		WiiController_StartFrame(c);
	}

	WiiController_Init(&kbmController, 0);
	WiiController_ZeroAllInputs(&kbmController);
	WiiController_StartFrame(&kbmController);
}

struct WiiController* Platform_GetController(int controllerNumber)
{
    return &platformPC.controllers[controllerNumber];
}

void Platform_DoProgramExit()
{
    Log_Info("DoProgramExit\n");
	// Close sound
    Audio_Deinit();

    // Close window
    glutDestroyWindow(glutWindowId);

	exit(0);
}

struct Platform* Platform_GetSingleton() { return &platformPC; }

float Platform_GetDeltaTime()
{
    return platformPC.deltaTimeS;
}

float Platform_GetElapsedSeconds()
{
    return platformPC.elapsedTimeS;
}

u32 Platform_GetElapsedUpdates()
{
    return platformPC.elapsedUpdates;
}
#endif

