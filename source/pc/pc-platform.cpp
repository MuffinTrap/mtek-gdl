#include <mgdl/mgdl-opengl.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-splash.h>
#include <mgdl/pc/mgdl-pc-platform.h>
#include <mgdl/pc/mgdl-pc-input.h>

// Declarations

static int windowWidth = 0;
static int windowHeight = 0;

static int WII_WIDTH = 640;
static int WII_HEIGHT = 480;

static int glutElapsedStartMS;
static int glutElapsedMS;

static std::function<void()> initCall = nullptr;
static std::function<void()> updateCall = nullptr;
static std::function<void()> drawCall = nullptr;


// Main glut functions
static void UpdateDeltaMS();

static void UpdateLoop(int value);
static void RenderLoop();

// For measuring A hold and splash screen
static int waitElapsedMS;
static bool showHoldAMessage;
static float startUpDeltaTimeMs;
static float splashProgress;
// For holding until a is held for 1 second
static float aHoldTimer;
static void UpdateSplash(int value);
static void RenderSplash();
static void UpdateWaitDeltaMS();

static void UpdateAHold(int value);
static void RenderAHold();
static bool IncreaseAHoldAndTest();

// Used by all
static void UpdateEnd();
static void RenderEnd();


// Definitions


static void RenderSplash()
{
    splashProgress = gdl::DrawSplashScreen(startUpDeltaTimeMs, showHoldAMessage, aHoldTimer);
    RenderEnd();
}

static void RenderAHold()
{
    RenderEnd();
}

static void UpdateWaitDeltaMS()
{
    glutElapsedMS = glutGet(GLUT_ELAPSED_TIME);
    int deltaMS = glutElapsedMS - glutElapsedStartMS;
    startUpDeltaTimeMs = float(deltaMS) / 1000.0f;
}

static void UpdateDeltaMS()
{
    // Update delta time
    // The game will call GetDeltaTime() during updateCall
    // Remove waiting time from elapsed so that game starts from 0 elapsed
    glutElapsedMS = glutGet(GLUT_ELAPSED_TIME) - waitElapsedMS;
}

static bool IncreaseAHoldAndTest()
{
    if (glutController.ButtonHeld(gdl::WiiButtons::ButtonA))
    {
        aHoldTimer += startUpDeltaTimeMs;
        if (aHoldTimer >= 1.0f)
        {
            return true;
        }
    }
    else
    {
        aHoldTimer = 0.0f;
    }
    return false;
}



static void UpdateSplash(int value)
{
    UpdateWaitDeltaMS();
    bool waitIsOver = false;
    if (showHoldAMessage)
    {
        waitIsOver = IncreaseAHoldAndTest();
    }
    else
    {
        waitIsOver = (splashProgress > 1.0f);
    }

    if (waitIsOver)
    {
        // Change to main Update function and render
        glutTimerFunc(16, UpdateLoop, value);
        glutDisplayFunc(RenderLoop);
    }

    // Keep waiting
    glutTimerFunc(16, UpdateSplash, value);
    UpdateEnd();
}

static void UpdateAHold(int value)
{
    UpdateWaitDeltaMS();
    if (IncreaseAHoldAndTest())
    {
        // Change to main update and render
        glutDisplayFunc(RenderLoop);
        glutTimerFunc(16, UpdateLoop, value);
    }
    // Keep waiting
    glutTimerFunc(16, UpdateAHold, value);
    UpdateEnd();
}

static void UpdateLoop(int value)
{
    UpdateDeltaMS();

	updateCall();

    glutTimerFunc(16, UpdateLoop, value);
    UpdateEnd();
}
static void UpdateEnd()
{
    // TODO Where is the correct place for this?
    // Need to have up to date information for the game to read
    glutController.StartFrame();

    glutElapsedStartMS = glutElapsedMS;
    // Tell glut that the window needs to be
    // redrawn.
    glutPostRedisplay();
}

void RenderLoop()
{
	drawCall();
    RenderEnd();
}

static void RenderEnd()
{
    // End drawing and process all commands
    glFlush();

    // Wait for v sync and swap
    glutSwapBuffers();
}

void onWindowSizeChange(int newWidth, int newHeight)
{
    // Keep aspect ratio
    float width = (float)newWidth/(float)windowWidth;
    float height = (float)newHeight/(float)windowHeight;
    float scale = width < height ? width : height;

    // Use the scale to center the graphics
    float scaledWidth = scale*(float)windowWidth;
    float scaledHeight = scale*(float)windowHeight;

    int left = 0;
    int top = 0;

    if (scaledWidth < newWidth)
    {
        // Black bars on sides
        left = (newWidth - scaledWidth)/2;
    }
    if (scaledHeight < newHeight)
    {
        // Black bars on top and bottom
        top = (newHeight - scaledHeight)/2;
    }

    // But keep showing the internal resolution scaled
    glViewport(left, top, scaledWidth, scaledHeight);
}

