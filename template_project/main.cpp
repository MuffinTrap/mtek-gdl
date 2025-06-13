#include <mgdl.h>

//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, mgdl_GetScreenWidth(), mgdl_GetScreenHeight());
    mgdl_InitOrthoProjection();

    glClearColor(0.4f, 0.4f, 0.39f, 1.0f);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    mgdl_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


// Called before render()
void update()
{
    if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::ButtonHome))
    {
        mgdl_DoProgramExit();
    }
}

int main()
{
    u32 flags = 0;//gdl::PlatformInitFlag::FlagSplashScreen;
    mgdl_InitSystem(MGDL_PLATFORM,
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render,
        flags
    );
}
