#ifdef MGDL_PLATFORM_WINDOWS

#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/mgdl-util.h>
#include <mgdl/mgdl-audio.h>
#include <mgdl/pc/mgdl-joystick.h>
#include <mgdl/pc/mgdl-pc-input.h>

#include <Windows.h>
#include <windowsx.h> // Macro for extracting mouse position

static Platform platformPC;
static USHORT keybuffer; // Ascii input for later textinput

static CallbackFunction initCall = nullptr;
static CallbackFunction frameCall = nullptr;
static CallbackFunction quitCall = nullptr;

// Windows context
static HINSTANCE instanceHandle;
static HWND windowHandle;

// OpenGL context
static HDC deviceContextHandle = NULL;
static HGLRC openGLContext = NULL;
static PIXELFORMATDESCRIPTOR pixelFormatDesc;
static int pixelFormatEnum;

// Timer
static UINT_PTR m_renderTimerId;
static UINT_PTR m_splashTimerId;
static BOOL windowIsVisible;
static DWORD applicationStartTimeMS;
static DWORD applicationElapsedMS;

// Controller(s)
static void ErrorExit()
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL) == 0) {
		MessageBox(NULL, TEXT("FormatMessage failed"), TEXT("Error"), MB_OK);
		ExitProcess(dw);
	}

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	ExitProcess(dw);
}

/*
	STATIC UPDATE AND RENDER
*/
static void UpdateDeltaTime(DWORD newElapsedMs)
{
	// If system elapsed is 100
	// new elapsed is 116
	// application elapsed is 0
	DWORD lastElapsed = applicationElapsedMS;
	applicationElapsedMS = newElapsedMs - applicationStartTimeMS;
	DWORD elapsedMS = applicationElapsedMS - lastElapsed;

	platformPC.elapsedTimeS = (float)applicationElapsedMS/ 1000.0f;
	platformPC.deltaTimeS = (float(elapsedMS) / 1000.0f);
}

static void RenderAHold(
	HWND target_window,
	UINT message,
	UINT_PTR timer_id,
	DWORD systemElapsedMs)
{ 
	UpdateDeltaTime(systemElapsedMs);
	Platform_UpdateControllers();
	Platform_UpdateAHold(0);
	Platform_RenderAHold();
}

void Platform_InitAudio()
{
	Audio_Init(&windowHandle);
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

static void RenderLoop(
	HWND target_window,
	UINT message,
	UINT_PTR timer_id,
	DWORD systemElapsedMs)
{
	UpdateDeltaTime(systemElapsedMs);
	Audio_Update();

	frameCall();

    Platform_RenderEnd();

	Platform_UpdateControllers();
}

void UpdateEnd()
{
	platformPC.elapsedUpdates += 1;
}

void Platform_RenderEnd()
{
	SwapBuffers(deviceContextHandle);
}


void Platform_UpdateSplash(int value)
{
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
        platformPC.waitElapsedMS = applicationElapsedMS;
        // Change to main Update function and render
		KillTimer(windowHandle, m_splashTimerId);
		m_renderTimerId = SetTimer(windowHandle, NULL, 16, RenderLoop);
    }
    UpdateEnd();
}

void Platform_UpdateAHold(int value)
{
    if (Platform_IncreaseAHoldAndTest(&platformPC))
    {
        // Record waiting time
        platformPC.waitElapsedMS = applicationElapsedMS;
        // Change to main update and render
		KillTimer(windowHandle, m_splashTimerId);
		m_renderTimerId = SetTimer(windowHandle, NULL, 16, RenderLoop);
    }
    UpdateEnd();
}

static void RenderSplash(
	HWND target_window,
	UINT message,
	UINT_PTR timer_id,
	DWORD systemElapsedMs
)
{
	UpdateDeltaTime(systemElapsedMs);
	Platform_UpdateControllers();
	Platform_UpdateSplash(0);
	Platform_RenderSplash(&platformPC);
}


