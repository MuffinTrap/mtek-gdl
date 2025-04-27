#include <mgdl.h>

//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
    gdl::InitOrthoProjection();

    glClearColor(0.4f, 0.4f, 0.39f, 1.0f);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
}


// Called before render()
void update()
{
    if (WiiController_ButtonPress(GetController(0), WiiButtons::ButtonHome))
    {
        DoProgramExit();
    }
}

int main()
{
    u32 flags = 0;//gdl::PlatformInitFlag::FlagSplashScreen;
    InitSystem(MGDL_PLATFORM,
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render,
        flags
    );
}
