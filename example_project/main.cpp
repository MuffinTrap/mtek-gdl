#include <mgdl.h>
#include "example.h"

static Example example;

void init()
{
    glViewport(0, 0, mgdl_GetScreenWidth(), mgdl_GetScreenHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    example.Init();
}

void frame()
{
    example.Update();

    // NOTE Use the mgdl_glClear to assure depth buffer working correctly on Wii
    mgdl_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    example.Draw();
}

void quit()
{
    // Called before program exits
    // Use this to free any resources and disconnect rocket
    example.Quit();
}

void init_system()
{
    mgdl_InitSystem("mgdl example project",
        ScreenAspect::Screen4x3,
            init,
            frame,
            quit,
        FlagNone
        // | FlagFullScreen
         | FlagSplashScreen
        // | FlagPauseUntilA
    );
}

// Linux, Mac, MSYS2(Windows) and Wii all use gcc or clang, and that looks for int main
// Windows native build looks for WinMain

#if defined(MGDL_PLATFORM_LINUX) || defined(MGDL_PLATFORM_MAC) || defined(MGDL_PLATFORM_WII) || defined(MGDL_PLATFORM_WINDOWS)

int main()
{
    init_system();
    return 0;
}
#else

#include <windows.h>

int CALLBACK
WinMain( HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow )
{
    init_system();
    return(0);
}


#endif