void SetupOpenGL(HWND target_windowHandle)
{
	deviceContextHandle = GetDC(target_windowHandle);
	if (deviceContextHandle == NULL)
	{
		OutputDebugStringA("Could not get device context\n");
		Log_Error("Could not get device context handle\n");
		ErrorExit();
	}
	// Set up requested pizel format
	pixelFormatDesc = {
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd  
		1,                     // version number  
		PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // 24-bit color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // no alpha buffer  
		0,                     // shift bit ignored  
		0,                     // no accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,                    // 32-bit z-buffer  
		0,                     // no stencil buffer  
		0,                     // no auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	};
	// Try set up OpenGL context
	pixelFormatEnum = ChoosePixelFormat(deviceContextHandle, &pixelFormatDesc);
	if (pixelFormatEnum == 0)
	{
		OutputDebugStringA("Failed to Choose pixel format\n");
		ErrorExit();
	}
	BOOL formatSet = SetPixelFormat(deviceContextHandle, pixelFormatEnum, &pixelFormatDesc);
	if (formatSet == FALSE)
	{
		OutputDebugStringA("Failed to set pixel format\n");
		ErrorExit();
	}

	openGLContext = wglCreateContext(deviceContextHandle);
	if (openGLContext == NULL)
	{
		OutputDebugStringA("Failed to get openGL context\n");
		ErrorExit();
	}
	BOOL currentSet = wglMakeCurrent(deviceContextHandle, openGLContext);
	if (currentSet == FALSE)
	{
		OutputDebugStringA("Failed to set openGL context\n");
		ErrorExit();
	}
	OutputDebugStringA("Created OpenGL context\n");
	Log_InfoF("OpenGL %s %s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
}

// This the function that windows will call when something happens
// When user clicks on X button, WM_CLOSE is sent
// When WM_CLOSE is received, release resources and destroy window
// When DestroyWindow is called, WM_DESTROY is sent
// When WM_DESTROY is received, call PostQuitMessage(0) which sends WM_QUIT
// When WM_QUIT is received, break out of the event loop and return to OS

// BIG DANGER: CreateWindowExA sends WM_CREATE and WM_NCCREATE
// but our WindowHandle is not set yet.
// Always use the parameter window handle inside this function!
LRESULT CALLBACK WindowCallback(HWND target_windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_CREATE:
	{
		Log_Info("WM_CREATE\n");
		// Windows has created the window and we can OpenGL
		SetupOpenGL(target_windowHandle);
	}
	break;
	case WM_ACTIVATEAPP:
	{
		// If the window is visible and active : wParam : BOOL
		Log_Info("WM_ACTIVATEAPP\n");
		windowIsVisible = (BOOL)wParam;
	}
	break;
	case WM_SIZE:
	{
		// Size is changed
		Log_Info("WM_SIZE\n");
	}
	break;
	case WM_TIMER:
	{
		Log_Info("WM_TIMER\n");
		// Windows calls this when timer is up
		// TODO Use high resolution timer
	}
	break;

	// Input from keyboard
	case WM_KEYDOWN:
	{
		UINT keycode = wParam;
		bool isRepeat = (lParam & (1 << 30)) > 0;
		if (isRepeat == false)
		{
			keyboardDown(keycode);
		}
		Log_InfoF("Key down %x\n", keycode);
	}
	break;
	case WM_KEYUP:
	{
		UINT keycode = wParam;
		keyboardUp(keycode);
		Log_InfoF("Key up %x\n", keycode);
	}
	break;

	// Input from mouse
	case WM_LBUTTONDOWN:

		OutputDebugStringA("Mouse left down");
		mouseDown(MGDL_VK_MOUSE_LEFT);
		break;
	case WM_RBUTTONDOWN:
		mouseDown(MGDL_VK_MOUSE_RIGHT);
		break;
	case WM_LBUTTONUP:
		OutputDebugStringA("Mouse left up");
		mouseUp(MGDL_VK_MOUSE_LEFT);
		break;
	case WM_RBUTTONUP:
		mouseUp(MGDL_VK_MOUSE_RIGHT);
		break;

	case WM_MOUSEMOVE:
	{
		SHORT xPos = GET_X_LPARAM(lParam);
		SHORT yPos = GET_Y_LPARAM(lParam);

		// Y coordinate starts from bottom
		// TODO calculate to Wii resolution
		mouseMove(xPos, platformPC.windowHeight - yPos);

		// Log_InfoF("mouse move %d,%d\n", xPos, yPos );
	}
	break;

	case WM_ERASEBKGND:
	{
		// Windows wants to clear the window
		// lie to it 
		result = TRUE;
	}
	break;
	case WM_CLOSE:
	{
		Log_Info("WM_CLOSE\n");
		KillTimer(windowHandle, m_renderTimerId); // Stop calling the render loop
		Platform_DoProgramExit();
	}
	break;
	case WM_DESTROY:
	{
		Log_Info("WM_DESTROY\n");
		// The window has been destroyed, bye bye
		PostQuitMessage(0); // Will send WM_QUIT message
	}
	break;
	default:
	{
		// Call the default
		result = DefWindowProcA(target_windowHandle, message, wParam, lParam);
	}
	break;
	}
	return(result);

}

void Platform_Init(const char* windowName,
	ScreenAspect screenAspect,
	CallbackFunction initCallback,
	CallbackFunction frameCallback,
	CallbackFunction quitCallback,
	u32 initFlags)
{
	mgdl_assert_print(initCallback != nullptr, "Need to provide init callback to platform")
	mgdl_assert_print(frameCallback != nullptr, "Need to provide frame callback to platform")

	initCall = initCallback;
	frameCall = frameCallback;
	quitCall = quitCallback;

	Platform_SetWindowNameAndSize(&platformPC, windowName, screenAspect);

	instanceHandle = GetModuleHandleA(NULL);

	WNDCLASSA WindowClass = {0};
	// Create window
	WindowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	WindowClass.lpfnWndProc = WindowCallback;
	WindowClass.hInstance = instanceHandle;
	WindowClass.lpszClassName = "MGDL Window Class";

	OutputDebugStringA("Register Class\n");
	if (RegisterClassA(&WindowClass) == FALSE)
	{
		OutputDebugStringA("Failed to register class\n");
		Log_Error("Could not register window class\n");
		ErrorExit();
		return;
	}

	OutputDebugStringA("Create Window\n");
	windowHandle = CreateWindowExA(
		0, // ext style
		WindowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // needed for OpenGL
		CW_USEDEFAULT, CW_USEDEFAULT, // X and Y
		MGDL_WII_WIDTH,
		MGDL_WII_HEIGHT,
		0, // Top level window, no parent
		0, // No menu
		instanceHandle,
		NULL // User data pointer
	);

	if (windowHandle == NULL)
	{
		OutputDebugStringA("Failed Create Window\n");
		Log_Error("Could not create window\n");
		ErrorExit();
		return;
	}

	// Change the window size so that the Client
	// area matches the WII resolution
	RECT wrect;
	GetWindowRect(windowHandle, &wrect);
	RECT crect;
	GetClientRect(windowHandle, &crect);
	POINT lefttop = { crect.left, crect.top }; // Practically both are 0
	ClientToScreen(windowHandle, &lefttop);
	POINT rightbottom = { crect.right, crect.bottom };
	ClientToScreen(windowHandle, &rightbottom);

	int left_border = lefttop.x - wrect.left; // Windows 10: includes transparent part
	int right_border = wrect.right - rightbottom.x; // As above
	int bottom_border = wrect.bottom - rightbottom.y; // As above
	int top_border_with_title_bar = lefttop.y - wrect.top; // There is no transparent par
	MoveWindow(windowHandle, wrect.left, wrect.top, MGDL_WII_WIDTH + left_border + right_border, MGDL_WII_HEIGHT + top_border_with_title_bar + bottom_border, TRUE);
	
	// Set up controllers
	Platform_InitControllers();
	Platform_InitAudio();
	// Hide cursor
	ShowCursor(FALSE);


	// Set up 60fps timer.
	// Setting NULL as lpTimerFunc makes windows send WM_TIMER
	// messages
	//SetUserObjectInformationW(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, FALSE, 1);

	// Main loop gets messages from Windows
	OutputDebugStringA("Start Message Loop\n");

	// Call the game/demo init
	initCall();
	// Setup timing
	Platform_ResetTime(&platformPC);

	ShowWindow(windowHandle, SW_SHOW);
	UpdateWindow(windowHandle);

	// Set up splash screen
 const bool SplashFlag = Flag_IsSet(initFlags, PlatformInitFlag::FlagSplashScreen);
    const bool HoldAFlag = Flag_IsSet(initFlags, PlatformInitFlag::FlagPauseUntilA);
    // Set up A hold variables
    Platform_ResetTime(&platformPC);

    if (HoldAFlag||SplashFlag)
    {
        platformPC.showHoldAMessage = HoldAFlag;
    }

	applicationStartTimeMS = GetTickCount();
	applicationElapsedMS = 0;

    // Select display and update functions
    if (SplashFlag)
    {
		m_splashTimerId = SetTimer(windowHandle, NULL, 16, RenderSplash);
    }
    else if (HoldAFlag)
    {
		m_splashTimerId = SetTimer(windowHandle, NULL, 16, RenderAHold);
        Log_Info("\n>> MGDL INIT COMPLETE\n");
        Log_Info(">> Hold A button to continue\n");
    }
    else
    {
		m_renderTimerId = SetTimer(windowHandle, NULL, 16, RenderLoop);
    }

	while (true)
	{
		MSG message;
		BOOL messageResult = GetMessageA(&message, 0, 0, 0);
		if (messageResult > 0)
		{
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
		else if (messageResult == 0)
		{
			// 0 is WM_QUIT
			OutputDebugStringA("Message WM_QUIT\n");
			Log_Info("Got WM_QUIT");
			break;
		}
		else
		{
			// < 0 is error
			OutputDebugStringA("Message Error\n");
			Log_Info("GetMessage Error");
			break;
		}
	}

	ShowCursor(TRUE);
	OutputDebugStringA("InitSystem over\n");
}

void Platform_InitControllers()
{
	InitPCInput();

    WiiController_Init(&kbmController, 0);

    WiiController_ZeroAllInputs(&kbmController);
    WiiController_StartFrame(&kbmController);

    // Init Joystick
    platformPC.gamepads[0] = Joystick_Create(0);
    platformPC.gamepads[1] = Joystick_Create(1);
    platformPC.gamepads[2] = Joystick_Create(2);
    platformPC.gamepads[3] = Joystick_Create(3);
    if (Joystick_IsConnected(platformPC.gamepads[0]))
    {
        Joystick_ZeroInputs(platformPC.gamepads[0]);
    }
}

// This is called in response to WM_CLOSE
// Cannot be called after WM_DESTROY because
// at that point windowHandle is invalid
void Platform_DoProgramExit(void)
{
	if (quitCall != NULL)
	{
		quitCall();
	}
	wglMakeCurrent(deviceContextHandle, NULL); // Detach context from window
	wglDeleteContext(openGLContext);
	ReleaseDC(windowHandle, deviceContextHandle);
	DestroyWindow(windowHandle);
}

struct Platform* Platform_GetSingleton(void)
{
	return &platformPC;
}
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

#endif 
