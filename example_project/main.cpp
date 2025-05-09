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
    gdl::RGBA8Floats bf = gdl::ColorToFloats(black);
    glClearColor(bf.red, bf.green, bf.blue, bf.alpha);
    example.Init();

#ifdef MGDL_ROCKET
    bool rocketInit = gdl::RocketSync::InitRocket(numbers, 120, 4);
    if (rocketInit == false)
    {
        DoProgramExit();
    }
#ifndef SYNC_PLAYER
    clear_r = gdl::RocketSync::GetTrack("clear_r");
#endif
    gdl::RocketSync::StartSync();
#endif
}

// Called before render()
void update()
{
    if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::ButtonHome))
    {
        mgdl_DoProgramExit();
    }
    //example.Update();

#ifdef MGDL_ROCKET
    float r = 1.0f;
    gdl::RocketSync::UpdateRow();
    r = gdl::RocketSync::GetFloat(clear_r);
#ifndef SYNC_PLAYER

    if (WiiController_ButtonPress(mgdl_GetController(0), WiiButtons::Button2))
    {
       gdl::RocketSync::StartSaveToHeader();
       gdl::RocketSync::SaveTrack(clear_r);
       gdl::RocketSync::EndSaveToHeader();
    }
#endif
/*
    float g = 1.0f;
    float b = 174.0f/255.0f;
    glClearColor(r,g ,b , 0.0f);
    */
#endif
}

// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    // NOTE Use this instead of glClear() for Wii quirk.
    mgdl_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    example.Draw();
}

int main()
{
    InitSystem(MGDL_PLATFORM,
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render,
        0
        //,gdl::PlatformInitFlag::FlagPauseUntilA
        //,gdl::PlatformInitFlag::FlagSplashScreen
        //,gdl::PlatformInitFlag::FlagSplashScreen|gdl::PlatformInitFlag::FlagPauseUntilA|gdl::PlatformInitFlag::FlagFullScreen
    );
}