void gdl::PlatformPC::InitAudio()
{
    printf("Setting up OpenAL Audio Device.\n");
    // Initialize OpenAL
    device = alcOpenDevice(NULL);
    if (!device) {
        printf("Failed to open OpenAL device\n");
        return;
    }
    printf("Setting up OpenAL Audio Contex.\n");
    context = alcCreateContext(device, NULL);
    if (alcGetError(device) != ALC_NO_ERROR || !context) {
        printf("Failed to create OpenAL context\n");
        alcCloseDevice(device);
        return;
    }
    ALboolean contextMadeOK = alcMakeContextCurrent(context);
    if (contextMadeOK != AL_TRUE)
    {
        printf("Failed to make OpenAL context current\n");
        alcCloseDevice(device);
        return;
    }

    printf("OpenAL context created\n");
}

void gdl::PlatformPC::InitSystem(const char* name, gdl::ScreenAspect screenAspect, std::function<void ()> initCallback, std::function<void ()> updateCallback, std::function<void ()> drawCallback, u32 initFlags)
{
    this->name = name;
	gdl_assert_print(initCallback != nullptr, "Need to provide init callback before system init on PC");
	gdl_assert_print(drawCallback != nullptr, "Need to provide update callback before system init on PC");
	gdl_assert_print(updateCallback != nullptr, "Need to provide draw callback before system init on PC");
    initCall = initCallback;
	drawCall = drawCallback;
	updateCall = updateCallback;


    screenWidth = WII_WIDTH;
    screenHeight = WII_HEIGHT;
    switch(screenAspect)
    {
        case ScreenAuto:
            windowWidth = 854;
            windowHeight = 480;
            aspectRatio = 16.0f/9.0f;
            break;
        case Screen4x3:
            windowWidth = 640;
            windowHeight = 480;
            aspectRatio = 4.0f/3.0f;
            break;
        case Screen16x9:
            // Wii only outputs 640x480, but in this format it is shown wider
            windowWidth = 854;
            windowHeight = 480;
            aspectRatio = 16.0f/9.0f;
            break;
    };

    // Store these for the reshape callback

	InitAudio();

    // fake command line arguments
    int argumentCount = 0;
    char* argumentVector = nullptr;

    printf("GlutInit\n");
    glutInit(&argumentCount, &argumentVector);
    printf("glutInitDisplayMode\n");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Press 2 to write Rocket tracks");
    if ((initFlags & gdl::PlatformInitFlag::FlagFullScreen) != 0)
    {
        glutFullScreen();
    }

    // Input callbacks and init
    glutKeyboardFunc(keyboardDown); // Register the keyboard callback
    glutKeyboardUpFunc(keyboardUp); // Register the keyboard release

    glutSpecialFunc(specialKeyDown); // Register the keyboard callback
    glutSpecialUpFunc(specialKeyUp); // Register the keyboard release

    glutMouseFunc(mouseKey);        // Register mouse buttons and movement
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);

    glutReshapeFunc(onWindowSizeChange);

	const bool SplashFlag = (initFlags & gdl::PlatformInitFlag::FlagSplashScreen)!= 0;
	const bool HoldAFlag = (initFlags & gdl::PlatformInitFlag::FlagPauseUntilA)!= 0;
    // Set up A hold variables
    if (HoldAFlag||SplashFlag)
    {
        waitElapsedMS = 0;
        aHoldTimer = 0.0f;
        showHoldAMessage = HoldAFlag;
    }

    // Select display function
    if (SplashFlag)
    {
        splashProgress = 0.0f;
        glutDisplayFunc(RenderSplash);
    }
    else if (HoldAFlag)
    {
        glutDisplayFunc(RenderAHold);
    }
    else
    {
        glutDisplayFunc(RenderLoop);
    }

    glutController.ZeroAllInputs();
    glutController.StartFrame();
    initCall();
    glutElapsedStartMS = 0;

    // Select update function
    if (SplashFlag)
    {
        glutTimerFunc(16, UpdateSplash, 0);
    }
    else if (HoldAFlag)
    {
        glutTimerFunc(16, UpdateAHold, 0);
    }
    else
    {
        glutTimerFunc(16, UpdateLoop, 0);
    }
	glutMainLoop();
}

gdl::WiiController& gdl::PlatformPC::GetController(int controllerNumber)
{
    if (controllerNumber == 0)
    {
        return glutController;
    }
    return glutController;
}

void gdl::PlatformPC::DoProgramExit()
{
    printf("DoProgramExit\n");
	// Close sound
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
    alcDestroy();

	exit(0);
}

float gdl::PlatformPC::GetDeltaTime()
{
    int deltaMS = glutElapsedMS - glutElapsedStartMS;
    deltaTimeS= float(deltaMS) / 1000.0f;
    return deltaTimeS;
}

float gdl::PlatformPC::GetElapsedSeconds()
{
    return float(glutElapsedMS)/1000.0f;
}


