#ifdef MGDL_PLATFORM_WINDOWS

#include <mgdl/mgdl-platform.h>
#include <mgdl/mgdl-assert.h>
#include <mgdl/mgdl-logger.h>
#include <mgdl/pc/mgdl-joystick.h>
#include "Windows.h"

static Platform platformPC;

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
static UINT_PTR m_timerId;
static BOOL windowIsVisible;

// Controller(s)
static WiiController win32Controller;
void ErrorExit()
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

void Platform_RenderEnd(Platform* platform)
{
	SwapBuffers(deviceContextHandle);
}

void RenderLoop()
{
	frameCall();

    Platform_RenderEnd(&platformPC);

    WiiController* controller = Platform_GetController(0);

    if (WiiController_ButtonPress(controller, ButtonHome))
    {
		DestroyWindow(windowHandle);
    }
    // Reset controller for next frame
    WiiController_StartFrame(controller);
	Joystick* gamepad_0 = platformPC.gamepad_0;
    if (Joystick_IsConnected(gamepad_0))
    {
        Joystick_ReadInputs(gamepad_0);
        // Always read cursor from glut
        controller->m_cursorX = win32Controller.m_cursorX;
        controller->m_cursorY = win32Controller.m_cursorY;
    }
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
			// Windows has created the window and we
			// can set up OpenGL context
			/*
			pixelFormatEnum = ChoosePixelFormat(deviceContextHandle, &pixelFormatDesc);
			SetPixelFormat(deviceContextHandle, pixelFormatEnum, &pixelFormatDesc);
			openGLContext = wglCreateContext(deviceContextHandle);
			wglMakeCurrent(deviceContextHandle, openGLContext);
			*/
		}
		break;
		case WM_ACTIVATEAPP: 
		{
			// If the window is visible and active : wParam : BOOL
			Log_Info("WM_ACTIVATEAPP\n");
			windowIsVisible = wParam;
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
			// Windows calls this when timer is up
			// RenderLoop();
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
			//KillTimer(windowHandle, m_timerId); // Stop calling the render loop
			Platform_DoProgramExit();
			DestroyWindow(target_windowHandle);
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
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // WS_CLIPCHILDREN needed for OpenGL
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

	// Get the Device Context for our window
	// client area : Must be released
	/*
	deviceContextHandle = GetDC(windowHandle);
	if (deviceContextHandle == NULL)
	{
		Log_Error("Could not get device context handle\n");
		return;
	}
	*/

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

	// Set up 60fps timer.
	// Setting NULL as lpTimerFunc makes windows send WM_TIMER
	// messages
	//SetUserObjectInformationW(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, FALSE, 1);
	//m_timerId = SetTimer(windowHandle, NULL, 16, NULL);

	// Main loop gets messages from Windows
	OutputDebugStringA("Start Message Loop\n");

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

	OutputDebugStringA("InitSystem over\n");
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
	//wglMakeCurrent(NULL, NULL); // Detach context from window
	//wglDeleteContext(openGLContext);
	//ReleaseDC(windowHandle, deviceContextHandle);
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
	return &win32Controller;
}

#endif 
