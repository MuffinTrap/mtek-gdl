#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC)
#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-openal.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-splash.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>

#include <mgdl/pc/mgdl-pc-input.h>
#include <mgdl/pc/mgdl-joystick.h>

#include <sndfile.h>

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

// For Audio

static void Platform_InitAudio();
// OpenAL sound
static ALCdevice* device;
static ALCcontext* context;

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

void Platform_UpdateControllers()
{
    WiiController* controller = Platform_GetController(0);

    if (WiiController_ButtonPress(controller, ButtonHome))
    {
        if (quitCall != NULL)
        {
            quitCall();
        }
        Platform_DoProgramExit();
    }
    // Reset controller for next frame
    WiiController_StartFrame(controller);
    Joystick* gamepad_0 = platformPC.gamepads[0];
    if (Joystick_IsConnected(gamepad_0))
    {
        Joystick_ReadInputs(gamepad_0);
        // Always read cursor from glut
        controller->m_cursorX = kbmController.m_cursorX;
        controller->m_cursorY = kbmController.m_cursorY;
    }
}

void RenderLoop()
{
	frameCall();

    Platform_RenderEnd();

    Platform_UpdateControllers();
}

void Platform_RenderEnd()
{
    // End drawing and process all commands
    // Wait for v sync and swap
    // glutSwapBuffers() will call glFlush();
    glutSwapBuffers();

}


void Platform_InitAudio()
{
    Log_Info("Setting up OpenAL Audio Device.\n");
    // Initialize OpenAL
    device = alcOpenDevice(NULL);
    if (!device) {
        Log_Error("Failed to open OpenAL device\n");
        return;
    }
    Log_Info("Setting up OpenAL Audio Contex.\n");
    context = alcCreateContext(device, NULL);
    if (alcGetError(device) != ALC_NO_ERROR || !context) {
        Log_Error("Failed to create OpenAL context\n");
        alcCloseDevice(device);
        return;
    }
    ALboolean contextMadeOK = alcMakeContextCurrent(context);
    if (contextMadeOK != AL_TRUE)
    {
        Log_Error("Failed to make OpenAL context current\n");
        alcCloseDevice(device);
        return;
    }

    Log_Info("OpenAL context created\n");
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

	Platform_InitAudio();

    // fake command line arguments
    int argumentCount = 0;
    char* argumentVector = nullptr;

    Log_Info("GlutInit\n");
    glutInit(&argumentCount, &argumentVector);
    Log_Info("glutInitDisplayMode\n");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutWindowId = glutCreateWindow(windowName);
    if ((initFlags & PlatformInitFlag::FlagFullScreen) != 0)
    {
        glutFullScreen();
    }


    glutReshapeFunc(Platform_ResizeWindow);

    InitPCInput();
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
    WiiController_Init(&kbmController, 0);

    WiiController_ZeroAllInputs(&kbmController);
    WiiController_StartFrame(&kbmController);

    // Init Joystick
    platformPC.gamepads[0] = Joystick_Create(0);
    if (Joystick_IsConnected(platformPC.gamepads[0]))
    {
        Joystick_ZeroInputs(platformPC.gamepads[0]);
    }
}

struct WiiController* Platform_GetController(int controllerNumber)
{
    Joystick* gamepad = platformPC.gamepads[controllerNumber];
    if (Joystick_IsConnected(gamepad) && gamepad->index == controllerNumber)
    {
        return Joystick_GetController(gamepad);
    }
    else if (controllerNumber == 0)
    {
        return &kbmController;
    }
    return &kbmController;
}

void Platform_DoProgramExit()
{
    Log_Info("DoProgramExit\n");
	// Close sound
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

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

