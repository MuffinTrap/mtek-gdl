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


void Init3D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0, gdl::GetAspectRatio(), 0.1, 100.0);
}
void Init2D()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, (double)gdl::GetScreenWidth(), 0.0, (double)gdl::GetScreenHeight());
}

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

    // Y increases down
    //gluOrtho2D(0.0, (double)gdl::GetScreenWidth(), (double)gdl::GetScreenHeight(), 0.0f);

    // Y increases up : OpenGL default
    gluOrtho2D(0.0, (double)gdl::GetScreenWidth(), 0.0, (double)gdl::GetScreenHeight());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glClearColor(247.0f/255.0f, 1.0f, 174.0f/255.0f, 0.0f);
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
    float g = 1.0f;
    float b = 174.0f/255.0f;
    glClearColor(r,g ,b , 0.0f);
#endif
}

// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    // NOTE Use this instead of glClear() for Wii quirk.
    gdl::cross_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    example.Draw();
    Cross2D();
}

int main()
{
    gdl::InitSystem(
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render
    );
}
