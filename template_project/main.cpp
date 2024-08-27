#include <mgdl.h>
#include "mgdl-rocket.h"

#ifdef SYNC_PLAYER
    #include MGDL_ROCKET_FILE_H
    #include MGDL_ROCKET_FILE_CPP
#else
    static ROCKET_TRACK clear_r;
#endif


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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Init3D();

    glClearColor(247.0f/255.0f, 1.0f, 174.0f/255.0f, 0.0f);
    gdl::Sound* numbers = gdl::LoadSound("numbers.wav");

    gdl::RocketSync::InitRocket(numbers, 120, 4);

#ifndef SYNC_PLAYER
    clear_r = gdl::RocketSync::GetTrack("clear_r");
#endif

    gdl::RocketSync::StartSync();
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Cross3D();
}


// Called before render()
void update()
{
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonHome))
    {
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        plat.DoProgramExit();
    }

#ifndef SYNC_PLAYER
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::Button2))
    {
        gdl::RocketSync::StartSaveToHeader();
        gdl::RocketSync::SaveTrack(clear_r);
        gdl::RocketSync::EndSaveToHeader();
    }
#endif

    gdl::RocketSync::UpdateRow();

    float r = 1.0f;

    r = 247.0f/255.0f;
    r = gdl::RocketSync::GetFloat(clear_r);

    float g = 1.0f;
    float b = 174.0f/255.0f;

    glClearColor(r,g ,b , 0.0f);
}

int main()
{
    gdl::InitSystem(
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render,
        gdl::PlatformInitFlag::FlagPauseUntilA
    );
}
