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

static Platform platformGlut;
// Glut specific platform variables
static int glutWindowId = 0;

static CallbackFunction initCall = nullptr;
static CallbackFunction frameCall = nullptr;
static CallbackFunction quitCall = nullptr;

static void CheckForQuit()
{
    WiiController* firstController = Platform_GetController(0);
    // Test for ESC unless game handles it
	if (Flag_IsSet(platformGlut.initFlags, FlagGameHandlesHOME) == false)
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

// Glut callbacks

void OnGlutDisplay()
{
    Audio_Update();

	frameCall();

    Platform_RenderEnd();
}

static void OnGlutTimer(int value)
{
    // Calculate how much time rendering took
    Platform_ReadControllers();
    CheckForQuit();
    Platform_FrameStart();
    glutTimerFunc(16, OnGlutTimer, value);
    Platform_FrameEnd();
}

static void OnGlutReshape(GLint width, GLint height)
{
    Platform_ResizeWindow(width, height);
}

// For measuring A hold and splash screen

// Definitions

static void OnGlutRenderSplash()
{
    Platform_RenderSplash();
}

static void OnGlutRenderAHold(void)
{
    Platform_RenderAHold();
}


// Needs the int parameter to work with glutTimerFunc
void OnGlutTimer_UpdateSplash(int value)
{
    // Calculate how much time rendering took
    Platform_FrameStart();
    Platform_ReadControllers();
    CheckForQuit();

    bool waitIsOver = false;
    if (platformGlut.showHoldAMessage)
    {
        waitIsOver = Platform_IncreaseAHoldAndTest();
    }
    else
    {
        waitIsOver = (platformGlut.splashProgress > 1.0f);
    }

    if (waitIsOver)
    {
        // Record waiting time
        platformGlut.waitElapsedMS = platformGlut.elapsedTimeMs;
        // Change to main Update function and render
        glutTimerFunc(16, OnGlutTimer, value);
        glutDisplayFunc(OnGlutDisplay);
    }
    else
    {
        // Keep waiting
        glutTimerFunc(16, OnGlutTimer_UpdateSplash, value);
    }
    Platform_FrameEnd();
}

void OnGlutTimer_UpdateAHold(int value)
{
    // Calculate how much time rendering took
    Platform_FrameStart();
    Platform_ReadControllers();
    CheckForQuit();

    if (Platform_IncreaseAHoldAndTest())
    {
        // Record waiting time
        platformGlut.waitElapsedMS = platformGlut.elapsedTimeMs;
        // Change to main update and render
        glutDisplayFunc(OnGlutDisplay);
        glutTimerFunc(16, OnGlutTimer, value);
    }
    else
    {
        // Keep waiting
        glutTimerFunc(16, OnGlutTimer_UpdateAHold, value);
    }
    Platform_FrameEnd();
}


// Platform functions

static void InitWindow(const char* windowName,
                   ScreenAspect screenAspect)
{
    // Store these for the reshape callback
    Platform_SetWindowNameAndAspect(windowName, screenAspect);

    Log_Info("GlutInit\n");
    // fake command line arguments
    int argumentCount = 0;
    char* argumentVector = nullptr;
    glutInit(&argumentCount, &argumentVector);

    Log_Info("glutInitDisplayMode\n");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(MGDL_WII_WIDTH, MGDL_WII_HEIGHT);
    glutWindowId = glutCreateWindow(windowName);

    glutReshapeFunc(OnGlutReshape);
    Platform_InitControllers();
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

    platformGlut.initFlags = initFlags;

    InitWindow(windowName, screenAspect);
    if ((initFlags & PlatformInitFlag::FlagFullScreen) != 0)
    {
        glutFullScreen();
    }

	Platform_InitAudio();

    initCall();

    const bool SplashFlag = Flag_IsSet(initFlags, PlatformInitFlag::FlagSplashScreen);
    const bool HoldAFlag = Flag_IsSet(initFlags, PlatformInitFlag::FlagPauseUntilA);
    // Set up A hold variables
    Platform_ResetTime();

    if (HoldAFlag||SplashFlag)
    {
        platformGlut.showHoldAMessage = HoldAFlag;
    }

    // Select display and update functions
    if (SplashFlag)
    {
        glutDisplayFunc(OnGlutRenderSplash);
        glutTimerFunc(16, OnGlutTimer_UpdateSplash, 0);
    }
    else if (HoldAFlag)
    {
        glutDisplayFunc(OnGlutRenderAHold);
        printf("\n>> MGDL INIT COMPLETE\n");
        printf(">> Hold A button to continue\n");
        glutTimerFunc(16, OnGlutTimer_UpdateAHold, 0);
    }
    else
    {
        glutDisplayFunc(OnGlutDisplay);
        glutTimerFunc(16, OnGlutTimer, 0);
    }

	glutMainLoop();
}

void Platform_InitAudio(void)
{
    Audio_Init(nullptr);
}

void Platform_SetFullscreen(bool enabled)
{
    if (enabled)
    {
        glutFullScreen();
    }
    else
    {
        // This will disable fullscreen and call the reshape callback
        glutReshapeWindow(platformGlut.windowWidth, platformGlut.windowHeight);
    }
}

void Platform_FrameStart()
{
    Platform_UpdateDeltaTime(glutGet(GLUT_ELAPSED_TIME));
}

void Platform_FrameEnd()
{
    platformGlut.elapsedFrames += 1;
    // Tell glut that the window needs to be
    // redrawn.
    glutPostRedisplay();
}

void Platform_RenderEnd()
{
    Platform_StartNextFrameControllers();
    // End drawing and process all commands
    // Wait for v sync and swap
    // glutSwapBuffers() will call glFlush();
    glutSwapBuffers();
}

void Platform_DoProgramExit()
{
	// Close sound
    Audio_Deinit();

    // Close window
    glutDestroyWindow(glutWindowId);

	exit(0);
}

Platform* Platform_GetSingleton(void)
{
    return &platformGlut;
}
#endif

