#include <mgdl.h>
#include "example.h"

//#define MGDL_ROCKET

#ifdef MGDL_ROCKET
#include "mgdl-rocket.h"

#ifdef SYNC_PLAYER
    #include MGDL_ROCKET_FILE_H
    #include MGDL_ROCKET_FILE_CPP
#else
    static ROCKET_TRACK clear_r;
#endif

#endif

static Example example;

//---------------------------------------------------------------------

void init()
{
    glViewport(0, 0, mgdl_GetScreenWidth(), mgdl_GetScreenHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    u32 black = Palette_GetColor(Palette_GetDefault(), 0);
    Color4f bf = ColorToFloats(black);
    glClearColor(bf.red, bf.green, bf.blue, bf.alpha);
    example.Init();

#ifdef MGDL_ROCKET
    bool rocketInit = Rocket_Init(numbers, 120, 4);
    if (rocketInit == false)
    {
        DoProgramExit();
    }
#ifndef SYNC_PLAYER
    clear_r = Rocket_AddTrack("clear_r");
#endif
    Rocket_StartSync();
#endif
}

void frame()
{
    example.Update();

    // NOTE Use the mgdl_glClear to assure depth buffer working correctly on Wii
    mgdl_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    example.Draw();

    if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::ButtonHome))
    {
        mgdl_DoProgramExit();
    }


#ifdef MGDL_ROCKET

    gdl::RocketSync::UpdateRow();
    float r = gdl::RocketSync::GetFloat(clear_r);
    float g = 1.0f;
    float b = 174.0f/255.0f;
    glClearColor(r,g ,b , 0.0f);

#ifndef SYNC_PLAYER

    if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::Button2))
    {
       gdl::RocketSync::StartSaveToHeader();
       gdl::RocketSync::SaveTrack(clear_r);
       gdl::RocketSync::EndSaveToHeader();
    }
#endif
#endif
}


int main()
{
    mgdl_InitSystem("mgdl example project",
        ScreenAspect::Screen4x3,
        init,
        frame,
        FlagNone
        //,gdl::PlatformInitFlag::FlagPauseUntilA
        //,gdl::PlatformInitFlag::FlagSplashScreen
        //,gdl::PlatformInitFlag::FlagSplashScreen|gdl::PlatformInitFlag::FlagPauseUntilA|gdl::PlatformInitFlag::FlagFullScreen
    );
}
