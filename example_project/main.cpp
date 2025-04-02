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

void Cross3D()
{
    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);

        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();
}

void Cross2D()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f((float)gdl::GetScreenWidth(), (float)gdl::GetScreenHeight());

    glVertex2f(0.0f, (float)gdl::GetScreenHeight());
    glVertex2f((float)gdl::GetScreenWidth(), 0.0f);
    glEnd();
}


//---------------------------------------------------------------------

void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    u32 black = gdl::Palette::GetDefaultPalette()->GetColor(0);
    gdl::RGBA8Floats bf = gdl::ColorToFloats(black);
    glClearColor(bf.red, bf.green, bf.blue, bf.alpha);
    example.Init();

#ifdef MGDL_ROCKET
    bool rocketInit = gdl::RocketSync::InitRocket(numbers, 120, 4);
    if (rocketInit == false)
    {
        gdl::DoProgramExit();
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
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonHome))
    {
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        plat.DoProgramExit();
    }
    example.Update();

#ifdef MGDL_ROCKET
    float r = 1.0f;
    gdl::RocketSync::UpdateRow();
    r = gdl::RocketSync::GetFloat(clear_r);
#ifndef SYNC_PLAYER
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::Button2))
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
    gdl::cross_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    example.Draw();
}

int main()
{
    gdl::InitSystem(MGDL_PLATFORM,
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render
        //,gdl::PlatformInitFlag::FlagPauseUntilA
        //,gdl::PlatformInitFlag::FlagSplashScreen
        //,gdl::PlatformInitFlag::FlagSplashScreen|gdl::PlatformInitFlag::FlagPauseUntilA|gdl::PlatformInitFlag::FlagFullScreen
    );
}
